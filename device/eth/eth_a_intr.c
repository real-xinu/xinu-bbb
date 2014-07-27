/* eth_a_intr.c - eth_a_intr */

#include <xinu.h>

/*------------------------------------------------------------------------
 * eth_a_intr - Ethernet Interrupt Handler
 *------------------------------------------------------------------------
 */
interrupt	eth_a_intr (
				uint32	xnum
				)
{
	struct	eth_a_csreg *csrptr;
	struct	eth_a_tx_desc *tdescptr;
	struct	eth_a_rx_desc *rdescptr;
	struct	ether *ethptr = &ethertab[0];

	csrptr = (struct eth_a_csreg *)ethptr->csr;

	if(xnum == ETH_AM335X_TXINT) {

		tdescptr = (struct eth_a_tx_desc *)ethptr->txRing +
							ethptr->txHead;

		sched_cntl(DEFER_START);

		while(semcount(ethptr->osem) < (int32)ethptr->txRingSize) {

			if(tdescptr->stat & ETH_AM335X_TDS_OWN) {
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
				tdescptr = (struct eth_a_tx_desc *)
							ethptr->txRing;
			}

			signal(ethptr->osem);
		}

		csrptr->cpdma->eoi_vector = 0x2;

		sched_cntl(DEFER_STOP);
	}
	else if(xnum == ETH_AM335X_RXINT) {

		rdescptr = (struct eth_a_rx_desc *)ethptr->rxRing +
							ethptr->rxTail;

		sched_cntl(DEFER_START);

		while(semcount(ethptr->isem) < (int32)ethptr->rxRingSize) {

			if(rdescptr->stat & ETH_AM335X_RDS_OWN) {
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
				rdescptr = (struct eth_a_rx_desc *)
							ethptr->rxRing;
			}
			//kprintf("signaling semaphore %d\n", semcount(ethptr->isem));
			signal(ethptr->isem);
		}

		csrptr->cpdma->eoi_vector = 0x1;

		sched_cntl(DEFER_STOP);
	}
}
