#include <pthread.h>

#include "utils.h"

int
launch_thread(void *routine, void *data)
{
    pthread_attr_t attr;
    pthread_t thread_id;
    
    if (pthread_attr_init(&attr)) {
        return 1;
    }
    
    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) {
        return 1;
    }
 
    if (pthread_create(&thread_id, &attr, routine, data)) {
        return 1;
    }
 
    if (pthread_attr_destroy(&attr)) {
        return 1;
    }

    return 0;
}
