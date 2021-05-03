#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "oko.h"

static void
print_machine_info(machine *machine)
{
    printf("\tid: %d\n", machine->id);
    printf("\ttype: %s\n", machine->type);
    printf("\tmodel: %s\n", machine->model);
    printf("\thostname: %s\n", machine->hostname);
    printf("\tip: %s\n", machine->ip);
    printf("\tmacaddress: %s\n", machine->macaddress);
    printf("\tcpu.arch: %s\n", machine->cpu.arch);
    printf("\tcpu.model: %s\n", machine->cpu.model);
    printf("\tcpu.ncpus: %d\n", machine->cpu.ncpus);
    printf("\tmemory.physmem: %lld\n", machine->memory.physmem);
    printf("\tos.name: %s\n", machine->os.name);
    printf("\tos.release %s\n", machine->os.release);
    printf("\tos.version %s\n", machine->os.version);
}

static void
print_machine_usage(usage *usage)
{
    printf("\tcpu.user: %.2f %%\n", usage->cpu.user);
    printf("\tcpu.system: %.2f %%\n", usage->cpu.system);
    printf("\tcpu.idle: %.2f %%\n", usage->cpu.idle);
    printf("\tcpu.nice: %.2f %%\n", usage->cpu.nice);
    printf("\tmemory.used: %lld\n", usage->memory.used);
    printf("\tmemory.free: %lld\n", usage->memory.free);
    printf("\tmemory.swaptotal: %lld\n", usage->memory.swaptotal);
    printf("\tmemory.swapused: %lld\n", usage->memory.swapused);
    printf("\tmemory.swapfree: %lld\n", usage->memory.swapfree);
}

int
main(int argc, const char * argv[])
{
    machine m;
    usage u;
    
    collect_machine_info(&m);

    printf("Machine\n");
    printf("=======\n");
    print_machine_info(&m);
    
   
    for (;;) {
    	collect_machine_usage(&u);

	printf("Usage\n");
    	printf("=====\n");
    	print_machine_usage(&u);
	sleep(2);
    }

    return (EXIT_SUCCESS);
}
