#ifndef oko_h
#define oko_h

#include <stdint.h>

typedef struct {
	struct {
		char *serialnumber;
		char *type;
		char *model;
	} hardware;
	struct {
		char *hostname;
		char *ip;
		char *macaddress;
	} network;
	struct {
		char *arch;
		char *model;
		int32_t ncpus;
	} cpu;
	struct {
		int64_t physmem;
		u_int64_t swaptotal;
	} memory;
	struct {
		char *name;
		char *release;
	} os;
} machine;

typedef struct {
	struct {
		float user;
		float system;
		float idle;
		float nice;
	} cpu;
	struct {
		int64_t memused;
		int64_t memfree;
		u_int64_t swapused;
		u_int64_t swapfree;
	} memory;
	struct {
		int64_t readin;
		int64_t writeout;
		int64_t readinsec;
		int64_t writeoutsec;
		int64_t dataread;
		int64_t datawritten;
		int64_t datareadsec;
		int64_t datawrittensec;
	} io;
	struct {
		int64_t pckin;
		int64_t pckout;
		int64_t pckinsec;
		int64_t pckoutsec;
		int64_t datarec;
		int64_t datasent;
		int64_t datarecsec;
		int64_t datasentsec;
	} network;
} usage;

int retrieve_hardware_serialnumber(machine *machine);
int retrieve_hardware_type(machine *machine);
int retrieve_hardware_model(machine *machine);
int retrieve_network_hostname(machine *machine);
int retrieve_network_ip(machine *machine);
int retrieve_network_macaddress(machine *machine);
int retrieve_cpu_arch(machine *machine);
int retrieve_cpu_model(machine *machine);
int retrieve_cpu_ncpus(machine *machine);
int retrieve_memory_physmem(machine *machine);
int retrieve_memory_swaptotal(machine *machine);
int retrieve_os_name(machine *machine);
int retrieve_os_release(machine *machine);

int collect_machine_info(machine *machine);

int retrieve_cpu_usage(usage *usage);
int retrieve_memory_usage(usage *usage);
int retrieve_memory_swap_usage(usage *usage);
int retrieve_network_usage(usage *usage);

int collect_machine_usage(usage *usage);

#endif /* oko_h */
