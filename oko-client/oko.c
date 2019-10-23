#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/sysctl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "oko.h"

static int
retrieve_type(machine *machine)
{
    size_t len;
    
    if (sysctlbyname("hw.targettype", NULL, &len, NULL, 0)) {
        return 1;
    }
    machine->type = malloc(len);
    if (sysctlbyname("hw.targettype", machine->type, &len, NULL, 0)) {
        return 1;
    }
    
    return 0;
}

static int
retrieve_model(machine *machine)
{
    size_t len;
    
    if (sysctlbyname("hw.model", NULL, &len, NULL, 0)) {
        return 1;
    }
    machine->model = malloc(len);
    if (sysctlbyname("hw.model", machine->model, &len, NULL, 0)) {
        return 1;
    }
    
    return 0;
}

static int
retrieve_hostname(machine *machine)
{
    size_t len;
    
    if (sysctlbyname("kern.hostname", NULL, &len, NULL, 0)) {
        return 1;
    }
    machine->hostname = malloc(len);
    if (sysctlbyname("kern.hostname", machine->hostname, &len, NULL, 0)) {
        return 1;
    }
    
    return 0;
}

static int
retrieve_ipv4(machine *machine)
{
    struct ifaddrs *ifa;
    struct ifaddrs *ifs;
    struct sockaddr_in *ipv4;
    char *ip_if;

    ipv4 = malloc(sizeof(struct sockaddr_in));
    if (getifaddrs(&ifs)) {
        return 1;
    }
    ip_if = strdup("en0");
    
    for (ifa = ifs; ifa != NULL; ifa = ifa->ifa_next) {
        if (!strcmp(ifa->ifa_name, ip_if)) {
            if (ifa->ifa_addr->sa_family == AF_INET) {
                ipv4->sin_addr.s_addr = ((struct sockaddr_in*)(ifa->ifa_addr))->sin_addr.s_addr;
                machine->ipv4 = malloc(INET_ADDRSTRLEN);
                strncpy(machine->ipv4, inet_ntoa(ipv4->sin_addr), INET_ADDRSTRLEN);
                
                return 0;
            }
        }
    }
    
    return 1;
}

static int
retrieve_ipv6(machine *machine)
{
    struct ifaddrs *ifa;
    struct ifaddrs *ifs;
    struct sockaddr_in6 *ipv6;
    char *ip_if;

    ipv6 = malloc(sizeof(struct sockaddr_in6));
    if (getifaddrs(&ifs)) {
        return 1;
    }
    ip_if = strdup("en0");
    
    for (ifa = ifs; ifa != NULL; ifa = ifa->ifa_next) {
        if (!strcmp(ifa->ifa_name, ip_if)) {
            if (ifa->ifa_addr->sa_family == AF_INET6) {
                ipv6->sin6_addr = ((struct sockaddr_in6*)(ifa->ifa_addr))->sin6_addr;
                machine->ipv6 = malloc(INET6_ADDRSTRLEN);
                inet_ntop(AF_INET6, (const void *)&ipv6->sin6_addr, machine->ipv6, INET6_ADDRSTRLEN);
                
                return 0;
            }
        }
    }
    
    return 1;
}

static int
retrieve_macaddress(machine *machine)
{
    struct ifaddrs *ifa;
    struct ifaddrs *ifs;
    unsigned char *macaddr;
    char *ip_if;

    if (getifaddrs(&ifs)) {
        return 1;
    }
    ip_if = strdup("en0");
    
    for (ifa = ifs; ifa != NULL; ifa = ifa->ifa_next) {
        if (!strcmp(ifa->ifa_name, ip_if)) {
            if (ifa->ifa_addr->sa_family == AF_LINK) {
                macaddr = malloc(sizeof(struct sockaddr));
                macaddr = (unsigned char*)((struct sockaddr*)ifa->ifa_addr->sa_data);
                macaddr += 9; /* To get the real MAC address */
                machine->macaddress = malloc(sizeof(struct sockaddr));
                sprintf(machine->macaddress,
                    "%02x:%02x:%02x:%02x:%02x:%02x",
                    *macaddr,
                    *(macaddr + 1),
                    *(macaddr + 2),
                    *(macaddr + 3),
                    *(macaddr + 4),
                    *(macaddr + 5));
                
                return 0;
            }
        }
    }
    
    return 1;
}

static int
retrieve_cpu_arch(machine *machine)
{
    size_t len;
    
    if (sysctlbyname("hw.machine", NULL, &len, NULL, 0)) {
        return 1;
    }
    machine->cpu.arch = malloc(len);
    if (sysctlbyname("hw.machine", machine->cpu.arch, &len, NULL, 0)) {
        return 1;
    }
    
    return 0;
}

static int
retrieve_cpu_model(machine *machine)
{
    size_t len;
    
    if (sysctlbyname("machdep.cpu.brand_string", NULL, &len, NULL, 0)) {
        return 1;
    }
    machine->cpu.model = malloc(len);
    if (sysctlbyname("machdep.cpu.brand_string", machine->cpu.model, &len, NULL, 0)) {
        return 1;
    }
    
    return 0;
}

