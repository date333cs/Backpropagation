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
  double *uu;
  double z;  /* output unit */
  double uz;
  double **s; /* s[j][k]: connection weights */
  double **ds;
  double *w; /* w[j]: connection weights to the ouput unit */
  double *dw;
} BP;

#define RAND_SEED 20180509

BP* bp_new(int n1, int n2);
void free_bp(BP* this);
void bp_init_weights(BP* this);
void bp_learning(BP* bp, int train_x[64][7], int n_examples);
void test_x_train(BP* this, int train_x[64][7], int n_examples);
  
#endif /* BACKPRO_H*/
