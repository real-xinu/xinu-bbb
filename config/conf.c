/* conf.c (GENERATED FILE; DO NOT EDIT) */

#include <xinu.h>


extern	devcall	ioerr(void);
extern	devcall	ionull(void);

/* Device independent I/O switch */

struct	dentry	devtab[NDEVS] =
{
/**
 * Format of entries is:
 * dev-number, minor-number, dev-name,
 * init, open, close,
 * read, write, seek,
 * getc, putc, control,
 * dev-csr-address, intr-handler, irq
 */

/* CONSOLE is tty */
	{ 0, 0, "CONSOLE",
	  (void *)ttyinit, (void *)ionull, (void *)ionull,
	  (void *)ttyread, (void *)ttywrite, (void *)ioerr,
	  (void *)ttygetc, (void *)ttyputc, (void *)ttycontrol,
	  (void *)0x44e09000, (void *)ttyhandler, 72 },

/* NULLDEV is null */
	{ 1, 0, "NULLDEV",
	  (void *)ionull, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)ioerr,
	  (void *)0x0, (void *)ioerr, 0 },

/* ETHER0 is eth */
	{ 2, 0, "ETHER0",
	  (void *)ethinit, (void *)ioerr, (void *)ioerr,
	  (void *)ethread, (void *)ethwrite, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ethcontrol,
	  (void *)0x0, (void *)ethhandler, 0 },

/* NAMESPACE is nam */
	{ 3, 0, "NAMESPACE",
	  (void *)naminit, (void *)namopen, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ioerr, 0 },

