#ifndef display_h
#define display_h

#include "oko.h"

typedef struct {
	WINDOW *win;
	int line;
	int col;
	usage *usage;
	int interval;
} usage_display_data;

void print_instructions(WINDOW *win, int line, int col);
void print_machine_info(WINDOW *win, int line, int col, machine *machine);
void print_machine_usage(WINDOW *win, int line, int col, usage *usage);
void *print_machine_usage_routine(usage_display_data *udd);

int display_mode(int interval);

#endif /* display_h */
