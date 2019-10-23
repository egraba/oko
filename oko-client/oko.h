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
    struct {
        float usage;
    } cpu;
    struct {
        int64_t memusage;
        u_int64_t swapusage;
    } memory;
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

int collect_info(machine *machine);
int collect_usage(usage *usage);

#endif /* oko_h */