/* RDISK is rds */
	{ 4, 0, "RDISK",
	  (void *)rdsinit, (void *)rdsopen, (void *)ioerr,
	  (void *)rdsread, (void *)rdswrite, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)rdscontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* RAM0 is ram */
	{ 5, 0, "RAM0",
	  (void *)raminit, (void *)ramopen, (void *)ramclose,
	  (void *)ramread, (void *)ramwrite, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILESYS is rfs */
	{ 6, 0, "RFILESYS",
	  (void *)rfsinit, (void *)rfsopen, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)rfscontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE0 is rfl */
	{ 7, 0, "RFILE0",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE1 is rfl */
	{ 8, 1, "RFILE1",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE2 is rfl */
	{ 9, 2, "RFILE2",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE3 is rfl */
	{ 10, 3, "RFILE3",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE4 is rfl */
	{ 11, 4, "RFILE4",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE5 is rfl */
	{ 12, 5, "RFILE5",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE6 is rfl */
	{ 13, 6, "RFILE6",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE7 is rfl */
	{ 14, 7, "RFILE7",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE8 is rfl */
	{ 15, 8, "RFILE8",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE9 is rfl */
	{ 16, 9, "RFILE9",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILESYS is lfs */
	{ 17, 0, "LFILESYS",
	  (void *)lfsinit, (void *)lfsopen, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE0 is lfl */
	{ 18, 0, "LFILE0",
	  (void *)lflinit, (void *)ioerr, (void *)lflclose,
	  (void *)lflread, (void *)lflwrite, (void *)lflseek,
	  (void *)lflgetc, (void *)lflputc, (void *)lflcontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE1 is lfl */
	{ 19, 1, "LFILE1",
	  (void *)lflinit, (void *)ioerr, (void *)lflclose,
	  (void *)lflread, (void *)lflwrite, (void *)lflseek,
	  (void *)lflgetc, (void *)lflputc, (void *)lflcontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE2 is lfl */
	{ 20, 2, "LFILE2",
	  (void *)lflinit, (void *)ioerr, (void *)lflclose,
	  (void *)lflread, (void *)lflwrite, (void *)lflseek,
	  (void *)lflgetc, (void *)lflputc, (void *)lflcontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE3 is lfl */
	{ 21, 3, "LFILE3",
	  (void *)lflinit, (void *)ioerr, (void *)lflclose,
	  (void *)lflread, (void *)lflwrite, (void *)lflseek,
	  (void *)lflgetc, (void *)lflputc, (void *)lflcontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE4 is lfl */
	{ 22, 4, "LFILE4",
	  (void *)lflinit, (void *)ioerr, (void *)lflclose,
	  (void *)lflread, (void *)lflwrite, (void *)lflseek,
	  (void *)lflgetc, (void *)lflputc, (void *)lflcontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE5 is lfl */
	{ 23, 5, "LFILE5",
	  (void *)lflinit, (void *)ioerr, (void *)lflclose,
	  (void *)lflread, (void *)lflwrite, (void *)lflseek,
	  (void *)lflgetc, (void *)lflputc, (void *)lflcontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* GPIO0 is gpio */
	{ 24, 0, "GPIO0",
	  (void *)gpioinit, (void *)ionull, (void *)ionull,
	  (void *)gpioread, (void *)gpiowrite, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)gpiocontrol,
	  (void *)0x44e07000, (void *)gpiohandler, 96 },

/* GPIO1 is gpio */
	{ 25, 1, "GPIO1",
	  (void *)gpioinit, (void *)ionull, (void *)ionull,
	  (void *)gpioread, (void *)gpiowrite, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)gpiocontrol,
	  (void *)0x4804c000, (void *)gpiohandler, 98 },

/* GPIO2 is gpio */
	{ 26, 2, "GPIO2",
	  (void *)gpioinit, (void *)ionull, (void *)ionull,
	  (void *)gpioread, (void *)gpiowrite, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)gpiocontrol,
	  (void *)0x481ac000, (void *)gpiohandler, 32 },

/* GPIO3 is gpio */
	{ 27, 3, "GPIO3",
	  (void *)gpioinit, (void *)ionull, (void *)ionull,
	  (void *)gpioread, (void *)gpiowrite, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)gpiocontrol,
	  (void *)0x481ae000, (void *)gpiohandler, 62 },

/* SPI0 is spi */
	{ 28, 0, "SPI0",
	  (void *)spiinit, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)spicontrol,
	  (void *)0x48030000, (void *)ionull, 0 },

/* SPI1 is spi */
	{ 29, 1, "SPI1",
	  (void *)spiinit, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)spicontrol,
	  (void *)0x481a0000, (void *)ionull, 0 },

/* PIPE is pipem */
	{ 30, 0, "PIPE",
	  (void *)ionull, (void *)pipe_open, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* PIPE0 is pip */
	{ 31, 0, "PIPE0",
	  (void *)pipe_init, (void *)ioerr, (void *)pipe_close,
	  (void *)pipe_read, (void *)pipe_write, (void *)ioerr,
	  (void *)pipe_getc, (void *)pipe_putc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* PIPE1 is pip */
	{ 32, 1, "PIPE1",
	  (void *)pipe_init, (void *)ioerr, (void *)pipe_close,
	  (void *)pipe_read, (void *)pipe_write, (void *)ioerr,
	  (void *)pipe_getc, (void *)pipe_putc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* PIPE2 is pip */
	{ 33, 2, "PIPE2",
	  (void *)pipe_init, (void *)ioerr, (void *)pipe_close,
	  (void *)pipe_read, (void *)pipe_write, (void *)ioerr,
	  (void *)pipe_getc, (void *)pipe_putc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* PIPE3 is pip */
	{ 34, 3, "PIPE3",
	  (void *)pipe_init, (void *)ioerr, (void *)pipe_close,
	  (void *)pipe_read, (void *)pipe_write, (void *)ioerr,
	  (void *)pipe_getc, (void *)pipe_putc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* PIPE4 is pip */
	{ 35, 4, "PIPE4",
	  (void *)pipe_init, (void *)ioerr, (void *)pipe_close,
	  (void *)pipe_read, (void *)pipe_write, (void *)ioerr,
	  (void *)pipe_getc, (void *)pipe_putc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* PIPE5 is pip */
	{ 36, 5, "PIPE5",
	  (void *)pipe_init, (void *)ioerr, (void *)pipe_close,
	  (void *)pipe_read, (void *)pipe_write, (void *)ioerr,
	  (void *)pipe_getc, (void *)pipe_putc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* PIPE6 is pip */
	{ 37, 6, "PIPE6",
	  (void *)pipe_init, (void *)ioerr, (void *)pipe_close,
	  (void *)pipe_read, (void *)pipe_write, (void *)ioerr,
	  (void *)pipe_getc, (void *)pipe_putc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* PIPE7 is pip */
	{ 38, 7, "PIPE7",
	  (void *)pipe_init, (void *)ioerr, (void *)pipe_close,
	  (void *)pipe_read, (void *)pipe_write, (void *)ioerr,
	  (void *)pipe_getc, (void *)pipe_putc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* PIPE8 is pip */
	{ 39, 8, "PIPE8",
	  (void *)pipe_init, (void *)ioerr, (void *)pipe_close,
	  (void *)pipe_read, (void *)pipe_write, (void *)ioerr,
	  (void *)pipe_getc, (void *)pipe_putc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* PIPE9 is pip */
	{ 40, 9, "PIPE9",
	  (void *)pipe_init, (void *)ioerr, (void *)pipe_close,
	  (void *)pipe_read, (void *)pipe_write, (void *)ioerr,
	  (void *)pipe_getc, (void *)pipe_putc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 }
};
