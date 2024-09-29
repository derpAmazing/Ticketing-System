#include "Version.h"
#ifndef MYDUMPDB
#warning("WARNING - USING THE SOLUTION dumpdb.c NOT MY CODE")
#else
#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "parking.h"
#include "subs.h"
#include "dumpdb.h"

/*
 * dumpchain
 *
 * dump the linked list chain on a single hash chain
 * print each vehicle on the chain using printvehicle()
 *
 * Arguments:
 *
 * index:
 *          hash table chain index number
 * cnt:
 *          a pointer to an output variable that contains the count
 *          of the total number of tickets on the hash chain
 *          set *cnt = 0UL if no tickets are found
 *
 * return:
 *          number of vehicles found on the hash chain
 *          total number of tickets in chain returned in output variable *cnt
 */
unsigned long
dumpchain(uint32_t index, unsigned long *cnt)
{
	struct vehicle *chain = *(htable+index);	// vehicle in chain
	*cnt = 0;
	unsigned long vehcnt = 0;
	if (chain != NULL) {
		printf("Chain %u: \n", index);
	}
	while (chain != NULL) {
		struct ticket *ticket = chain->head;	// ticket in vehicle's chain
		while (ticket != NULL) {
			*cnt += 1;
			ticket = ticket->next;
		}
		printvehicle(chain);
		vehcnt += 1;
		chain = chain->next;
	} 
	
	return vehcnt;
}

/*
 * dumpdb
 *
 * dump the entire database one hash chain at a time from chain 0
 * in the hash table to the last entry in the table
 *
 * call dumpchain() to print each hash chain.
 *
 * Prints the total number of vehicles and tickets in the database
 *
 */
void
dumpdb(void)
{
    unsigned long vcnt = 0UL; // total vehicle count
    unsigned long tcnt = 0UL; // total ticket count

	for (int i = 0; i < (int)tabsz; i++) {
		unsigned long ticketcnt = 0; // ticket count in one chain
		vcnt += dumpchain(i, &ticketcnt);
		tcnt += ticketcnt;
	}
 
    printf("Total vehicles in database: %lu\n", vcnt);
    printf("Total tickets in database: %lu\n", tcnt);
    return;
}
#endif
