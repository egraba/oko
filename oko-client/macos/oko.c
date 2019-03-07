#include <sys/sysctl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "oko.h"

int
device_info(device *device)
{
    int rc;
    size_t len;
    long mem;
    
    /* Device type */
    rc = sysctlbyname("hw.targettype", NULL, &len, NULL, 0);
    device->type = malloc(len);
    rc = sysctlbyname("hw.targettype", device->type, &len, NULL, 0);
    
    /* Device name */
    rc = sysctlbyname("hw.model", NULL, &len, NULL, 0);
    device->name = malloc(len);
    rc = sysctlbyname("hw.model", device->name, &len, NULL, 0);
    
    /* CPU architecture */
    rc = sysctlbyname("hw.machine", NULL, &len, NULL, 0);
    device->cpuarch = malloc(len);
    rc = sysctlbyname("hw.machine", device->cpuarch, &len, NULL, 0);
    
    /* CPU name */
    rc = sysctlbyname("machdep.cpu.brand_string", NULL, &len, NULL, 0);
    device->cpuname = malloc(len);
    rc = sysctlbyname("machdep.cpu.brand_string", device->cpuname, &len, NULL, 0);
    
    /* Number of CPUs */
    len = sizeof(device->ncpu);
    rc = sysctlbyname("hw.ncpu", &(device->ncpu), &len, NULL, 0);
    
    /* Hostname */
    rc = sysctlbyname("kern.hostname", NULL, &len, NULL, 0);
    device->hostname = malloc(len);
    rc = sysctlbyname("kern.hostname", device->hostname, &len, NULL, 0);
    
    /* OS type */
    rc = sysctlbyname("kern.ostype", NULL, &len, NULL, 0);
    device->ostype = malloc(len);
    rc = sysctlbyname("kern.ostype", device->ostype, &len, NULL, 0);
    
    /* OS version */
    rc = sysctlbyname("kern.version", NULL, &len, NULL, 0);
    device->osversion = malloc(len);
    rc = sysctlbyname("kern.version", device->osversion, &len, NULL, 0);
    
    /* Memory size (Mb) */
    len = sizeof(mem);
    rc = sysctlbyname("hw.memsize", &mem, &len, NULL, 0);
    device->memsize = mem / pow(1024, 2);
    
    return (rc);
}

static void
print_device(device *device)
{
    printf("Device\n");
    printf("======\n");
    printf("\ttype: %s\n", device->type);
    printf("\tname: %s\n", device->name);
    printf("\tcpuarch: %s\n", device->cpuarch);
    printf("\tcpuname: %s\n", device->cpuname);
    printf("\tncpu: %d\n", device->ncpu);
    printf("\thostname: %s\n", device->hostname);
    printf("\tostype: %s\n", device->ostype);
    printf("\tosversion: %s\n", device->osversion);
    printf("\tmemsize: %d\n", device->memsize);
    printf("\n");
}

int
main(int argc, const char * argv[])
{
    device d;
    
    device_info(&d);
    print_device(&d);
    
    return (EXIT_SUCCESS);
}
