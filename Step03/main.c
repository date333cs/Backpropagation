/*
 *   backpro メイン (main.c)
 *
 *   A. Date
 *   2018.5.10
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "backpro.h"
#include "irl_utility.h"

/* Fig 1 of Rumelhart et al 1986 */
void mirror_symmetry_detection(){

  int n1 = 6; // 入力信号の次元 
  int n2 = 2; // 中間層 の素子数 
  int n_epochs = 1000;
  int n_examples = 64;
  int t;

  BP* bp;
  int** train_x;
  int* target_y;
  
  // 入力信号の集合を読みこむ（もしくは作成）．
  train_x = alloc_2d_int(64,7);
  target_y = alloc_1d_int(64);
  // int train_x[64][7] = {
  // { 0, 0, 0, 0, 0, 0 },
  // { 0, 0, 0, 0, 0, 1 },
  generate_data_set_mirror_symmetry(train_x, target_y);

  // Creates a new netwotk.
  n1++; // for threshold units
  n2++;
  bp = bp_new(n1, n2);

  //  All weights are randomly initialized.
  bp_init_weights(bp);

  test_x_train(bp, train_x, target_y, n_examples);
  exit(0);

  // bp に入力信号を提示して，学習させる．
  for(t=0; t<n_epochs; t++) {
    bp_learning(bp, train_x, target_y, n_examples);
    // 回路の評価
  }

  free_bp(bp);

}



void bp_init_weights(BP* this){

  int j, k;
  int n1, n2;
  n1 = this->n1;
  n2 = this->n2;
  
  for(j=0; j<n2; j++) {
    for(k=0; k<n1; k++) {
      this->s[j][k] = 0.1*nrand();
    }
  }
  
  for(j=0; j<n2; j++) {
    this->w[j] = 0.1*nrand();
  }
}


void bp_learning(BP* this, int** train_x, int* target_y, int n_examples){

  int a;
  int j, k;
  int n1=this->n1;
  int n2=this->n2;
  double *x = this->x;
  double *u = this->u;
  double *uu = this->uu;
  double z = this->z;
  double uz = this->uz;
  double **s = this->s;
  double **ds = this->ds;
  double *w = this->w;
  double *dw = this->dw;
  double y;
  
  x[0]=1.0;
  u[0]=1.0;

  
  for(a=0; a<n_examples; a++) {
    // 入力の提示
    for(k=1; k<n1; k++) {
      x[k] = (double)train_x[a][k-1];
    }
    y = (double)target_y[a];
    
    for(j=1; j<n2; j++) {
      uu[j] = 0.0;
      for(k=0; k<n1; k++) {
	uu[j] += s[j][k]*x[k];
      }
      u[j] = 1.0/(1+ exp( -uu[j] ) );
    }
    
    uz = 0.0;
    for(j=0; j<n2; j++) {
      uz += w[j]*u[j];
    }
    z = 1.0/(1+ exp( -uz ) );

    // ds, dw の update
    // update_ds, dw;

  }
  // s, w の update

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
   this->uu = alloc_1d_dbl(n2);
   for(j=0; j<n2; j++) {
     this->u[j] = 0.0;
     this->uu[j] = 0.0;
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
    free((char *) this->u);
    free((char *) this->uu);

    for (i=0; i<n2; i++) {
        free((char *) this->s[i]);
        free((char *) this->ds[i]);
    }
    free((char *) this->w);
    free((char *) this->dw);

}


void generate_data_set_mirror_symmetry(int** x, int* y){

  int i,a;

  for (i=0; i<64; i++) {
    x[i][0]=1; // dammy. no use.
    a = i+1;
    if (a > 32){
      x[i][1]=1;
      a = a-32;
    }
    if (a > 16){
      x[i][2]=1;
      a = a-16;
    }
    if (a > 8){
      x[i][3]=1;
      a = a-8;
    }
    if (a > 4){
      x[i][4]=1;
      a = a-4;
    }
    if (a > 2){
      x[i][5]=1;
      a = a-2;
    }
    if (a > 1){
      x[i][6]=1;
    }
  }

  for (i=0; i<64; i++) {
    y[i]=0;
    if ( x[i][1]==x[i][6] &&  x[i][2]==x[i][5] && x[i][3]==x[i][4]){
      y[i] = 1;
    }
  }

}


void test_x_train(BP* this, int** train_x, int* target_y, int n_examples){
  int a;
  int k;
  int n1 = this->n1;
  double *x = this->x;
  double y;
  
  for(a=0; a<n_examples; a++) {
    for(k=1; k<n1; k++) {
      x[k] = (double)train_x[a][k];
      printf("%.0lf", x[k]);
    }
    y=(double)target_y[a];
    printf(" %.0lf\n", y);
  }


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
