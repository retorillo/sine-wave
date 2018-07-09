#include "app.h"

void main(){
  double hz = 1;
  TIME e7 = 10000000;
  TIME delta_t = 1000;
  TIME maxt = delta_t * 10000; // 1 sec
  for (TIME t = 0; t <= maxt; t += delta_t) {
    double norm_sin = sin(M_PI * 2 * t * hz / e7);
    double z = M_PI * 2 * t * hz / e7;
    double sweep_sin = sin(z * ((static_cast<double>(t) / maxt) + 1));
    printf("%d\t%f\t%f\n", t, norm_sin, sweep_sin);
  }
}
