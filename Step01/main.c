﻿/*
 *   backpro メイン (main.c)
 *
 *   A. Date
 *   2018.5.9
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "backpro.h"
#include "irl_utility.h"


// Rumelhart et al (1986) Fig1
void mirror_symmetry_detection(){

}


int main (int argc, char *argv[] ){

  int i;
  int seed = RAND_SEED;

  for (i=1; i<argc; i++) {
    switch (*(argv[i]+1)) {
    case 'r':
      seed = atoi(argv[++i]);
      break;
    default:
      fprintf(stderr, "Usage : %s\n",argv[0]);
      fprintf(stderr, "\t-r : random seed(%d)\n",seed);
      exit(0);
      break;
    }
  }
  srand48(seed);

  mirror_symmetry_detection();
  
  return 0;
}
