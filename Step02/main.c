/*
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



void rumelhart1986fig1(){

  int n1 = 6; // 入力信号の次元 
  int n2 = 2; // 中間層 の素子数 

  BP* bp;

  // 入力信号の集合を読みこむ（もしくは作成）．

  // 回路を作る．
  n1++; // しきい値の素子分を加える
  n2++;
  bp = bp_new(n1, n2);

  // 回路 bp の初期化

  // bp に入力信号を提示して，学習させる．

  free_bp(bp);

}


BP* bp_new(int n1, int n2) {

    BP* this;
    int j, k;

   this = (BP*) malloc (sizeof(BP));
   if (this == NULL) return NULL;

   this->n1 = n1;
   this->n2 = n2;

   this->x = alloc_1d_dbl(n1);
   for(k=0; k<n1; k++) {
     this->x[k] = 0.0;
   }

   this->u = alloc_1d_dbl(n2);
   for(j=0; j<n2; j++) {
     this->u[j] = 0.0;
   }

   this->s = alloc_2d_dbl(n2,n1);
   this->ds = alloc_2d_dbl(n2,n1);
   for(j=0; j<n2; j++) {
     for(k=0; k<n1; k++) {
       this->s[j][k] = 0.0;
       this->ds[j][k] = 0.0;
     }
   }


   this->w = alloc_1d_dbl(n2);
   this->dw = alloc_1d_dbl(n2);
   for(j=0; j<n2; j++) {
     this->w[j] = 0.0;
     this->dw[j] = 0.0;
   }

   return this;

}




void free_bp(BP* this) {

    int i;
    int n2 = this->n2;

    free((char *) this->x);

    for (i=0; i<n2; i++) {
        free((char *) this->s[i]);
        free((char *) this->ds[i]);
    }
    free((char *) this->w);
    free((char *) this->dw);

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

  rumelhart1986fig1();

  return 0;
}
