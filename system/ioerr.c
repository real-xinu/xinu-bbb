/* ioerr.c - ioerr */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ioerr  -  return an error status (used for "error" entries in devtab)
 *------------------------------------------------------------------------
 */
devcall	ioerr(void)
{
	return SYSERR;
}
