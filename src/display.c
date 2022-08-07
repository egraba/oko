#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "display.h"
#include "utils.h"

static char *
human_readable(long memory_value)
{
	char *buf;
	float mv;
	char unit[5][2] = {"B", "KB", "MB", "GB", "TB"};
	int i = 0;

	mv = memory_value;
	for (i = 0; mv >= 1024; i++) {
		mv /= 1024;
	}
	buf = malloc(10);
	sprintf(buf, "%.2f%.2s", mv, unit[i]);

	return buf;
}

void
print_instructions(WINDOW *win, int line, int col)
{
	mvwprintw(win, line, col, "oko <o> - Press 'q' to exit.");
	wrefresh(win);
}

void
print_machine_info(WINDOW *win, int line, int col, machine *machine)
{
	wattron(win, A_BOLD);
	mvwprintw(win, line, col, "Machine");
	wattroff(win, A_BOLD);

	mvwprintw(win, ++line, col, "hardware serialnumber: %s, type: %s, model: %s",
		machine->hardware.serialnumber, machine->hardware.type, machine->hardware.model);
	mvwprintw(win, ++line, col, "network hostname: %s, ip: %s, macaddress: %s",
		machine->network.hostname, machine->network.ip, machine->network.macaddress);
	mvwprintw(win, ++line, col, "cpu arch: %s, model: %s, ncpus: %d",
		machine->cpu.arch, machine->cpu.model, machine->cpu.ncpus);
	mvwprintw(win, ++line, col, "memory physmem: %s, swaptotal: %s",
		human_readable(machine->memory.physmem), human_readable(machine->memory.swaptotal));
	mvwprintw(win, ++line, col, "os name: %s, release %s",
		machine->os.name, machine->os.release);
	
	wrefresh(win);
}

void
print_machine_usage(WINDOW *win, int line, int col, usage *usage)
{
	wattron(win, A_BOLD);
	mvwprintw(win, line, col, "Usage");
	wattroff(win, A_BOLD);

	mvwprintw(win, ++line, col, "cpu user: %.2f%%, system: %.2f%%, idle: %.2f%%, nice: %.2f%%",
		usage->cpu.user, usage->cpu.system, usage->cpu.idle, usage->cpu.nice);
	mvwprintw(win, ++line, col, "memory used: %s, free: %s, swapused: %s, swapfree: %s",
		human_readable(usage->memory.used), human_readable(usage->memory.free),
		human_readable(usage->memory.swapused), human_readable(usage->memory.swapfree));
	mvwprintw(win, ++line, col, "network pckin: %lld, pckout: %lld",
		usage->network.pckin, usage->network.pckout);

	wrefresh(win);
}

void*
print_machine_usage_routine(usage_display_data *udd)
{
	int startup = 1;

	for (;;) {
		collect_machine_usage(udd->usage);

		if (startup) sleep(1); /* To avoid strange screen at startup */
		
		box(udd->win, ACS_VLINE, ACS_HLINE);
		print_machine_usage(udd->win, udd->line, udd->col, udd->usage);
		wclear(udd->win);
		
		if (!startup) sleep(udd->interval);
		startup = 0;
	}
	
	return NULL;
}

int
display_mode(int interval)
{
	machine m;
	usage u;
	WINDOW *instw, *topw, *bottomw;
	usage_display_data udd;

	initscr();
	instw = subwin(stdscr, 1, COLS, 0, 0);  
	topw = subwin(stdscr, 7, COLS, 1, 0);
	bottomw = subwin(stdscr, LINES - 8, COLS, 8, 0);
	box(topw, ACS_VLINE, ACS_HLINE);
	
	print_instructions(instw, 0, 0);

	collect_machine_info(&m);
	print_machine_info(topw, 0, 1, &m);

	udd.win = bottomw;
	udd.line = 0;
	udd.col = 1;
	udd.usage = &u;
	udd.interval = interval;

	launch_thread(&print_machine_usage_routine, (usage_display_data *) &udd);
	
	noecho();
	while (getch() != 'q');
	endwin();

	free(topw);
	free(bottomw);
    
	return 0;
}
