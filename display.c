#if 0
#include <pthread.h>

static  int thread_id = -1;
static pthread_t display_thread;
static void *display_thread_entry(void *arg)
{
    int th_id = *((int *)arg);

    while (1)
    {

    }
    return NULL;
}

int display_init(void)
{
  
    int rc = pthread_create(&display_thread, NULL, display_thread_entry, &thread_id);
    return 0;
}
#endif