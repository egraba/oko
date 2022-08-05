#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "cJSON.h"

void
init_machine_info_json(machine *machine)
{
	json_machine = cJSON_CreateObject();
	
	serialnumber = cJSON_CreateString(machine->serialnumber);
	cJSON_AddItemToObject(json_machine, "serialnumber", serialnumber);
	type = cJSON_CreateString(machine->type);
	cJSON_AddItemToObject(json_machine, "type", type);
	model = cJSON_CreateString(machine->model);
	cJSON_AddItemToObject(json_machine, "model", model);
	hostname = cJSON_CreateString(machine->hostname);
	cJSON_AddItemToObject(json_machine, "hostname", hostname);
	
	network = cJSON_CreateObject();
	cJSON_AddItemToObject(json_machine, "network", network);
	
	ip = cJSON_CreateString(machine->network.ip);
	cJSON_AddItemToObject(network, "ip", ip);
	macaddress = cJSON_CreateString(machine->network.macaddress);
	cJSON_AddItemToObject(network, "macaddress", macaddress);

	cpu = cJSON_CreateObject();
	cJSON_AddItemToObject(json_machine, "cpu", cpu);
	
	cpu_arch = cJSON_CreateString(machine->cpu.arch);
	cJSON_AddItemToObject(cpu, "arch", cpu_arch);
	cpu_model = cJSON_CreateString(machine->cpu.model);
	cJSON_AddItemToObject(cpu, "model", cpu_model);
	ncpus = cJSON_CreateNumber(machine->cpu.ncpus);
	cJSON_AddItemToObject(cpu, "ncpus", ncpus);

	memory = cJSON_CreateObject();
	cJSON_AddItemToObject(json_machine, "memory", memory);
	
	physmem = cJSON_CreateNumber(machine->memory.physmem);
	cJSON_AddItemToObject(memory, "physmem", physmem);

	os = cJSON_CreateObject();
	cJSON_AddItemToObject(json_machine, "os", os);
	
	name = cJSON_CreateString(machine->os.name);
	cJSON_AddItemToObject(os, "name", name);
	release = cJSON_CreateString(machine->os.release);
	cJSON_AddItemToObject(os, "release", release);
	version = cJSON_CreateString(machine->os.version);
	cJSON_AddItemToObject(os, "version", version);
}

void
log_machine_info(machine *machine)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	cJSON_SetValuestring(serialnumber, machine->serialnumber);
	cJSON_SetValuestring(type, machine->type);
	cJSON_SetValuestring(model, machine->model);
	cJSON_SetValuestring(hostname, machine->hostname);
	
	cJSON_SetValuestring(ip, machine->network.ip);
	cJSON_SetValuestring(macaddress, machine->network.macaddress);
	
	cJSON_SetValuestring(cpu_arch, machine->cpu.arch);
	cJSON_SetValuestring(cpu_model, machine->cpu.model);
	cJSON_SetNumberValue(ncpus, machine->cpu.ncpus);
	
	cJSON_SetNumberValue(physmem, machine->memory.physmem);
	
	cJSON_SetValuestring(name, machine->os.name);
	cJSON_SetValuestring(release, machine->os.release);
	cJSON_SetValuestring(version, machine->os.version);
	
	printf("[oko] %02d-%02d-%02d %02d:%02d:%02d -- MACHINE -- %s\n",
		tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec,
		cJSON_PrintUnformatted(json_machine));
}

void
init_machine_usage_json(usage *usage)
{
	json_usage = cJSON_CreateObject();
	
	cpu = cJSON_CreateObject();
	cJSON_AddItemToObject(json_usage, "cpu", cpu);

	cpu_user = cJSON_CreateNumber(usage->cpu.user);
	cJSON_AddItemToObject(cpu, "user", cpu_user);
	cpu_system = cJSON_CreateNumber(usage->cpu.system);
	cJSON_AddItemToObject(cpu, "system", cpu_system);
	cpu_idle = cJSON_CreateNumber(usage->cpu.idle);
	cJSON_AddItemToObject(cpu, "idle", cpu_idle);
	cpu_nice = cJSON_CreateNumber(usage->cpu.nice);
	cJSON_AddItemToObject(cpu, "nice", cpu_nice);

	memory = cJSON_CreateObject();
	cJSON_AddItemToObject(json_usage, "memory", memory);

	memory_used = cJSON_CreateNumber(usage->memory.used);
	cJSON_AddItemToObject(memory, "used", memory_used);
	memory_free = cJSON_CreateNumber(usage->memory.free);
	cJSON_AddItemToObject(memory, "free", memory_free);
	swaptotal = cJSON_CreateNumber(usage->memory.swaptotal);
	cJSON_AddItemToObject(memory, "swaptotal", swaptotal);
	swapused = cJSON_CreateNumber(usage->memory.swapused);
	cJSON_AddItemToObject(memory, "swapused", swapused);
	swapfree = cJSON_CreateNumber(usage->memory.swapfree);
	cJSON_AddItemToObject(memory, "swapfree", swapfree);

	network = cJSON_CreateObject();
	cJSON_AddItemToObject(json_usage, "network", network);

	pckin = cJSON_CreateNumber(usage->network.pckin);
	cJSON_AddItemToObject(network, "pckin", pckin);
	pckout = cJSON_CreateNumber(usage->network.pckout);
	cJSON_AddItemToObject(network, "pckout", pckout);
}

void
log_machine_usage(usage *usage)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	cJSON_SetNumberValue(cpu_user, usage->cpu.user);
	cJSON_SetNumberValue(cpu_system, usage->cpu.system);
	cJSON_SetNumberValue(cpu_idle, usage->cpu.idle);
	cJSON_SetNumberValue(cpu_nice, usage->cpu.nice);
	
	cJSON_SetNumberValue(memory_used, usage->memory.used);
	cJSON_SetNumberValue(memory_free, usage->memory.free);
	cJSON_SetNumberValue(swaptotal, usage->memory.swaptotal);
	cJSON_SetNumberValue(swapused, usage->memory.swapused);
	cJSON_SetNumberValue(swapfree, usage->memory.swapfree);

	cJSON_SetNumberValue(pckin, usage->network.pckin);
	cJSON_SetNumberValue(pckout, usage->network.pckout);
	
	printf("[oko] %02d-%02d-%02d %02d:%02d:%02d -- USAGE -- %s\n",
		tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec,
		cJSON_PrintUnformatted(json_usage));
}

int
log_mode(int interval)
{
	machine m;
	usage u;
	
	collect_machine_info(&m);

	init_machine_info_json(&m);
	init_machine_usage_json(&u);
	
	log_machine_info(&m);
	
	for (;;) {
		collect_machine_usage(&u);
		log_machine_usage(&u);
		sleep(interval);
	}
	
	return 0;	
}
