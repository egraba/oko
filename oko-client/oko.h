#ifndef oko_h
#define oko_h

#include <stdint.h>

typedef struct {
    int32_t id;
    char *type;
    char *model;
    char *hostname;
    char *ipv4;
    char *ipv6;
    char *macaddress;
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
        char *version;
    } os;
} machine;



typedef struct {
    int usage;
} swap;

typedef struct {
    int bytesread;
    int byteswrite;
} io;

typedef struct {
    int pckin;
    int pckout;
} network;

typedef struct {
    int pid;
    char *name;
    int cpuusage;
    int memusage;
    int bytesread;
    int byteswrite;
    int pckin;
    int pckout;
} process;

int collect_info(machine *machine);
/*int cpu_usage(cpu *cpu);
int mem_usage(memory *memory);
int swap_usage(swap *swap);
int io_usage(io *io);
int network_usage(network *network);*/

#endif /* oko_h */
