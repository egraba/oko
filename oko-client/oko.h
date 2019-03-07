#ifndef oko_h
#define oko_h

typedef struct {
    char *type;
    char *name;
    char *cpuarch;
    char *cpuname;
    int ncpu;
    char *hostname;
    char *ostype;
    char *osversion;
    int memsize;
} device;

typedef struct {
    int usage;
} cpu;

typedef struct {
    int usage;
} memory;

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

int device_info(device *device);
int cpu_usage(cpu *cpu);
int mem_usage(memory *memory);
int swap_usage(swap *swap);
int io_usage(io *io);
int network_usage(network *network);

#endif /* oko_h */
