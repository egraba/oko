#ifndef log_h
#define log_h

#include "cJSON.h"
#include "oko.h"

void init_machine_info_json(machine *machine);
void init_machine_usage_json(usage *usage);
void log_machine_info(machine *machine);
void log_machine_usage(usage *usage);

int log_mode(int interval);

#endif /* log_h */
