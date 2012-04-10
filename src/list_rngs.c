/*
* $Id: list_rngs.c 472 2008-09-22 04:13:56Z rgbatduke $
*
* See copyright in copyright.h and the accompanying file COPYING
* See also accompanying file STS.COPYING
*
*/

/*
 *========================================================================
 * This just lists the available rng's.  This is the default action of
 * dieharder when no test is specified.
 *========================================================================
 */

#include "dieharder.h"

void list_rngs()
{
 int i,j;

 /*
  * This is run right away or not at all, so just announce that we're
  * listing rngs if verbose is set at all.
  */
 if(verbose){
   Rprintf("list_rngs():\n");
 }

 /*
  * These have to be called to set up the types[] vector with all
  * the available rngs so we can list them.  This SHOULD be the first
  * time this is called (and the only time) but it shouldn't leak memory
  * even if it is called more than once -- it uses a static global vector
  * in libdieharder's data space and is therefore persistent.
  */
 dieharder_rng_types();
 add_ui_rngs();


 /* Version string seems like a good idea */
 dh_header();
 Rprintf("#   %3s %-20s|%3s %-20s|%3s %-20s#\n"," Id","Test Name",
         " Id","Test Name"," Id","Test Name");
 Rprintf("#=============================================================================#\n");
 i = 0;
 j = 0;
 while(dh_rng_types[i] != NULL){
   if(j%3 == 0) Rprintf("|   ");
   Rprintf("%3.3d %-20s|", i, dh_rng_types[i]->name);
   if(((j+1)%3) == 0 && i>0) Rprintf("\n");
   i++;
   j++;
 }
 /*
  * Finish off each block neatly.  If j == 0, we finished the row
  * and do nothing.  Otherwise, pad to the end of the row.
  */
 j = j%3;
 if(j == 1) Rprintf("                        |                        |\n");
 if(j == 2) Rprintf("                        |\n");
 Rprintf("#=============================================================================#\n");
 i = 200;
 j = 0;
 while(dh_rng_types[i] != NULL){
   if(j%3 == 0) Rprintf("|   ");
   Rprintf("%3.3d %-20s|", i, dh_rng_types[i]->name);
   if(((j+1)%3) == 0 && i>200) Rprintf("\n");
   i++;
   j++;
 }
 j = j%3;
 if(j == 1) Rprintf("                        |                        |\n");
 if(j == 2) Rprintf("                        |\n");
 Rprintf("#=============================================================================#\n");
 i = 400;
 j = 0;
 while(dh_rng_types[i] != NULL){
   if(j%3 == 0) Rprintf("|   ");
   Rprintf("%3d %-20s|", i, dh_rng_types[i]->name);
   if(((j+1)%3) == 0 && i>400) Rprintf("\n");
   i++;
   j++;
 }
 j = j%3;
 if(j == 1) Rprintf("                        |                        |\n");
 if(j == 2) Rprintf("                        |\n");
 Rprintf("#=============================================================================#\n");
 i = 500;
 j = 0;
 while(dh_rng_types[i] != NULL){
   if(j%3 == 0) Rprintf("|   ");
   Rprintf("%3d %-20s|", i, dh_rng_types[i]->name);
   if(((j+1)%3) == 0 && i>500) Rprintf("\n");
   i++;
   j++;
 }
 j = j%3;
 if(j == 1) Rprintf("                        |                        |\n");
 if(j == 2) Rprintf("                        |\n");
 Rprintf("#=============================================================================#\n");
 if(dh_num_user_rngs){

   i = 600;
   j = 0;
   while(dh_rng_types[i] != NULL){
     if(j%3 == 0) Rprintf("|   ");
     Rprintf("%3d %-20s|", i, dh_rng_types[i]->name);
     if(((j+1)%3) == 0 && i>600) Rprintf("\n");
     i++;
     j++;
   }
   j = j%3;
   if(j == 1) Rprintf("                        |                        |\n");
   if(j == 2) Rprintf("                        |\n");
   Rprintf("#=============================================================================#\n");

 }

}

