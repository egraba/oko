#include <arpa/inet.h>
#include <ifaddrs.h>
#include <mach/mach_error.h>
#include <mach/mach_host.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/route.h>
#include <sys/sysctl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>

#include "oko.h"

int
retrieve_serialnumber(machine *machine)
{
	io_service_t platform_expert;

	platform_expert = IOServiceGetMatchingService(kIOMainPortDefault,
		IOServiceMatching("IOPlatformExpertDevice"));

	if (platform_expert) {
		CFTypeRef serialNumberAsCFString = 
		IORegistryEntryCreateCFProperty(platform_expert,
			CFSTR(kIOPlatformSerialNumberKey),
			kCFAllocatorDefault, 0);
		
		if (serialNumberAsCFString) {
			machine->serialnumber = (char *) CFStringGetCStringPtr(serialNumberAsCFString,
				CFStringGetSystemEncoding());
		}
		else {
			machine->serialnumber = strdup("__VM__");
		}

		IOObjectRelease(platform_expert);
	}

	return 0;
}

int
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

int
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

int
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

int
retrieve_ip(machine *machine)
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
				machine->network.ip = malloc(INET_ADDRSTRLEN);
				strncpy(machine->network.ip, inet_ntoa(ipv4->sin_addr), INET_ADDRSTRLEN);
                
				return 0;
			}
		}
	}
    
	return 1;
}

int
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
				macaddr = (unsigned char*) ifa->ifa_addr->sa_data;
				macaddr += 9; /* To get the real MAC address */
				machine->network.macaddress = malloc(17);
				sprintf(machine->network.macaddress,
					"%.2x:%02x:%02x:%02x:%02x:%02x",
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

int
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

int
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

int
retrieve_ncpus(machine *machine)
{
	size_t len;
    
	len = sizeof(machine->cpu.ncpus);
	if (sysctlbyname("hw.ncpu", &(machine->cpu.ncpus), &len, NULL, 0)) {
		return 1;
	}
    
	return 0;
}

int
retrieve_physmem(machine *machine)
{
	size_t len;
    
	len = sizeof(machine->memory.physmem);
	if (sysctlbyname("hw.memsize", &(machine->memory.physmem), &len, NULL, 0)) {
		return 1;
	}
    
	return 0;
}

int
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

int
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

int
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
collect_machine_info(machine *machine)
{
	int nerrors = 0;
    
	/* serialnumber */
	nerrors += retrieve_serialnumber(machine);
    
	/* type */
	nerrors += retrieve_type(machine);
    
	/* model */
	nerrors += retrieve_model(machine);
    
	/* hostname */
	nerrors += retrieve_hostname(machine);
    
	/* network.ip */
	nerrors += retrieve_ip(machine);
    
	/* network.macaddress */
	nerrors += retrieve_macaddress(machine);
    
	/* cpu.arch */
	nerrors += retrieve_cpu_arch(machine);
    
	/* cpu.model */
	nerrors += retrieve_cpu_model(machine);
    
	/* cpu.ncpus */
	nerrors += retrieve_ncpus(machine);
    
	/* memory.physmem */
	nerrors += retrieve_physmem(machine);
    
	/* os.name */
	nerrors += retrieve_os_name(machine);
    
	/* os.release */
	nerrors += retrieve_os_release(machine);
    
	/* os.version */
	nerrors += retrieve_os_version(machine);
    
	return (nerrors);
}

struct host_cpu_load_info cur;
struct host_cpu_load_info prev;

int
retrieve_cpu_usage(usage *usage)
{
	mach_port_t port;
	mach_msg_type_number_t count;
	struct host_cpu_load_info tmp;
	unsigned int i;
	int sum;
    
	count = HOST_CPU_LOAD_INFO_COUNT;
	port = mach_host_self();
	if (host_statistics(port, HOST_CPU_LOAD_INFO, (host_info_t) &cur, &count)) {
		return 1;
	}
    
	sum = 0;
	for (i = 0; i < HOST_CPU_LOAD_INFO_COUNT; i++) {
		tmp.cpu_ticks[i] = cur.cpu_ticks[i];
		cur.cpu_ticks[i] -= prev.cpu_ticks[i];
		prev.cpu_ticks[i] = tmp.cpu_ticks[i];
		sum += cur.cpu_ticks[i];
	}
    
	usage->cpu.user = (float) cur.cpu_ticks[CPU_STATE_USER] / (float) sum * 100;
	usage->cpu.system = (float) cur.cpu_ticks[CPU_STATE_SYSTEM] / (float) sum * 100;
	usage->cpu.idle = (float) cur.cpu_ticks[CPU_STATE_IDLE] / (float) sum * 100;
	usage->cpu.nice = (float) cur.cpu_ticks[CPU_STATE_NICE] / (float) sum * 100;
    
	return 0;
}

int
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

int
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
retrieve_network_usage(usage *usage)
{
	int mib[6];
	size_t len;
	char *buf;
	char *lim;
	char *next;
	struct if_msghdr *ifm;
	
	mib[0]	= CTL_NET;
	mib[1]	= PF_ROUTE;
	mib[2]	= 0;
	mib[3]	= 0;
	mib[4]	= NET_RT_IFLIST2;
	mib[5]	= 0;

	if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
		return 1;
	}

	buf = malloc(len);
	if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
		return 1;
	}

	usage->network.pckin = 0;
	usage->network.pckout = 0;

	lim = buf + len;
	for (next = buf; next < lim; ) {
		ifm = (struct if_msghdr *) next;
		next += ifm->ifm_msglen;

		if (ifm->ifm_type == RTM_IFINFO2) {
			struct if_msghdr2 *if2m = (struct if_msghdr2 *) ifm;
			
			usage->network.pckin += if2m->ifm_data.ifi_ipackets;
			usage->network.pckout += if2m->ifm_data.ifi_opackets;
		}
	}

	return 0;
}

int
collect_machine_usage(usage *usage)
{
	int nerrors = 0;
    
	/* cpu.user */
	/* cpu.system */
	/* cpu.idle */
	/* cpu.nice */
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
	nerrors += retrieve_network_usage(usage);
    
	return (nerrors);
}
