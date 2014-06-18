/* clock.h */

/* Intel 8254-2 clock chip constants */

#define	CLOCKBASE	0x40		/* I/O base port of clock chip	*/
#define	CLOCK0		CLOCKBASE
#define	CLKCNTL		(CLOCKBASE+3)	/* chip CSW I/O port		*/


#define CLKTICKS_PER_SEC  1000	/* clock timer resolution		*/

extern	uint32	clktime;	/* current time in secs since boot	*/

extern	qid16	sleepq;		/* queue for sleeping processes		*/
extern	int32	slnonempty;	/* nonzero if sleepq is nonempty	*/
extern	int32	*sltop;		/* ptr to key in first item on sleepq	*/
extern	uint32	preempt;	/* preemption counter			*/

struct am335x_timer1ms {
	uint32	tidr;
	uint32	res1[3];
	uint32	tiocp_cfg;
	uint32	tistat;
	uint32	tisr;
	uint32	tier;
	uint32	twer;
	uint32	tclr;
	uint32	tcrr;
	uint32	tldr;
	uint32	ttgr;
	uint32	twps;
	uint32	tmar;
	uint32	tcar1;
	uint32	tsicr;
	uint32	tcar2;
	uint32	tpir;
	uint32	tnir;
	uint32	tcvr;
	uint32	tocr;
	uint32	towr;
};

#define AM335X_TIMER1MS_TIOCP_CFG_SOFTRESET	0x00000002
#define AM335X_TIMER1MS_TISTAT_RESETDONE	0x00000001

#define AM335X_TIMER1MS_TISR_MAT_IT_FLAG	0x00000001
#define AM335X_TIMER1MS_TISR_OVF_IT_FLAG	0x00000002
#define AM335X_TIMER1MS_TISR_TCAR_IT_FLAG	0x00000004

#define AM335X_TIMER1MS_TIER_MAT_IT_ENA		0x00000001
#define AM335X_TIMER1MS_TIER_OVF_IT_ENA		0x00000002
#define AM335X_TIMER1MS_TIER_TCAR_IT_ENA	0x00000004

#define AM335X_TIMER1MS_TCLR_ST			0x00000001
#define AM335X_TIMER1MS_TCLR_AR			0x00000002

#define AM335X_TIMER1MS_CLKCTRL_ADDR		0x44E004C4
#define AM335X_TIMER1MS_CLKCTRL_EN		0x00000002
