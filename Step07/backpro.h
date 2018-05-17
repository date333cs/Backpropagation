/*
 *   backpro.h
 */

#ifndef __BACKPRO_H
#define __BACKPRO_H

typedef struct {
  int n1;  /* number of input units */
  int n2;  /* number of units in the second layer */
  double *x; /* input units */
  double *u; /* output of the second layer units */
  double z;  /* output unit */
  double uz;  /* output unit */
  double **s; /* s[j][k]: connection weights */
  double **ds;
  double **ds2;
  double *w; /* w[j]: connection weights to the ouput unit */
  double *dw;
  double *dw2;
} BPNN;

#define RAND_SEED 20180509

BPNN* bpnn_new(int n1, int n2);
void free_bpnn(BPNN* this);
void bpnn_init_weights(BPNN* this);
void bpnn_learning(BPNN* this, double** train_x, double* target_y, int n_examples);
void generate_data_set_mirror_symmetry(double** x, double* y);
void test_x_train(BPNN* this, double** train_x, double* target_y, int n_examples);

void bpnn_draw_figure_geman1992fig9();
double bpnn_output(BPNN* this, double* x);
void  generate_data_set_geman1992fig3(double** x, double* y);
int make_files_of_traing_data(double** train_x, double* target_y, int l);
double f(double u);
  
#endif /* BACKPRO_H*/
