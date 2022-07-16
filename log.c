#include <stdio.h>
#include <unistd.h>

#include "log.h"
#include "cJSON.h"

void
log_machine_info(machine *machine)
{
	cJSON *json_machine = cJSON_CreateObject();
	
	cJSON *serialnumber = cJSON_CreateString(machine->serialnumber);
	cJSON_AddItemToObject(json_machine, "serialnumber", serialnumber);
	cJSON *type = cJSON_CreateString(machine->type);
	cJSON_AddItemToObject(json_machine, "type", type);
	cJSON *model = cJSON_CreateString(machine->model);
	cJSON_AddItemToObject(json_machine, "model", model);
	cJSON *hostname = cJSON_CreateString(machine->hostname);
	cJSON_AddItemToObject(json_machine, "hostname", hostname);
	
	cJSON *network = cJSON_CreateObject();
	cJSON_AddItemToObject(json_machine, "network", network);
	
	cJSON *ip = cJSON_CreateString(machine->network.ip);
	cJSON_AddItemToObject(network, "ip", ip);
	cJSON *macaddress = cJSON_CreateString(machine->network.macaddress);
	cJSON_AddItemToObject(network, "macaddress", macaddress);

	cJSON *cpu = cJSON_CreateObject();
	cJSON_AddItemToObject(json_machine, "cpu", cpu);
	
	cJSON *arch = cJSON_CreateString(machine->cpu.arch);
	cJSON_AddItemToObject(cpu, "arch", arch);
	cJSON *cpu_model = cJSON_CreateString(machine->cpu.model);
	cJSON_AddItemToObject(cpu, "model", cpu_model);
	cJSON *ncpus = cJSON_CreateNumber(machine->cpu.ncpus);
	cJSON_AddItemToObject(cpu, "ncpus", ncpus);

	cJSON *memory = cJSON_CreateObject();
	cJSON_AddItemToObject(json_machine, "memory", memory);
	
	cJSON *physmem = cJSON_CreateNumber(machine->memory.physmem);
	cJSON_AddItemToObject(memory, "physmem", physmem);

	cJSON *os = cJSON_CreateObject();
	cJSON_AddItemToObject(json_machine, "os", os);
	
	cJSON *name = cJSON_CreateString(machine->os.name);
	cJSON_AddItemToObject(os, "name", name);
	cJSON *release = cJSON_CreateString(machine->os.release);
	cJSON_AddItemToObject(os, "release", release);
	cJSON *version = cJSON_CreateString(machine->os.version);
	cJSON_AddItemToObject(os, "version", version);
	
	printf("%s\n", cJSON_PrintUnformatted(json_machine));
}

void
log_machine_usage(usage *usage)
{
	cJSON *json_usage = cJSON_CreateObject();
	
	cJSON *cpu = cJSON_CreateObject();
	cJSON_AddItemToObject(json_usage, "cpu", cpu);

	cJSON *user = cJSON_CreateNumber(usage->cpu.user);
	cJSON_AddItemToObject(cpu, "user", user);
	cJSON *system = cJSON_CreateNumber(usage->cpu.system);
	cJSON_AddItemToObject(cpu, "system", system);
	cJSON *idle = cJSON_CreateNumber(usage->cpu.idle);
	cJSON_AddItemToObject(cpu, "idle", idle);
	cJSON *nice = cJSON_CreateNumber(usage->cpu.nice);
	cJSON_AddItemToObject(cpu, "nice", nice);

	cJSON *memory = cJSON_CreateObject();
	cJSON_AddItemToObject(json_usage, "memory", memory);

	cJSON *used = cJSON_CreateNumber(usage->memory.used);
	cJSON_AddItemToObject(memory, "used", used);
	cJSON *free = cJSON_CreateNumber(usage->memory.free);
	cJSON_AddItemToObject(memory, "free", free);
	cJSON *swaptotal = cJSON_CreateNumber(usage->memory.swaptotal);
	cJSON_AddItemToObject(memory, "swaptotal", swaptotal);
	cJSON *swapused = cJSON_CreateNumber(usage->memory.swapused);
	cJSON_AddItemToObject(memory, "swapused", swapused);
	cJSON *swapfree = cJSON_CreateNumber(usage->memory.swapfree);
	cJSON_AddItemToObject(memory, "swapfree", swapfree);

	printf("%s\n", cJSON_PrintUnformatted(json_usage));
}

int
log_mode(int interval)
{
	machine m;
	usage u;
	
	collect_machine_info(&m);
	log_machine_info(&m);
	
	for (;;) {
		log_machine_usage(&u);
		sleep(interval);
	}
	
	return 0;	
}
