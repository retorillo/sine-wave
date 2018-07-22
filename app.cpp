#include "app.h"

void main(){
  TIME delta_t = 1000;
  TIME t = 0; // t: time (unit: 100ns)
  TIME sec1 = delta_t * 10000;
  SINESWEEP curve1 { 3, 5, 2, sec1, sec1, sec1 };
  SINESWEEP curve2 { 3, 10, 0.5, sec1, sec1, sec1 };
  SINESWEEP curve3 { 3, 5, 0.5, sec1, sec1, sec1 };
  std::vector<SINESWEEP*> curves;
  curves.push_back(&curve1);
  curves.push_back(&curve2);
  curves.push_back(&curve3);
  auto lastcurve = curves.back();
  TIME total_D = finalize(0, curves);
  double vol_min = 0.2;
  double vol_max = 1.0;
  double vol_H = 2.0 / (1.0 * total_D / e7);
  TIME last_t = 0;
  double offset = 0;
  double c_nr = 0;
  for (auto c : curves) {
    double H = c->H;
    double K = c->K;
    TIME D0 = c->D0;
    TIME D1 = c->D1;
    TIME D2 = c->D2;
    TIME D  = D0 + D1 + D2;
    TIME max_t = last_t + D;
    for (; t <= max_t; t += delta_t) {
      double vol = sin(t * vol_H / e7 * 2 * M_PI) * (vol_max - vol_min) + vol_min;
      double s = sweep(c, t - last_t);
      printf("%d\t", static_cast<int>(t));
      for (int nr = 0; nr < c_nr; nr++)
        printf("NaN\t");
      printf("%f\n", vol * sin(s + offset));
    }
    offset = fmod(sweep(c, D) + offset, 2 * M_PI);
    last_t = max_t;
    c_nr++;
  }
}
double sweep(SINESWEEP* ss, TIME t) {
  return sweep(ss->n, t, ss->D0, ss->D1, ss->D2, ss->H, ss->K);
}
double sweep(unsigned char n, TIME t, TIME D0, TIME D1, TIME D2, double H, double K) {
  static const auto F = [](unsigned char n, TIME t, TIME D1, double k) {
    double numerator = pow(2, n - 1) * k * pow(t, n + 1);
    double denominator = (n + 1) * pow(D1, n);
    return numerator / denominator;
  };
  static const auto G = [](unsigned char n, TIME t, TIME D1, double k) {
    return k * (t - D1 / 2) + F(n, D1 - t, D1, k);
  };
  static const auto E = [](unsigned char n, TIME t, TIME D0, TIME D1, TIME D2, double H, double k) {
    TIME hD1 = D1 / 2;
    TIME t0 = D0, t1 = D0 + D1, t2 = D0 + D1 + D2;
    if (0 <= t && t <= t0) { return static_cast<double>(t); }
    else if (t0 < t && t <= t0 + hD1) { return F(n, t - t0, D1, k) + t; }
    else if (t0 + hD1 < t && t <= t1) { return G(n, t - t0, D1, k) + t; }
    else if (t1 < t && t <= t2) { return G(n, D1, D1, k) + k * (t - t1) + t; }
    else return 0.0;
  };
  double k = K - 1;
  // NOTE: In this program, D2 is integer, so must be integral multiple of 2,
  // so D2 is increment by 1 if required.
  if (D1 % 2 != 0) D1++;
  return 2 * M_PI * E(n, t, D0, D1, D2, H, k) * H / e7;
}
TIME finalize(double offset, std::vector<SINESWEEP*> curves) {
  auto last_c = curves.back();
  TIME total_D = 0;
  double next_offset;
  for (auto c : curves) {
    TIME D = c->D0 + c->D1 + c->D2;
    next_offset = fmod(sweep(c, D) + offset, 2 * M_PI);
    if (c == last_c && next_offset > 0) {
      // NOTE: When working with sine sweep, finalizing herz is H * K.
      TIME inc = (int)round((1.0 - next_offset / (2 * M_PI)) / (c->H * c->K) * e7);
      c->D2 += inc;
      D += inc;
    }
    total_D += D;
    next_offset = 0;
  }
  return total_D;
}
