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


/*
  Step07
  画像の表示について．
  [-6,6] x [-1.5, 1.5] の領域 （縦 3 x 横 12） 
  刻み幅を 0.01 とすると大きさ 300 x 1200 の画像．

  入力（各場所）に対するニューラルネットの出力値を濃淡色で表現するとFig.9のような画像が描ける．
 */

void bpnn_draw_figure_geman1992fig9(){

  int i;
  int n1 = 2; // 入力信号の次元 
  int n2 = 10; // 中間層 の素子数 
  int n_sweeps = 100000;
  int n_examples = 100;
  int t;
  double mu= 0.4;
  double alpha = 0.0;
  double err;
  double delta_x =0.01;
  int nx = (int)(12.0/delta_x);
  int ny = (int)(3.0/delta_x);
  int n_pixels = nx*ny;
  double* img = alloc_1d_dbl(n_pixels);
  
  BPNN* bpnn;
  double** train_x;  
  double* target_y;
  double* x;  

  double r,c;

  // 入力信号の集合を読みこむ（もしくは作成）．
  train_x = alloc_2d_dbl(n_examples,n1+1);
  target_y = alloc_1d_dbl(n_examples);

  generate_data_set_geman1992fig3(train_x, target_y);
  make_files_of_traing_data(train_x, target_y, 1);   // 例題をファイルに保存
  
  // Creates a new netwotk.
  n1++; // for threshold units
  n2++;
  bpnn = bpnn_new(n1, n2);
  x = alloc_1d_dbl(n1);
  
  //  All weights are randomly initialized.
  bpnn_init_weights(bpnn);

  /*
  for(t=0; t<n_sweeps; t++) {   // 回路に入力信号を提示して，学習させる．
    bpnn_reset_sub_weights(bpnn); // ds, dw のリセット
    bpnn_learning(bpnn, train_x, target_y, n_examples, mu, alpha);
    err = bp_evaluate(bpnn, train_x, target_y, n_examples);
    if (t % 1000 == 0){
      printf("%d %.20lf\n", t, err);
    }
  }
  */
  
  t=0;
  x[n1-1]=1.0;
  for(c= -1.5; c<1.5;  c+=delta_x) {
    for(r= -6.0; r<6.0; r+=delta_x) {
      x[0]=r;       // 入力の提示
      x[1]=c;
      img[t] = bpnn_output(bpnn, x);
      printf("%.2lf %2lf %.10lf\n", c,r,img[t]);
      t++;
    }
    printf("\n");
  }

  free(img);
  free_bpnn(bpnn);

}


/* sigmoid function */
double f(double u){
  
  double z;
  z  = 1.0/(1.0 + exp( -u ) );

  return z;
}


double bpnn_output(BPNN* this, double* x){

  int j, k;
  int n1=this->n1;
  int n2=this->n2;
  double *u = this->u;
  double z = this->z;
  double uz = this->uz;
  double **s = this->s;
  double *w = this->w;
  double uu;
  
  x[n1-1]=1.0;
  u[n2-1]=1.0;
  
  /*** Feed forward input activations. ***/
  for(j=0; j<n2-1; j++) {
    uu = 0.0;
    for(k=0; k<n1; k++) {
      uu += s[j][k]*x[k];
    }
    u[j] = f(uu);
  }
  
  uz = 0.0;
  for(j=0; j<n2; j++) {
    uz += w[j]*u[j];
  }
  z = f(uz);
  
  return z;

}




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
    for(k=0; k<n1-1; k++) {
      x[k] = train_x[a][k];
    }
    y=target_y[a];

    for(j=0; j<n2-1; j++) {
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
   this->ds2 = alloc_2d_dbl(n2,n1);
   for(j=0; j<n2; j++) {
     for(k=0; k<n1; k++) {
       this->s[j][k] = 0.0;
       this->ds[j][k] = 0.0;
       this->ds2[j][k] = 0.0;
     }
   }


   this->w = alloc_1d_dbl(n2);
   this->dw = alloc_1d_dbl(n2);
   for(j=0; j<n2; j++) {
     this->w[j] = 0.0;
     this->dw[j] = 0.0;
     this->dw2[j] = 0.0;
   }

   return this;

}




void free_bpnn(BPNN* this) {

    int i;
    int n2 = this->n2;
    n2++;
    
    free((char *) this->x);
    free((char *) this->u);

    for (i=0; i<n2; i++) {
        free((char *) this->s[i]);
        free((char *) this->ds[i]);
        free((char *) this->ds2[i]);
    }
    free((char *) this->w);
    free((char *) this->dw);
    free((char *) this->dw2);

}



void  generate_data_set_geman1992fig3(double** x, double* y){

  int i;
  double x1, x2;

  i = 0;
  while( i < 50) {
    x1 = 12.0*drand48() - 6.0;
    x2 = 3.0*drand48() - 1.5;
    if ( x2 >= sin(M_PI*x1/2.0) ){
      x[i][0]=x1;
      x[i][1]=x2;
      y[i]=0.95;
      i++;
    }
  }
  
  while( i < 100) {
    x1 = 12.0*drand48() - 6.0;
    x2 = 3.0*drand48() - 1.5;
    if ( x2 < sin(M_PI*x1/2.0) ){
      x[i][0]=x1;
      x[i][1]=x2;
      y[i]=0.05;
      i++;
    }
  }  

  /*
  for (i=0; i<100; i++) {
    printf("%.5lf \t %.5lf \t %.2lf\n", x[i][1], x[i][2], y[i]);
  }
  */
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

int make_files_of_traing_data(double** train_x, double* target_y, int l){
  int a;
  int k;
  FILE *outputfile;  
  outputfile = fopen("ex_positive_001.dat", "w");
  if (outputfile == NULL) {
    printf("cannot make a file.\n");
    exit(1);
  }
  
  for(a=0; a<50; a++) {
    for(k=0; k<2; k++) {
      fprintf(outputfile, "%.8lf ", (double)train_x[a][k]);
    }
    fprintf(outputfile, " %.3lf\n", (double)target_y[a]);
  }
  fclose(outputfile);

  outputfile = fopen("ex_negative_001.dat", "w"); 
  if (outputfile == NULL) {
    printf("cannot make a file.\n");
    exit(1);
  }
  for(a=50; a<100; a++) {
    for(k=0; k<2; k++) {
      fprintf(outputfile, "%.8lf ", (double)train_x[a][k]);
    }
    fprintf(outputfile, " %.3lf\n", (double)target_y[a]);
  }
  fclose(outputfile);

  return 0;
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

  bpnn_draw_figure_geman1992fig9();

  return 0;
}
