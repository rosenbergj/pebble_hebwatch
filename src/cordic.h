#ifndef CORDIC_H_ADDED
#define CORDIC_H_ADDED

double angle_shift ( double alpha, double beta );
double arccos_cordic ( double t, int n );
double arcsin_cordic ( double t, int n );
double arctan_cordic ( double x, double y, int n );
void cossin_cordic ( double beta, int n, double *c, double *s );
int i4_min ( int i1, int i2 );
double sqrt_cordic ( double x, int n );
double tan_cordic ( double beta, int n );

#endif
