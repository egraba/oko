#ifndef oko_h
#define oko_h

#include <stdint.h>

typedef struct {
	int32_t id;
	char *type;
	char *model;
	char *hostname;
	char *ip;
	char *macaddress;
	struct {
		char *arch;
		char *model;
		int32_t ncpus;
	} cpu;
	struct {
		int64_t physmem;
	} memory;
	struct {
		char *name;
		char *release;
		char *version;
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
		int64_t used;
		int64_t free;
		u_int64_t swaptotal;
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


int retrieve_type(machine *machine);
int retrieve_model(machine *machine);
int retrieve_hostname(machine *machine);
int retrieve_ip(machine *machine);
int retrieve_macadress(machine *machine);
int retrieve_cpu_arch(machine *machine);
int retrive_cpu_model(machine *machine);
int retrieve_ncpus(machine *machine);
int retrieve_physmem(machine *machine);
int retrieve_os_name(machine *machine);
int retrieve_os_release(machine *machine);
int retrieve_os_version(machine *machine);

int collect_machine_info(machine *machine);

int retrieve_cpu_usage(usage *usage);
int retrieve_memory_usage(usage *usage);
int retrieve_memory_swapusage(usage *usage);

int collect_machine_usage(usage *usage);

#endif /* oko_h */
