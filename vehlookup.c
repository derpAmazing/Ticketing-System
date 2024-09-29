#include "Version.h"
#ifndef MYVEHLOOKUP
#warning("WARNING - USING THE SOLUTION vehlookup.c NOT MY CODE")
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
#include "vehlookup.h"
#include "subs.h"

/*
 * vehiclelookup
 *          look for vehicle in the database
 *          vehicle must match both plate and state strings
 *
 * Arguments:
 *
 *  plate:
 *          plate id string to be found
 *  state:
 *          state id string to be found
 *
 * return:
 *          pointer to vehicle if found
 *          NULL otherwise
 */

struct vehicle *
vehiclelookup(char *plate, char *state)
{
	uint32_t hashval = hash(plate) % tabsz;			// hash value of plate
	struct vehicle *chain = *(htable + hashval);	// vehicle in the chain

	while (chain != NULL) {
		if ((strcmp(chain->state, state) == 0) && (strcmp(chain->plate, plate) == 0)) {
			return chain;
		}
		chain = chain->next;
	}
	return NULL;
}
#endif
