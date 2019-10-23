#include <stdio.h>
#include <stdlib.h>

#include "oko.h"

static void
print_machine(machine *machine)
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
    printf("\tmemory.swap: %lld\n", machine->memory.swaptotal);
    printf("\tos.name: %s\n", machine->os.name);
    printf("\tos.release %s\n", machine->os.release);
    printf("\tos.version %s\n", machine->os.version);
}

static void
print_usage(usage *usage)
{
    printf("Usage\n");
    printf("=====\n");
    
    for (;;) {
        printf("\tcpu.usage: %f\n", usage->cpu.usage);
    }
}

int
main(int argc, const char * argv[])
{
    machine m;
    usage u;
    
    collect_info(&m);
    print_machine(&m);
    
    collect_usage(&u);
    //print_usage(&u);
    
    return (EXIT_SUCCESS);
}
