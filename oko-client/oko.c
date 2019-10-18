#include <sys/sysctl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "oko.h"

int
machine_info(machine *machine)
{
    int rc;
    size_t len;
    int64_t mem;
    
    /* id */
    machine->id = 0;
    
    /* type */
    rc = sysctlbyname("hw.targettype", NULL, &len, NULL, 0);
    machine->type = malloc(len);
    rc = sysctlbyname("hw.targettype", machine->type, &len, NULL, 0);
    
    /* model */
    rc = sysctlbyname("hw.model", NULL, &len, NULL, 0);
    machine->model = malloc(len);
    rc = sysctlbyname("hw.model", machine->model, &len, NULL, 0);
    
    /* hostname */
    rc = sysctlbyname("kern.hostname", NULL, &len, NULL, 0);
    machine->hostname = malloc(len);
    rc = sysctlbyname("kern.hostname", machine->hostname, &len, NULL, 0);
    
    /* ipv4 */
    
    /* ipv6 */
    
    /* mac_address */
    
    /* cpu.arch */
    rc = sysctlbyname("hw.machine", NULL, &len, NULL, 0);
    machine->cpu.arch = malloc(len);
    rc = sysctlbyname("hw.machine", machine->cpu.arch, &len, NULL, 0);
    
    /* cpu.model */
    rc = sysctlbyname("machdep.cpu.brand_string", NULL, &len, NULL, 0);
    machine->cpu.model = malloc(len);
    rc = sysctlbyname("machdep.cpu.brand_string", machine->cpu.model, &len, NULL, 0);
    
    /* cpu.ncpus */
    len = sizeof(machine->cpu.ncpus);
    rc = sysctlbyname("hw.ncpu", &(machine->cpu.ncpus), &len, NULL, 0);
    
    /* memory.phys_mem */
    len = sizeof(mem);
    rc = sysctlbyname("hw.memsize", &mem, &len, NULL, 0);
    machine->memory.phys_mem = mem;
    
    /* memory.swap */
    //rc = sysctlbyname("vm.swapusage", NULL, &len, NULL, 0);
    //machine->memory.swap = malloc(len);
    //rc = sysctlbyname("vm.swapusage", machine->memory.swap, &len, NULL, 0);
    
    /* os.name */
    rc = sysctlbyname("kern.ostype", NULL, &len, NULL, 0);
    machine->os.name = malloc(len);
    rc = sysctlbyname("kern.ostype", machine->os.name, &len, NULL, 0);
    
    /* os.release */
    rc = sysctlbyname("kern.osrelease", NULL, &len, NULL, 0);
    machine->os.release = malloc(len);
    rc = sysctlbyname("kern.osrelease", machine->os.release, &len, NULL, 0);
    
    /* os.version */
    rc = sysctlbyname("kern.version", NULL, &len, NULL, 0);
    machine->os.version = malloc(len);
    rc = sysctlbyname("kern.version", machine->os.version, &len, NULL, 0);
    
    return (rc);
}
