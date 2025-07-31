/*
 * rel.h
 *
 *  Created on: Aug 28, 2023
 *      Author: BaracchiF
 */

#ifndef _REL_H_
#define _REL_H_

/* Include Compiuling Data **/

#include "nrcomp.h"

/* Release struct ***********/

typedef struct {
	char	fw_name[16];
	char	hw_name[16];
	char	rel_nr[16];
	char	rel_date[16];
	char	rel_comp[16];
	char	rel_note[48];
} release_struct;

/* Release Data *************/

static const __attribute__ ((section(".id_data"))) release_struct	rel = {
/* fw_name  */	"ElbALTM",
/* hw_name  */	"MIRTILLO-PICO",
/* rel_nr   */	"1.0",
/* rel_date */	DATA_COMPILE,
/* rel_comp */	NR_COMPILE,
/* rel_note */	"-",
};


#endif /* _REL_H_ */
