#include "Version.h"
#ifndef MYFREETICKETS
#warning("WARNING - USING THE SOLUTION freetickets.c NOT MY CODE")
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
#include "freetickets.h"

/*
 * freetickets
 *      tears down the database freeing all allocated memory
 *      by deleting all tickets and vehicles structs in the database
 *      count the number of tickets and vehicles freed in
 *      the database 
 *      prints the counts before returning
 */
void
freetickets(void)
{
    unsigned long tcnt = 0UL;		// tickets freed count
    unsigned long vcnt = 0UL;		// vehicles freed count
	
	for (int i = 0; i < (int)tabsz; i++) {
		struct vehicle *vehicle = *(htable+i);	// vehicle in the chain

		while (vehicle != NULL) {
			struct ticket *ticket = vehicle->head;	// ticket in the chain
			while (ticket != NULL) {
				struct ticket *ticketToFree = ticket;	// temporary var to store ticket to free
				ticket = ticket->next;
				free(ticketToFree);
				tcnt += 1;
				vehicle->cnt_ticket -= 1;
			}	
			struct vehicle *vehicleToFree = vehicle;	// temporary var to store veh to free
			vehicle = vehicle->next;
			free(vehicleToFree->state);
			free(vehicleToFree->plate);
			free(vehicleToFree);
			vcnt += 1;
		}
		
		*(htable+i) = NULL;
	}

    printf("Total vehicles freed: %lu\n", vcnt);
    printf("Total tickets freed: %lu\n", tcnt);
    return;
}
#endif
