//-----------------------------------------------------------------------------------------
#ifndef _PTHREAD_WATCHDOG_H
#define _PTHREAD_WATCHDOG_H
//-----------------------------------------------------------------------------------------

typedef struct S_PTHREAD_WATCHDOG
{
	int		wdt;						// watchdog timer
	int		timeout;					// timeout (second)
	int		enable;						// is enable or not
	char	descript[30];				// description, normaly use function name
	struct S_PTHREAD_WATCHDOG *next;	// next pointer
} C_PTHREAD_WATCHDOG;

int pthread_watchdog_init(void);
int pthread_watchdog_unreg(C_PTHREAD_WATCHDOG *watchdog);
C_PTHREAD_WATCHDOG *pthread_watchdog_reg(void);

//-----------------------------------------------------------------------------------------
#endif // _PTHREAD_WATCHDOG_H
//-----------------------------------------------------------------------------------------

// End of File
//----------------------------------------------------------------------------------------- 
