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
#include <Availability.h>

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>

#include "oko.h"

/*
 * The port name changes depending on the version.
 */
#define MACH_PORT kIOMainPortDefault
#ifdef OSX_BEFORE_MONTEREY
#undef MACH_PORT
#define MACH_PORT kIOMasterPortDefault
#endif

int
retrieve_hardware_serialnumber(machine *machine)
{
	io_service_t platform_expert;
	mach_port_t mp;

	mp = MACH_PORT;
	platform_expert = IOServiceGetMatchingService(mp, IOServiceMatching("IOPlatformExpertDevice"));

	if (platform_expert) {
		CFTypeRef serialNumberAsCFString = 
		IORegistryEntryCreateCFProperty(platform_expert,
			CFSTR(kIOPlatformSerialNumberKey),
			kCFAllocatorDefault, 0);
		
		if (serialNumberAsCFString) {
			machine->hardware.serialnumber = (char *) CFStringGetCStringPtr(serialNumberAsCFString,
				CFStringGetSystemEncoding());
		}
		else {
			return 1;
		}

		IOObjectRelease(platform_expert);
	}
	else {
		return 1;
	}

	return 0;
}

int
retrieve_hardware_type(machine *machine)
{
	size_t len;
    
	if (sysctlbyname("hw.targettype", NULL, &len, NULL, 0)) {
		return 1;
	}
	machine->hardware.type = malloc(len);
	if (sysctlbyname("hw.targettype", machine->hardware.type, &len, NULL, 0)) {
		return 1;
	}
    
	return 0;
}

int
retrieve_hardware_model(machine *machine)
{
	size_t len;
    
	if (sysctlbyname("hw.model", NULL, &len, NULL, 0)) {
		return 1;
	}
	machine->hardware.model = malloc(len);
	if (sysctlbyname("hw.model", machine->hardware.model, &len, NULL, 0)) {
		return 1;
	}
    
	return 0;
}

int
retrieve_network_hostname(machine *machine)
{
	size_t len;
    
	if (sysctlbyname("kern.hostname", NULL, &len, NULL, 0)) {
		return 1;
	}
	machine->network.hostname = malloc(len);
	if (sysctlbyname("kern.hostname", machine->network.hostname, &len, NULL, 0)) {
		return 1;
	}
    
	return 0;
}

int
retrieve_network_ip(machine *machine)
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
retrieve_network_macaddress(machine *machine)
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
retrieve_cpu_ncpus(machine *machine)
{
	size_t len;
    
	len = sizeof(machine->cpu.ncpus);
	if (sysctlbyname("hw.ncpu", &(machine->cpu.ncpus), &len, NULL, 0)) {
		return 1;
	}
    
	return 0;
}

int
retrieve_memory_physmem(machine *machine)
{
	size_t len;
    
	len = sizeof(machine->memory.physmem);
	if (sysctlbyname("hw.memsize", &(machine->memory.physmem), &len, NULL, 0)) {
		return 1;
	}
    
	return 0;
}

int
retrieve_memory_swaptotal(machine *machine)
{
	size_t len;
	struct xsw_usage xsu;
    
	if (sysctlbyname("vm.swapusage", NULL, &len, NULL, 0)) {
		return 1;
	}
	if (sysctlbyname("vm.swapusage", &xsu, &len, NULL, 0)) {
		return 1;
	}
	machine->memory.swaptotal = xsu.xsu_total;
    
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
collect_machine_info(machine *machine)
{
	int nerrors = 0;
    
	nerrors += retrieve_hardware_serialnumber(machine);
    nerrors += retrieve_hardware_type(machine);
    nerrors += retrieve_hardware_model(machine);
    
	nerrors += retrieve_network_hostname(machine);
    nerrors += retrieve_network_ip(machine);
    nerrors += retrieve_network_macaddress(machine);
    
	nerrors += retrieve_cpu_arch(machine);
    nerrors += retrieve_cpu_model(machine);
    nerrors += retrieve_cpu_ncpus(machine);
    
	nerrors += retrieve_memory_physmem(machine);
    
	nerrors += retrieve_os_name(machine);
    nerrors += retrieve_os_release(machine);
    
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
	int64_t memfree;
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
	usage->memory.memused = pagesize * pages;
    
	len = sizeof(memfree);
	memfree = 0;
	if (sysctlbyname("vm.page_free_count", &memfree, &len, NULL, 0)) {
		return 1;
	}
	usage->memory.memfree = pagesize * memfree;
    
	return 0;
}

int
retrieve_memory_swap_usage(usage *usage)
{
	size_t len;
	struct xsw_usage xsu;
    
	if (sysctlbyname("vm.swapusage", NULL, &len, NULL, 0)) {
		return 1;
	}
	if (sysctlbyname("vm.swapusage", &xsu, &len, NULL, 0)) {
		return 1;
	}
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

	free(buf);

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
	nerrors += retrieve_memory_swap_usage(usage);
    
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
