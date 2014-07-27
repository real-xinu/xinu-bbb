/* delay.h - DELAY */

/*
 * Delay units are in microseconds.
 */
#define	DELAY(n)                                	\
{                                               	\
	register long N = (n * 100);			\
							\
	while(N > 0) {					\
		N--;					\
	}						\
}

/*
 * Delay units are in milli-seconds.
 */
#define	MDELAY(n)					\
{							\
	register long i;				\
							\
	for (i=n;i>0;i--) {				\
		DELAY(1000);				\
	}						\
}
