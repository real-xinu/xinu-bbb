/* ethInit.c - ethInit */

#include <xinu.h>

struct	ether	ethertab[Neth];		/* Ethernet control blocks 	*/

/*------------------------------------------------------------------------
 * ethInit - Initialize Ethernet device structures
 *------------------------------------------------------------------------
 */
devcall	ethInit (
		struct dentry *devptr
		)
{
	struct	ether 	*ethptr;
	int32	dinfo;			/* device information		*/

	/* Initialize structure pointers */
	ethptr = &ethertab[devptr->dvminor];
	
	memset(ethptr, '\0', sizeof(struct ether));
	ethptr->dev = devptr;
	ethptr->csr = devptr->dvcsr;
	ethptr->state = ETH_STATE_DOWN;
	ethptr->mtu = ETH_MTU;
	ethptr->errors = 0;
	ethptr->addrLen = ETH_ADDR_LEN;
	ethptr->rxHead = ethptr->rxTail = 0;
	ethptr->txHead = ethptr->txTail = 0;
#if 0
	if ((dinfo = find_pci_device(_3COM_3C905C_DEVICE_ID,
					_3COM_VENDOR_ID, 0))
			!= SYSERR) {
		kprintf("Found 3COM 3c905C Ethernet NIC\n");

		ethptr->type = ETH_TYPE_3C905C;
		ethptr->pcidev = dinfo;
		
		/* Initialize function pointers */
		
		ethptr->ethInit = _3c905C_init;
		ethptr->ethOpen = _3c905C_open;
//		ethptr->ethClose = _3c905C_close;
		ethptr->ethRead = _3c905C_read;
		ethptr->ethWrite = _3c905C_write;
		ethptr->ethControl = _3c905C_cntl;
		ethptr->ethInterrupt = _3c905C_intr;
	} else if ((dinfo = find_pci_device(INTEL_82567LM_DEVICE_ID,
					INTEL_VENDOR_ID, 0))
			!= SYSERR) {
		kprintf("Found Intel Hub 10D/82567LM Ethernet NIC\n");

		ethptr->type = ETH_TYPE_E1000E;
		ethptr->pcidev = dinfo;

		/* Initialize function pointers */
		
		ethptr->ethInit = e1000e_init;
		ethptr->ethOpen = e1000e_open;
//		ethptr->ethClose = e1000e_close;
		ethptr->ethRead = e1000e_read;
		ethptr->ethWrite = e1000e_write;
		ethptr->ethControl = e1000e_cntl;
		ethptr->ethInterrupt = e1000e_intr;
	}
	else {
		kprintf("No recognized PCI Ethernet NIC found\n");
		return SYSERR;
	}
#endif
	/* Initialize control block */
		
	ethptr->ethInit(ethptr);

	/* Allocate descriptors and buffers, active the NIC */

	if (ethptr->ethOpen(ethptr) != OK)
		return SYSERR;

	/* Ethernet interface is active from here */

	ethptr->state = ETH_STATE_UP;

	return OK;
}
