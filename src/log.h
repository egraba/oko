#ifndef log_h
#define log_h

#include <time.h>

#include "cJSON.h"
#include "oko.h"

cJSON *json_machine;
cJSON *serialnumber;
cJSON *type;
cJSON *model;
cJSON *hostname;
cJSON *network;
cJSON *ip;
cJSON *macaddress;
cJSON *cpu;
cJSON *cpu_arch;
cJSON *cpu_model;
cJSON *ncpus;
cJSON *memory;
cJSON *physmem;
cJSON *os;
cJSON *name;
cJSON *release;
cJSON *version;

cJSON *json_usage;
cJSON *cpu;
cJSON *cpu_user;
cJSON *cpu_system;
cJSON *cpu_idle;
cJSON *cpu_nice;
cJSON *memory;
cJSON *memory_used;
cJSON *memory_free;
cJSON *swaptotal;
cJSON *swapused;
cJSON *swapfree;
cJSON *network;
cJSON *pckin;
cJSON *pckout;

void init_machine_info_json(machine *machine);
void init_machine_usage_json(usage *usage);
void log_machine_info(machine *machine);
void log_machine_usage(usage *usage);

int log_mode(int interval);

#endif /* log_h */
