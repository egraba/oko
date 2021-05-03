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

int collect_machine_info(machine *machine);
int collect_machine_usage(usage *usage);

#endif /* oko_h */
