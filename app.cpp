#include "app.h"
#include <float.h>

void main(){
  double hz = 30;
  TIME delta_t = 1000;
  TIME t; // t: time (unit: 100ns)
  TIME D = delta_t * 10000; // D: duration (1 sec)
  for (t = 0; t <= D; t += delta_t) {
    // double norm_sin = sin(M_PI * 2 * t * hz / e7);
    double cubic_sweep_k1 = cubic_sweep_fn(t, D, hz, 1);
    double cubic_sweep_k2 = cubic_sweep_fn(t, D, hz, 2);
    double cubic_sweep_k8 = cubic_sweep_fn(t, D, hz, 8);
    double cubic_sweep_k05 = cubic_sweep_fn(t, D, hz, 0.5);
    printf("%d\t%f\t%f\t%f\t%f\n", static_cast<int>(t), cubic_sweep_k1,
      cubic_sweep_k2, cubic_sweep_k8, cubic_sweep_k05);
  }
}

double cubic_sweep_fn(TIME t, TIME D, double hz, double k){
  static const auto easeint = [](TIME t, TIME D, double k) {
    if (0 <= t && t <= 0.5 * D)
        return (k - 1) * pow(t, 4) / pow(D, 3) + t;
    else if (0.5 * D <= t && t <=  D)
      return (k - 1) * (pow(t,4)/pow(D,3) - 4*pow(t, 3)/pow(D,2)
        + 6*pow(t,2)/D - 3.0*t + D/2.0 ) + t;
    else
      return 0.0;
  };
  static const auto ease = [](TIME t, TIME D, double k) {
    if (0 <= t && t <= 0.5 * D)
      return 4.0 * (k - 1) * pow(1.0 * t / D, 3) + 1;
    else if (0.5 * D <= t && t <= D)
      return (k - 1) * (4.0 * pow(1.0 * t / D - 1, 3) + 1) + 1;
    else
      return 0.0;
  };
  static const auto z = [] (TIME t, double hz) {
      return M_PI * 2 * t * hz / e7;
    };
  double St = easeint(D, D, k) - easeint(0, D, k);
  return sin(z(easeint(t, D, k) / St * D, St / D * hz));
};
