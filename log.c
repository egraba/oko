#include <stdio.h>

#include "log.h"

void
log_machine_info(machine *machine)
{
	printf("Machine");
	
	printf("serialnumber: %s", machine->serialnumber);
	printf("type: %s", machine->type);
	printf("model: %s", machine->model);
	printf("hostname: %s", machine->hostname);
	printf("network.ip: %s", machine->network.ip);
	printf("network.macaddress: %s", machine->network.macaddress);
	printf("cpu.arch: %s", machine->cpu.arch);
	printf("cpu.model: %s", machine->cpu.model);
	printf("cpu.ncpus: %d", machine->cpu.ncpus);
	printf("memory.physmem: %lld", machine->memory.physmem);
	printf("os.name: %s", machine->os.name);
	printf("os.release %s", machine->os.release);
	printf("os.version %s", machine->os.version);
	printf("\n");
}

void
log_machine_usage(usage *usage)
{
	printf("Usage");
	
	printf("cpu.user: %.2f %%", usage->cpu.user);
	printf("cpu.system: %.2f %%", usage->cpu.system);
	printf("cpu.idle: %.2f %%", usage->cpu.idle);
	printf("cpu.nice: %.2f %%", usage->cpu.nice);
	printf("memory.used: %lld", usage->memory.used);
	printf("memory.free: %lld", usage->memory.free);
	printf("memory.swaptotal: %lld", usage->memory.swaptotal);
	printf("memory.swapused: %lld", usage->memory.swapused);
	printf("memory.swapfree: %lld", usage->memory.swapfree);
	printf("\n");
}
