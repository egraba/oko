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

int collect_info(machine *machine);

#endif /* oko_h */
