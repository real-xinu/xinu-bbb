/* meminit.c - meminit */
#include <xinu.h>

void	*minheap;
void	*maxheap;

void	meminit() {

	struct	memblk *memptr;

	minheap = (void *)&end;
	maxheap = (void *)0x84000000;

	memlist.mnext = (struct memblk *)minheap;
	memptr = memlist.mnext;

	memptr->mnext = (struct memblk *)NULL;
	memptr->mlength = (uint32)maxheap - (uint32)minheap;
}
