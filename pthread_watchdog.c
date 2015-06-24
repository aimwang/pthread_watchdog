#include "pthread_watchdog.h"

struct C_PTHREAD_WATCHDOG	*pthread_watchdog_head = NULL;
int	watchdog_inited = 0;
int watchdog_locked = 0;

/***********************************************************************
 * 
 * register
 * 
 * ********************************************************************/
C_PTHREAD_WATCHDOG *pthread_watchdog_reg(void)
{
	C_PTHREAD_WATCHDOG *pthread_watchdog = pthread_watchdog_head;
	C_PTHREAD_WATCHDOG *new_watchdog;

	if (0 == watchdog_inited)
		return 0;

	while(watchdog_locked)
	{
		usleep(1000);
	}
	watchdog_locked = -1;
	new_watchdog = (C_PTHREAD_WATCHDOG*) malloc(sizeof(C_PTHREAD_WATCHDOG));
	if (new_watchdog)
	{
		memset(new_watchdog, 0, sizeof(C_PTHREAD_WATCHDOG));
		if(pthread_watchdog)
		{
			while(pthread_watchdog->next)
			{
				pthread_watchdog = pthread_watchdog->next;
			}
			pthread_watchdog->next = new_watchdog;
		}
		else
		{
			pthread_watchdog_head = new_watchdog;
		}
	}
	watchdog_locked = 0;
	
	return new_watchdog;
}

/***********************************************************************
 * 
 * unregister
 * 
 * ********************************************************************/

int pthread_watchdog_unreg(C_PTHREAD_WATCHDOG *watchdog)
{
	C_PTHREAD_WATCHDOG *pthread_watchdog;
	
	if (0 == watchdog_inited)
		return 0;
		
	while(watchdog_locked)
	{
		usleep(1000);
	}
	watchdog_locked = -1;
	if (watchdog)
	{
		if(watchdog == pthread_watchdog_head)
		{
			pthread_watchdog_head = watchdog->next;
		}
		else
		{
			pthread_watchdog = pthread_watchdog_head;
			while(watchdog != pthread_watchdog->next)
			{
				pthread_watchdog = pthread_watchdog->next;
			}
			pthread_watchdog->next = watchdog->next;
		}
		free(watchdog);
	}
	watchdog_locked = 0;
}

/***********************************************************************
 * 
 * Main function
 * 
 * ********************************************************************/
void *pthread_watchdog_main(void)
{
	C_PTHREAD_WATCHDOG *pthread_watchdog;
	
	while(1)
	{
		if (pthread_watchdog_head && (0 == watchdog_locked))
		{
			watchdog_locked = -1;
			pthread_watchdog = pthread_watchdog_head;
			while(pthread_watchdog)
			{
				if (pthread_watchdog->enable)
				{
					pthread_watchdog->wdt++;
					//printf("[WDT], %s, %d, %d\n", pthread_watchdog->descript, pthread_watchdog->wdt, pthread_watchdog->timeout);
					if(pthread_watchdog->wdt > pthread_watchdog->timeout)
					{
						printf("[PW] %s timeout(%d)\n", pthread_watchdog->descript, pthread_watchdog->timeout);
						//pthread_watchdog_unreg(pthread_watchdog);
						//kill(getpid(),SIGINT);	//kill by my self
					}
				}
				if (watchdog_locked)
				{
					watchdog_locked = 0;
					// abort this cycle
					break;
				}
				pthread_watchdog = pthread_watchdog->next;
			}
			watchdog_locked = 0;
		}
		usleep(1000000);
	}
}

/***********************************************************************
 * 
 * init
 * 
 * ********************************************************************/
int pthread_watchdog_init(void)
{
	pthread_t pthread_watchdog_Pid;
	int ret;

	if (watchdog_inited)
		return 0;

	ret = pthread_create(&pthread_watchdog_Pid, 0, (void *)pthread_watchdog_main, (void *)(NULL));
	
	if (ret)
	{
		printf("create watchdog thread fail (%d)\n", ret);
	}
	else
	{
		printf("CARBOX watchdog init sucessful\n");
		watchdog_inited = -1;
	}
	return ret;
}

