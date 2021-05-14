#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "oko.h"

static void
print_machine_info(WINDOW *win, int line, int col, machine *machine)
{
	wattron(win, A_BOLD);
	mvwprintw(win, line, col, "Machine");
	wattroff(win, A_BOLD);

	mvwprintw(win, ++line, col, "id: %d", machine->id);
	mvwprintw(win, ++line, col, "type: %s", machine->type);
	mvwprintw(win, ++line, col, "model: %s", machine->model);
	mvwprintw(win, ++line, col, "hostname: %s", machine->hostname);
	mvwprintw(win, ++line, col, "ip: %s", machine->ip);
	mvwprintw(win, ++line, col, "macaddress: %s", machine->macaddress);
	mvwprintw(win, ++line, col, "cpu.arch: %s", machine->cpu.arch);
	mvwprintw(win, ++line, col, "cpu.model: %s", machine->cpu.model);
	mvwprintw(win, ++line, col, "cpu.ncpus: %d", machine->cpu.ncpus);
	mvwprintw(win, ++line, col, "memory.physmem: %lld", machine->memory.physmem);
	mvwprintw(win, ++line, col, "os.name: %s", machine->os.name);
	mvwprintw(win, ++line, col, "os.release %s", machine->os.release);
	mvwprintw(win, ++line, col, "os.version %s", machine->os.version);

	wrefresh(win);
}

static void
print_machine_usage(WINDOW *win, int line, int col, usage *usage)
{
	wattron(win, A_BOLD);
	mvwprintw(win, line, col, "Usage");
	wattroff(win, A_BOLD);

	mvwprintw(win, ++line, col, "cpu.user: %.2f %%", usage->cpu.user);
	mvwprintw(win, ++line, col, "cpu.system: %.2f %%", usage->cpu.system);
	mvwprintw(win, ++line, col, "cpu.idle: %.2f %%", usage->cpu.idle);
	mvwprintw(win, ++line, col, "cpu.nice: %.2f %%", usage->cpu.nice);
	mvwprintw(win, ++line, col, "memory.used: %lld", usage->memory.used);
	mvwprintw(win, ++line, col, "memory.free: %lld", usage->memory.free);
	mvwprintw(win, ++line, col, "memory.swaptotal: %lld", usage->memory.swaptotal);
	mvwprintw(win, ++line, col, "memory.swapused: %lld", usage->memory.swapused);
	mvwprintw(win, ++line, col, "memory.swapfree: %lld", usage->memory.swapfree);

	wrefresh(win);
}

int
main(int argc, const char * argv[])
{
	machine m;
	usage u;
	WINDOW *topw, *bottomw;

	initscr();
	topw = subwin(stdscr, LINES / 2, COLS, 0, 0);
	bottomw = subwin(stdscr, LINES / 2, COLS, LINES / 2, 0);
	box(topw, ACS_VLINE, ACS_HLINE);
	box(bottomw, ACS_VLINE, ACS_HLINE);
	
	collect_machine_info(&m);
	print_machine_info(topw, 1, 1, &m);

	for (;;) {
		collect_machine_usage(&u);
		print_machine_usage(bottomw, 1, 1, &u);
		sleep(2);
	}

	getch();
	endwin();

	free(topw);
	free(bottomw);
	
    
	return (EXIT_SUCCESS);
}
