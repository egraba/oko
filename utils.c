#include <assert.h>
#include <pthread.h>

#include "utils.h"

int
launch_thread(void *routine, void *data)
{
    pthread_attr_t attr;
    pthread_t thread_id;
    int rc, thread_error;
 
    rc = pthread_attr_init(&attr);
    assert(!rc);
    rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    assert(!rc);
 
    thread_error = pthread_create(&thread_id, &attr, routine, data);
 
    rc = pthread_attr_destroy(&attr);
    assert(!rc);
    
    if (thread_error != 0) return 1;

    return 0;
}
