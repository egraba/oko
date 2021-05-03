#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "oko.h"

static void
print_machine_info(machine *machine)
{
    printf("Machine\n");
    printf("======\n");
    
    printf("\tid: %d\n", machine->id);
    printf("\ttype: %s\n", machine->type);
    printf("\tmodel: %s\n", machine->model);
    printf("\thostname: %s\n", machine->hostname);
    printf("\tipv4: %s\n", machine->ipv4);
    printf("\tipv6: %s\n", machine->ipv6);
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
    printf("Usage\n");
    printf("=====\n");
    
    for (;;) {
        printf("\tcpu.user: %f\n", usage->cpu.user);
        printf("\tcpu.system: %f\n", usage->cpu.system);
        printf("\tcpu.idle: %f\n", usage->cpu.idle);
        printf("\tcpu.nice: %f\n", usage->cpu.nice);
        printf("\tmemory.used: %lld\n", usage->memory.used);
        printf("\tmemory.free: %lld\n", usage->memory.free);
        printf("\tmemory.swaptotal: %lld\n", usage->memory.swaptotal);
        printf("\tmemory.swapused: %lld\n", usage->memory.swapused);
        printf("\tmemory.swapfree: %lld\n", usage->memory.swapfree);
        sleep(1);
    }
}

int
main(int argc, const char * argv[])
{
    machine m;
    usage u;
    
    collect_machine_info(&m);
    print_machine_info(&m);
    
    collect_machine_usage(&u);
    print_machine_usage(&u);
    
    return (EXIT_SUCCESS);
}
