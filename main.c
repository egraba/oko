#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <ncurses.h>

#include "display.h"
#include "log.h"
#include "oko.h"

#define DEFAULT_INTERVAL 2

static void
print_usage()
{
	printf("Usage: oko [options]\n");
	printf("Options:\n");
	printf("\t-h\t\tPrint this message and exit.\n");
	printf("\t-i [interval]\tSets refresh interval (in seconds).\n");
	printf("\t-l\t\tLog the output instead of displaying it on the terminal.\n");
}

int
main(int argc, char * const argv[])
{
	int opt;

	int no_option = 0;
	int is_help = 0;
	int is_interval = 0;
	int is_log = 0;

	char *interval_arg;
	int interval = DEFAULT_INTERVAL;
	
	if (argc < 2) {
		no_option = 1;
	}

	while ((opt = getopt(argc, argv, "hi:l")) != -1) {
		switch(opt) {
			case 'h':
				print_usage();
				return (EXIT_SUCCESS);

			case 'i':
				is_interval = 1;
				interval_arg = strdup(optarg);
				interval = strtonum(interval_arg, 0, 10, NULL);
				break;

			case 'l':
				is_log = 1;
				break;

			default:
				print_usage();
				return (EXIT_FAILURE);	
		}
	}

	if (is_help) {
		print_usage();
		return (EXIT_SUCCESS);
	}
	
	else if (no_option) {
		display_mode(interval);	
	}

	else if (!is_log && is_interval) {
		display_mode(interval);	
	} 
	
	else if (is_log) {
		log_mode(interval);	
	}

	else {
		print_usage();
		return (EXIT_SUCCESS);
	}

	return (EXIT_SUCCESS);
}
