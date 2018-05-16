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

  BPNN* bpnn;
  double** train_x;
  double* target_y;
  
  // 入力信号の集合を読みこむ（もしくは作成）．
  train_x = alloc_2d_dbl(64,n1);
  target_y = alloc_1d_dbl(64);
  // double train_x[64][7] = {
  // { 0, 0, 0, 0, 0, 0 },
  // { 0, 0, 0, 0, 0, 1 },
  generate_data_set_mirror_symmetry(train_x, target_y);

  // Creates a new netwotk.
  n1++; // for threshold units
  n2++;
  bpnn = bpnn_new(n1, n2);

  //  All weights are randomly initialized.
  bpnn_init_weights(bpnn);

  test_x_train(bpnn, train_x, target_y, n_examples);

  exit(0);
    

  // bp に入力信号を提示して，学習させる．
  for(t=0; t<n_epochs; t++) {
    bpnn_learning(bpnn, train_x, target_y, n_examples);
    // 回路の評価
  }

  free_bpnn(bpnn);

}

void bpnn_init_weights(BPNN* this){

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


void bpnn_learning(BPNN* this, double** train_x, double* target_y, int n_examples){

  int a;
  int j, k;
  int n1=this->n1;
  int n2=this->n2;
  double *x = this->x;
  double *u = this->u;
  double z = this->z;
  double uz = this->uz;
  double **s = this->s;
  double **ds = this->ds;
  double *w = this->w;
  double *dw = this->dw;
  double uu, y;
  
  x[n1-1]=1.0;
  u[n2-1]=1.0;
  
  for(a=0; a<n_examples; a++) {
    // 入力の提示
    for(k=0; k<n1; k++) {
      x[k] = train_x[a][k];
    }
    y=target_y[a];

    for(j=1; j<n2; j++) {
      uu = 0.0;
      for(k=0; k<n1; k++) {
	uu += s[j][k]*x[k];
      }
      u[j] = 1.0/(1+ exp( -uu ) );
    }
    
    uz = 0.0;
    for(j=0; j<n2; j++) {
      uz += w[j]*u[j];
    }
    z = 1.0/(1+ exp( -uz ) );

    // ds, dw の update

  }

  // s, w の update

}


BPNN* bpnn_new(int n1, int n2) {

    BPNN* this;
    int j, k;

   this = (BPNN*) malloc (sizeof(BPNN));
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




void free_bpnn(BPNN* this) {

    int i;
    int n2 = this->n2;

    free((char *) this->x);
    free((char *) this->u);

    for (i=0; i<n2; i++) {
        free((char *) this->s[i]);
        free((char *) this->ds[i]);
    }
    free((char *) this->w);
    free((char *) this->dw);

}


void generate_data_set_mirror_symmetry(double** x, double* y){

  int i,j,a;

  for (i=0; i<64; i++) {

    for (j=0; j<6; j++) {
      x[i][j]=0.0;
    }

    a = i+1;
    if (a > 32){
      x[i][0]=1.0;
      a = a-32;
    }
    if (a > 16){
      x[i][1]=1.0;
      a = a-16;
    }
    if (a > 8){
      x[i][2]=1.0;
      a = a-8;
    }
    if (a > 4){
      x[i][3]=1.0;
      a = a-4;
    }
    if (a > 2){
      x[i][4]=1.0;
      a = a-2;
    }
    if (a > 1){
      x[i][5]=1.0;
    }
  }

  for (i=0; i<64; i++) {
    y[i]=0.0;
    if ( fabs(x[i][0]-x[i][5]) < 0.01 &&  fabs(x[i][1]-x[i][4]) < 0.01  && fabs(x[i][2]-x[i][3]) < 0.01 ){
      y[i] = 1.0;
    }
  }

}


void test_x_train(BPNN* this, double** train_x, double* target_y, int n_examples){
  int a, k;
  int n1 = this->n1 -1;
  
  for(a=0; a<n_examples; a++) {
    for(k=0; k<n1; k++) {
      printf("%.0lf", train_x[a][k]);
    }
    printf(" %.0lf\n", target_y[a]);
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
