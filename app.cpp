#include "app.h"
#include <float.h>

void main(){
  double hz = 30;
  TIME e7 = 10000000;
  TIME delta_t = 1000;
  TIME t; // t: time (unit: 100ns)
  TIME D = delta_t * 10000; // D: duration (1 sec)
  for (t = 0; t <= D; t += delta_t) {
    double norm_sin = sin(M_PI * 2 * t * hz / e7);
    double sweep_sin1;
    {
      double z = M_PI * 2 * t * hz / e7;
      sweep_sin1 = sin(z * ((static_cast<double>(t) / D) + 1));
    }
    double sweep_sin2;
    {
      double k = 1;
      const double hzk = hz * k;
      auto z = [hzk, e7](TIME t) {
          return M_PI * 2 * t * hzk / e7;
        };
      auto g = [&z, D](TIME t, double k) {
          double zt = z(t), zD = z(D);
          return ((k * zD) / M_PI) * (1 - cos(zt / zD * M_PI)) + zt;
        };
      sweep_sin2 = sin(g(t, k) * z(D) / g(D, k));
    }
    double sweep_sin3;
    { // exponential
      // allows to drastically compress by p value
      double k = 2;
      double p = 4;
      const double hzk = hz * k;
      auto z = [hzk, e7](TIME t) {
          return M_PI * 2 * t * hzk / e7;
        };
      auto g = [p, &z, D](TIME t, double k) {
          double zt = z(t);
          return (k / p) * pow(zt, p) + zt;
        };
      sweep_sin3 = sin(g(t, k) * z(D) / g(D, k));
    }
    double sweep_sin4;
    { // linar
      double k = 2;
      const double hzk = hz * k;
      auto z = [hzk, e7](TIME t) {
          return M_PI * 2 * t * hzk / e7;
        };
      auto g = [&z, D](TIME t, double k) {
          return ((k + 1) / 2 / z(D)) * pow(z(t), 2) + z(t);
        };
      sweep_sin4 = sin(g(t, k) * z(D) / g(D, k));
    }
    printf("%d\t%f\t%f\t%f\t%f\t%f\n", t, norm_sin,
      sweep_sin1, sweep_sin2, sweep_sin3, sweep_sin4);
  }
}
