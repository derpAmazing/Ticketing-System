#include "Version.h"
#ifndef MYDELTICKET
#warning("WARNING - USING THE SOLUTION delticket.c NOT MY CODE")
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
#include "delticket.h"
#include "subs.h"

/*
 * delticket
 *      remove ticket (summons) from the database
 *      look up the summons for a vehicle and pay it (remove the ticket) 
 *
 *      Find the vehicle by the state and plate. if not found return -1
 *
 *      convert the summ string to a unsigned int using using strtosumid.
 *
 *      After you have the vehicle, find the summons on the ticket chain
 *      that matches the converted summons string 
 *           
 *      If found, delete it from the ticket chain. If the vehicle
 *      no longer has any tickets, then remove the vehicle from the hash chain
 *      Otherwise you must update the tot_fine and cnt_tickets fields in
 *      the struct vehicle so the are accurate after removing the summons
 *      Make sure to free() all space that was malloc()'d 
 *
 * Arguments:
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  summ    summon id string to be found
 *
 * returns 0 if ok -1 if not found or error
 */

int
delticket(char *plate, char *state, char *summ)
{
	uint32_t hashval = hash(plate) % tabsz;	// hash value for vehicle plate
	unsigned long summid;					// summons id
	int result = strtosumid(summ, &summid);	// result of strtosumid for checking

	if (result == -1) {
		return -1;
	}
	
	struct vehicle *prevVehicle = NULL;					// pointer to previous vehicle
	struct vehicle *currentVehicle = *(htable+hashval);	// pointer to current vehicle

	while (currentVehicle != NULL) {
		if ((strcmp(currentVehicle->plate, plate) == 0) && (strcmp(currentVehicle->state, state) == 0)) {
			struct ticket *prevTicket = NULL;			// pointer to previous ticket
			struct ticket *currentTicket = currentVehicle->head;	// pointer to current ticket

			while (currentTicket != NULL) {
				if (currentTicket->summons == summid) {
					currentVehicle->tot_fine -= fineTab[currentTicket->code].fine;
                    currentVehicle->cnt_ticket -= 1;

					if (prevTicket == NULL) {
						currentVehicle->head = currentTicket->next;
					} else {
						prevTicket->next = currentTicket->next;
					}
					free(currentTicket);

					if (currentVehicle->head == NULL) {
						if (prevVehicle == NULL) {
							*(htable+hashval) = currentVehicle->next;
						} else {
							prevVehicle->next = currentVehicle->next;
						}
						free(currentVehicle->plate);
						free(currentVehicle->state);
						free(currentVehicle);
					}

					return 0;
				}
				prevTicket = currentTicket;
				currentTicket = currentTicket->next;
			}
		}
		prevVehicle = currentVehicle;
		currentVehicle = currentVehicle->next;
	}

    return -1;
}
#endif
