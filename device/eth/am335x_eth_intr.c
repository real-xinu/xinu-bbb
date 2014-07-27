/* am335x_eth_intr.c - am335x_eth_intr */

#include <xinu.h>

/*------------------------------------------------------------------------
 * am335x_eth_intr - Ethernet Interrupt Handler
 *------------------------------------------------------------------------
 */
interrupt	am335x_eth_intr (
				uint32	xnum
				)
{
	struct	am335x_eth_csreg *csrptr;
	struct	am335x_eth_tx_desc *tdescptr;
	struct	am335x_eth_rx_desc *rdescptr;
	struct	ether *ethptr = &ethertab[0];

	csrptr = (struct am335x_eth_csreg *)ethptr->csr;

	if(xnum == AM335X_ETH_TXINT) {

		tdescptr = (struct am335x_eth_tx_desc *)ethptr->txRing +
							ethptr->txHead;

		sched_cntl(DEFER_START);

		while(semcount(ethptr->osem) < (int32)ethptr->txRingSize) {

			if(tdescptr->stat & AM335X_ETH_TDS_OWN) {
				if(csrptr->stateram->tx_hdp[0] == 0) {
					csrptr->stateram->tx_hdp[0] =
							(uint32)tdescptr;
				}
				break;
			}

			csrptr->stateram->tx_cp[0] = (uint32)tdescptr;

			ethptr->txHead++;
			tdescptr++;
			if(ethptr->txHead >= ethptr->txRingSize) {
				ethptr->txHead = 0;
				tdescptr = (struct am335x_eth_tx_desc *)
							ethptr->txRing;
			}

			signal(ethptr->osem);
		}

		csrptr->cpdma->eoi_vector = 0x2;

		sched_cntl(DEFER_STOP);
	}
	else if(xnum == AM335X_ETH_RXINT) {

		rdescptr = (struct am335x_eth_rx_desc *)ethptr->rxRing +
							ethptr->rxTail;

		sched_cntl(DEFER_START);

		while(semcount(ethptr->isem) < (int32)ethptr->rxRingSize) {

			if(rdescptr->stat & AM335X_ETH_RDS_OWN) {
				if(csrptr->stateram->rx_hdp[0] == 0) {
					csrptr->stateram->rx_hdp[0] =
							(uint32)rdescptr;
				}
				break;
			}

			csrptr->stateram->rx_cp[0] = (uint32)rdescptr;

			ethptr->rxTail++;
			rdescptr++;
			if(ethptr->rxTail >= ethptr->rxRingSize) {
				ethptr->rxTail = 0;
				rdescptr = (struct am335x_eth_rx_desc *)
							ethptr->rxRing;
			}
			//kprintf("signaling semaphore %d\n", semcount(ethptr->isem));
			signal(ethptr->isem);
		}

		csrptr->cpdma->eoi_vector = 0x1;

		sched_cntl(DEFER_STOP);
	}
}
