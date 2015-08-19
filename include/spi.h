/* spi.h - SPI definitions */

struct	spi_csreg {
	volatile uint32	rev;		/* SPI revision			*/
	volatile uint32	res1[67];	/* Reserved address space	*/
	volatile uint32	sysconfig;	/* System configuration register*/
	volatile uint32	sysstatus;	/* System status register	*/
	volatile uint32	irqstatus;	/* IRQ status register		*/
	volatile uint32	irqenable;	/* IRQ Enable register		*/
	volatile uint32 res2;		/* Reserved address space	*/
	volatile uint32	syst;		/* System test register		*/
	volatile uint32 modulctrl;	/* Module control register	*/
	struct {
	 volatile uint32 chconf;	/* Channel conf register	*/
	 volatile uint32 chstat;	/* Channel status register	*/
	 volatile uint32 chtx;		/* Channel xmit register	*/
	 volatile uint32 chrx;		/* Channel receive register	*/
	} ch[3];
	volatile uint32 xferlevel;	/* Transfer level register	*/
	volatile uint32 daftx;		/* FIFO xmit register		*/
	volatile uint32 dafrx;		/* FIFO receive register	*/
};

/* Bit definitions of SPI registers */

#define	SPI_SYSCONFIG_SOFTRESET	0x00000002

#define	SPI_SYSSTATUS_RESETDONE	0x00000001

#define	SPI_MODULCTRL_SINGLE	0x00000001
#define	SPI_MODULCTRL_PIN34	0x00000002
#define	SPI_MODULCTRL_MS	0x00000004
