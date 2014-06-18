/* process.h - isbadpid */

/* Maximum number of processes in the system */

#ifndef NPROC
#define	NPROC		8
#endif		

/* Process state constants */

#define	PR_FREE		0	/* process table entry is unused	*/
#define	PR_CURR		1	/* process is currently running		*/
#define	PR_READY	2	/* process is on ready queue		*/
#define	PR_RECV		3	/* process waiting for message		*/
#define	PR_SLEEP	4	/* process is sleeping			*/
#define	PR_SUSP		5	/* process is suspended			*/
#define	PR_WAIT		6	/* process is on semaphore queue	*/
#define	PR_RECTIM	7	/* process is receiving with timeout	*/

/* Miscellaneous process definitions */

#define	PNMLEN		16	/* length of process "name"		*/
#define	NULLPROC	0	/* ID of the null process		*/

/* Process initialization constants */

#define	INITSTK		65536	/* initial process stack size		*/
#define	INITPRIO	20	/* initial process priority		*/
#define	INITRET		userret	/* address to which process returns	*/

/* Reschedule constants for ready */

#define	RESCHED_YES	1	/* call to ready should reschedule	*/
#define	RESCHED_NO	0	/* call to ready should not reschedule	*/

/* Inline code to check process ID (assumes interrupts are disabled)	*/

#define	isbadpid(x)	( ((pid32)(x) < 0) || \
			  ((pid32)(x) >= NPROC) || \
			  (proctab[(x)].prstate == PR_FREE))

/* Number of device descriptors a process can have open */

#define NDESC		5	/* must be odd to make procent 4N bytes	*/

/* Definition of the process table (multiple of 32 bits) */

struct procent {		/* entry in the process table		*/
	uint16	prstate;	/* process state: PR_CURR, etc.		*/
	pri16	prprio;		/* process priority			*/
	char	*prstkptr;	/* saved stack pointer			*/
	char	*prstkbase;	/* base of run time stack		*/
	uint32	prstklen;	/* stack length in bytes		*/
	char	prname[PNMLEN];	/* process name				*/
	uint32	prsem;		/* semaphore on which process waits	*/
	pid32	prparent;	/* id of the creating process		*/
	umsg32	prmsg;		/* message sent to this process		*/
	bool8	prhasmsg;	/* nonzero iff msg is valid		*/
	int16	prdesc[NDESC];	/* device descriptors for process	*/
};

/* Marker for the top of a process stack (used to help detect overflow)	*/
#define	STACKMAGIC	0x0A0AAAA9

extern	struct	procent proctab[];
extern	int32	prcount;	/* currently active processes		*/
extern	pid32	currpid;	/* currently executing process		*/
