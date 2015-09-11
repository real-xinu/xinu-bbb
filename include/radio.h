/* radio.h */

#define	RAD_ADDR_LEN	8	/* Hardware address length		*/

struct	radcblk {

	did32	spidev;		/* Index in device switch table		*/
	void	*csr;		/* Address of control and status regs.	*/

	byte	devAddress[RAD_ADDR_LEN];/* Mac address			*/
	byte	addrLen;	/* Hardware address length		*/

	uint16	mtu;		/* Maximum Transmission Unit		*/
};

extern	struct	radcblk	radtab[];	/* Array of Control blocks		*/