static int
retrieve_ncpus(machine *machine)
{
    size_t len;
    
    len = sizeof(machine->cpu.ncpus);
    if (sysctlbyname("hw.ncpu", &(machine->cpu.ncpus), &len, NULL, 0)) {
        return 1;
    }
    
    return 0;
}

static int
retrieve_physmem(machine *machine)
{
    size_t len;
    
    len = sizeof(machine->memory.physmem);
    if (sysctlbyname("hw.memsize", &(machine->memory.physmem), &len, NULL, 0)) {
        return 1;
    }
    
    return 0;
}

static int
retrieve_os_name(machine *machine)
{
    size_t len;
    
    if (sysctlbyname("kern.ostype", NULL, &len, NULL, 0)) {
        return 1;
    }
    machine->os.name = malloc(len);
    if (sysctlbyname("kern.ostype", machine->os.name, &len, NULL, 0)) {
        return 1;
    }
    
    return 0;
}

static int
retrieve_os_release(machine *machine)
{
    size_t len;
    
    if (sysctlbyname("kern.osrelease", NULL, &len, NULL, 0)) {
        return 1;
    }
    machine->os.release = malloc(len);
    if (sysctlbyname("kern.osrelease", machine->os.release, &len, NULL, 0)) {
        return 1;
    }
    
    return 0;
}

static int
retrieve_os_version(machine *machine)
{
    size_t len;
    
    if (sysctlbyname("kern.version", NULL, &len, NULL, 0) != 0) {
        return 1;
    }
    machine->os.version = malloc(len);
    if (sysctlbyname("kern.version", machine->os.version, &len, NULL, 0)) {
        return 1;
    }
    
    return 0;
}

int
collect_info(machine *machine)
{
    int nerrors = 0;
    
    /* id */
    machine->id = 0;
    
    /* type */
    nerrors += retrieve_type(machine);
    
    /* model */
    nerrors += retrieve_model(machine);
    
    /* hostname */
    nerrors += retrieve_hostname(machine);
    
    /* ipv4 */
    nerrors += retrieve_ipv4(machine);
    
    /* ipv6 */
    nerrors += retrieve_ipv6(machine);
    
    /* mac_address */
    nerrors += retrieve_macaddress(machine);
    
    /* cpu.arch */
    nerrors += retrieve_cpu_arch(machine);
    
    /* cpu.model */
    nerrors += retrieve_cpu_model(machine);
    
    /* cpu.ncpus */
    nerrors += retrieve_ncpus(machine);
    
    /* memory.phys_mem */
    nerrors += retrieve_physmem(machine);
    
    /* os.name */
    nerrors += retrieve_os_name(machine);
    
    /* os.release */
    nerrors += retrieve_os_release(machine);
    
    /* os.version */
    nerrors += retrieve_os_version(machine);
    
    return (nerrors);
}

static int
retrieve_cpu_usage(usage *usage)
{
    /* NOT SUPPORTED */
    usage->cpu.usage = 0.0;
    
    return 0;
}

static int
retrieve_memory_usage(usage *usage)
{
    size_t len;
    int32_t pagesize;
    int64_t free;
    int64_t pages;
    
    len = sizeof(pagesize);
    pagesize = 0;
    if (sysctlbyname("vm.pagesize", &pagesize, &len, NULL, 0)) {
        return 1;
    }
    
    len = sizeof(pages);
    pages = 0;
    if (sysctlbyname("vm.pages", &pages, &len, NULL, 0)) {
        return 1;
    }
    usage->memory.used = pagesize * pages;
    
    len = sizeof(free);
    free = 0;
    if (sysctlbyname("vm.page_free_count", &free, &len, NULL, 0)) {
        return 1;
    }
    usage->memory.free = pagesize * free;
    
    return 0;
}

static int
retrieve_memory_swapusage(usage *usage)
{
    size_t len;
    struct xsw_usage xsu;
    
    if (sysctlbyname("vm.swapusage", NULL, &len, NULL, 0)) {
        return 1;
    }
    if (sysctlbyname("vm.swapusage", &xsu, &len, NULL, 0)) {
        return 1;
    }
    usage->memory.swaptotal = xsu.xsu_total;
    usage->memory.swapused = xsu.xsu_used;
    usage->memory.swapfree = xsu.xsu_avail;
    
    return 0;
}



int
collect_usage(usage *usage)
{
    int nerrors = 0;
    
    /* cpu.usage */
    nerrors += retrieve_cpu_usage(usage);
    
    /* memory.used */
    /* memory.free */
    nerrors += retrieve_memory_usage(usage);
    
    /* memory.swaptotal */
    /* memory.swapused */
    /* memory.swapfree */
    nerrors += retrieve_memory_swapusage(usage);
    
    /* io.pckin */
    /* io.pckout */
    /* io.pckinsec */
    /* io.pckoutsec */
    /* io.datain */
    /* io.dataout */
    /* io.datainsec */
    /* io.dataoutsec */
    
    /* network.pckin */
    /* network.pckout */
    /* network.pckinsec */
    /* network.pckoutsec */
    /* network.datain */
    /* network.dataout */
    /* network.datainsec */
    /* network.dataoutsec */
    
    return (nerrors);
}
