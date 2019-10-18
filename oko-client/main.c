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
    printf("\tmac_address: %s\n", machine->mac_address);
    printf("\tcpu.arch: %s\n", machine->cpu.arch);
    printf("\tcpu.model: %s\n", machine->cpu.model);
    printf("\tcpu.ncpus: %d\n", machine->cpu.ncpus);
    printf("\tmemory.phys_mem: %ld\n", machine->memory.phys_mem);
    printf("\tmemory.swap: %d\n", machine->memory.swap);
    printf("\tos.name: %s\n", machine->os.name);
    printf("\tos.release %s\n", machine->os.release);
    printf("\tos.version %s\n", machine->os.version);
}

int
main(int argc, const char * argv[])
{
    machine m;
    
    machine_info(&m);
    print_machine(&m);
    
    return (EXIT_SUCCESS);
}
