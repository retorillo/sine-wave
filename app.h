#include <stdio.h>
#define _USE_MATH_DEFINES
#include <cmath>
typedef unsigned __int64 TIME;
#include <float.h>
#include <vector>

struct SINESWEEP{
  unsigned char n;
  double H;
  double K;
  TIME D0;
  TIME D1;
  TIME D2;
};

const TIME e7 = 10000000;
double sweep(unsigned char n, TIME t, TIME D0, TIME D1, TIME D2, double H, double k);
double sweep(SINESWEEP* ss, TIME t);
TIME finalize(double offset, std::vector<SINESWEEP *> curves);
