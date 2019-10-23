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
    int rc;
    size_t len;
    
    rc = sysctlbyname("hw.targettype", NULL, &len, NULL, 0);
    if (rc != 0) {
        return (rc);
    }
    machine->type = malloc(len);
    rc = sysctlbyname("hw.targettype", machine->type, &len, NULL, 0);
    
    return (rc);
}

static int
retrieve_model(machine *machine)
{
    int rc;
    size_t len;
    
    rc = sysctlbyname("hw.model", NULL, &len, NULL, 0);
    if (rc != 0) {
        return (rc);
    }
    machine->model = malloc(len);
    rc = sysctlbyname("hw.model", machine->model, &len, NULL, 0);
    
    return (rc);
}

static int
retrieve_hostname(machine *machine)
{
    int rc;
    size_t len;
    
    rc = sysctlbyname("kern.hostname", NULL, &len, NULL, 0);
    if (rc != 0) {
        return (rc);
    }
    machine->hostname = malloc(len);
    rc = sysctlbyname("kern.hostname", machine->hostname, &len, NULL, 0);
    
    return (rc);
}

static int
retrieve_ipv4(machine *machine)
{
    int rc;
    struct ifaddrs *ifa;
    struct ifaddrs *ifs;
    struct sockaddr_in *ipv4;
    char *ip_if;

    ipv4 = malloc(sizeof(struct sockaddr_in));
    rc = getifaddrs(&ifs);
    if (rc != 0) {
        return (rc);
    }
    ip_if = strdup("en0");
    
    rc = -1;
    for (ifa = ifs; ifa != NULL; ifa = ifa->ifa_next) {
        if (!strcmp(ifa->ifa_name, ip_if)) {
            if (ifa->ifa_addr->sa_family == AF_INET) {
                ipv4->sin_addr.s_addr = ((struct sockaddr_in*)(ifa->ifa_addr))->sin_addr.s_addr;
                machine->ipv4 = malloc(INET_ADDRSTRLEN);
                strncpy(machine->ipv4, inet_ntoa(ipv4->sin_addr), INET_ADDRSTRLEN);
                rc = 0;
            }
        }
    }
    
    return (rc);
}

static int
retrieve_ipv6(machine *machine)
{
    int rc;
    struct ifaddrs *ifa;
    struct ifaddrs *ifs;
    struct sockaddr_in6 *ipv6;
    char *ip_if;

    ipv6 = malloc(sizeof(struct sockaddr_in6));
    rc = getifaddrs(&ifs);
    if (rc != 0) {
        return (rc);
    }
    ip_if = strdup("en0");
    
    rc = -1;
    for (ifa = ifs; ifa != NULL; ifa = ifa->ifa_next) {
        if (!strcmp(ifa->ifa_name, ip_if)) {
            if (ifa->ifa_addr->sa_family == AF_INET6) {
                ipv6->sin6_addr = ((struct sockaddr_in6*)(ifa->ifa_addr))->sin6_addr;
                machine->ipv6 = malloc(INET6_ADDRSTRLEN);
                inet_ntop(AF_INET6, (const void *)&ipv6->sin6_addr, machine->ipv6, INET6_ADDRSTRLEN);
                rc = 0;
            }
        }
    }
    
    return (rc);
}

static int
retrieve_macaddress(machine *machine)
{
    int rc;
    struct ifaddrs *ifa;
    struct ifaddrs *ifs;
    unsigned char *macaddr;
    char *ip_if;

    rc = getifaddrs(&ifs);
    if (rc != 0) {
        return (rc);
    }
    ip_if = strdup("en0");
    
    rc = -1;
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
                rc = 0;
            }
        }
    }
    
    return (rc);
}

static int
retrieve_cpu_arch(machine *machine)
{
    int rc;
    size_t len;
    
    rc = sysctlbyname("hw.machine", NULL, &len, NULL, 0);
    if (rc != 0) {
        return (rc);
    }
    machine->cpu.arch = malloc(len);
    rc = sysctlbyname("hw.machine", machine->cpu.arch, &len, NULL, 0);
    
    return (rc);
}

static int
retrieve_cpu_model(machine *machine)
{
    int rc;
    size_t len;
    
    rc = sysctlbyname("machdep.cpu.brand_string", NULL, &len, NULL, 0);
    if (rc != 0) {
        return (rc);
    }
    machine->cpu.model = malloc(len);
    rc = sysctlbyname("machdep.cpu.brand_string", machine->cpu.model, &len, NULL, 0);
    
    return (rc);
}

static int
retrieve_ncpus(machine *machine)
{
    int rc;
    size_t len;
    
    len = sizeof(machine->cpu.ncpus);
    rc = sysctlbyname("hw.ncpu", &(machine->cpu.ncpus), &len, NULL, 0);
    
    return (rc);
}

static int
retrieve_physmem(machine *machine)
{
    int rc;
    size_t len;
    
    len = sizeof(machine->memory.physmem);
    rc = sysctlbyname("hw.memsize", &(machine->memory.physmem), &len, NULL, 0);
    
    return (rc);
}

static int
retrieve_swap(machine *machine)
{
    int rc;
    size_t len;
    struct xsw_usage xsu;
    
    rc = sysctlbyname("vm.swapusage", NULL, &len, NULL, 0);
    if (rc != 0) {
        return (rc);
    }
    rc = sysctlbyname("vm.swapusage", &xsu, &len, NULL, 0);
    machine->memory.swaptotal = xsu.xsu_total;
    
    return (rc);
}

static int
retrieve_os_name(machine *machine)
{
    int rc;
    size_t len;
    
    rc = sysctlbyname("kern.ostype", NULL, &len, NULL, 0);
    if (rc != 0) {
        return (rc);
    }
    machine->os.name = malloc(len);
    rc = sysctlbyname("kern.ostype", machine->os.name, &len, NULL, 0);
    
    return (rc);
}

static int
retrieve_os_release(machine *machine)
{
    int rc;
    size_t len;
    
    rc = sysctlbyname("kern.osrelease", NULL, &len, NULL, 0);
    if (rc != 0) {
        return (rc);
    }
    machine->os.release = malloc(len);
    rc = sysctlbyname("kern.osrelease", machine->os.release, &len, NULL, 0);
    
    return (rc);
}

static int
retrieve_os_version(machine *machine)
{
    int rc;
    size_t len;
    
    rc = sysctlbyname("kern.version", NULL, &len, NULL, 0);
    if (rc != 0) {
        return (rc);
    }
    machine->os.version = malloc(len);
    rc = sysctlbyname("kern.version", machine->os.version, &len, NULL, 0);
    
    return (rc);
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
    
    /* memory.swap */
    nerrors += retrieve_swap(machine);
    
    /* os.name */
    nerrors += retrieve_os_name(machine);
    
    /* os.release */
    nerrors += retrieve_os_release(machine);
    
    /* os.version */
    nerrors += retrieve_os_version(machine);
    
    return (nerrors);
}
