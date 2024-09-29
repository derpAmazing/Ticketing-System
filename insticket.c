#include "Version.h"
#ifndef MYINSTICKET
#warning("WARNING - USING THE SOLUTION insticket.c NOT MY CODE")
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
#include "insticket.h"
#include "subs.h"

/*
 * insertticket
 *      add a ticket (summons) to the database
 *
 *      YOU CANNOT USE CALLOC(), only malloc() is allowed.
 *
 *      if the vehicle (plate state) is not in the database then
 *      a new struct vehicle entry must be made for it in the hash table.
 *      New vehicle entries are made at the the FRONT of the collison chain
 *
 *      If the vehicle is already in the hash chain, you only need to add 
 *      a new ticket to the ticket chain for this summons.
 *
 *      Tickets are added to the ticket chain linked to the struct vehicle
 *      at the END of the ticket chain.
 *
 *      Also you must update the tot_fine and cnt_tickets in the struct vehicle
 *      to account for the new summons(ticket) just added
 *
 * Arguments
 *
 *  summ:
 *      summons id string to be added
 *      this string needs to be converted to a number to be stored in the
 *      database. This saves space and is faster to compare than a string         
 *      Conversion function is supplied. use example:
 *           unsigned long summid;
 *           if (strtosumid(summ, &summid, argv) != 0)
 *                  error handling
 *           new_ticket->summons = summid;
 *  plate:
 *      plate id string to be added
 *  state:
 *      state id string to be added
 *  code:
 *      summons code integer value used an an index into the fines table
 *      fineTab maps code number to text description & fine cost 
 *      you use this table to get the amount of the fine when calculating
 *      the tot_fine field in struct vehicle.
 *          example:
 *              struct vehicle *vhpt;
 *              vhpt->tot_fine += fineTab[code].fine;
 *
 * return
 *       0 if ok -1 for all errors
 */

int
insertticket(char *summ, char *plate, char *state, int code)
{
	uint32_t hashval = hash(plate) % tabsz; // hash value of plate
	unsigned long summid;					// ticket summons id
	int result = strtosumid(summ, &summid);	// result of strtosumid call

	if (result == -1) {
		return -1;
	}

	struct vehicle *chain = *(htable+hashval); // head of the htable chain
	struct vehicle *current = chain;		// current vehicle
	while (current != NULL) {
		if ((strcmp(current->plate, plate) == 0) && (strcmp(current->state, state) == 0)) {
			struct ticket *curTicket = current->head;	// current ticket
			while (curTicket != NULL) {
				if (curTicket->summons == summid) {
					fprintf(stderr,"%s: duplicate summons %lu\n", argv0, summid);
					return -1;
				}
				curTicket = curTicket->next;
			}

			struct ticket *newTicket = malloc(sizeof(struct ticket));	// new ticket to be inserted
			if (newTicket == NULL) {
				return -1;
			}

			newTicket->summons = summid;
			newTicket->code = code;
			newTicket->next = NULL;

			if (current->head == NULL) {
				current->head = newTicket;
			} else {
				struct ticket *tail = current->head;		// tail ticket
				while (tail->next != NULL) {
					tail = tail->next;
				}
				tail->next = newTicket;
			}

			current->tot_fine += fineTab[code].fine;
			current->cnt_ticket += 1;	
			return 0;
		}
		current = current->next;
	}

	struct vehicle *newVehicle = malloc(sizeof(struct vehicle));	// new vehicle to be inserted
	if (newVehicle == NULL) {
		return -1;
	}
	newVehicle->state = strdup(state);
	newVehicle->plate = strdup(plate);
	newVehicle->tot_fine = fineTab[code].fine;
	newVehicle->cnt_ticket = 1;
	newVehicle->next = chain;
	newVehicle->head = malloc(sizeof(struct ticket));
	if (newVehicle->head == NULL) {
		return -1;
	}
	newVehicle->head->summons = summid;
	newVehicle->head->code = code;
	newVehicle->head->next = NULL;

	*(htable+hashval) = newVehicle;
    return 0;
}

#endif
