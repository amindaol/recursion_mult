#include <stdio.h>
#include <time.h>

double mult1(int n, double x);
double mult2(int n, double x);

int main(void) {
  printf("13 * 2.5 = %f\n", mult1(13, 2.5));
  printf("13 * 2.5 = %f\n", mult2(13, 2.5));
  printf("14 * 10.1 = %f\n", mult1(14, 10.1));
  printf("14 * 10.1 = %f\n", mult2(14, 10.1));

  // --- Tidtaking: mult1 ---
  {
    int ns[] = {10, 100, 1000, 3000, 5000}; // testverdier
    int nn = sizeof(ns) / sizeof(ns[0]);
    int reps = 1000;   // repetisjoner
    double x = 2.5;
    volatile double sink = 0;

    printf("\n[ mult1 ] (Theta(n))\n");
    printf(" n\treps\ttotal(s)\tper_call(us)\n");

    for (int k = 0; k < nn; k++) {
      int n = ns[k];

      clock_t start = clock();
      for (int i = 0; i < reps; i++) {
        sink += mult1(n, x);
      }
      clock_t stop = clock();

      double total = (double)(stop - start) / CLOCKS_PER_SEC;
      double per = total / reps * 1e6;  // µs
      printf(" %d\t%d\t%.6f\t%.3f\n", n, reps, total, per);
    }
    printf("sink=%f\n", sink);
  }

  // --- Tidtaking: mult2 ---
  {
    int ns[] = {10, 1000, 100000, 1000000, 10000000};
    int nn = sizeof(ns) / sizeof(ns[0]);
    int reps = 100000; // større reps for å måle rask metode
    double x = 2.5;
    volatile double sink = 0;

    printf("\n[ mult2 ] (Theta(log n))\n");
    printf(" n\t\treps\ttotal(s)\tper_call(us)\n");

    for (int k = 0; k < nn; k++) {
      int n = ns[k];

      clock_t start = clock();
      for (int i = 0; i < reps; i++) {
        sink += mult2(n, x);
      }
      clock_t stop = clock();

      double total = (double)(stop - start) / CLOCKS_PER_SEC;
      double per = total / reps * 1e6;  // µs
      printf(" %d\t%d\t%.6f\t%.3f\n", n, reps, total, per);
    }
    printf("sink=%f\n", sink);
  }

  return 0;
}

double mult1(int n, double x) {
  if (n == 1) {
    return x;
  }
  return x + mult1(n - 1, x);
}

double mult2(int n, double x) {
  if (n == 1) {
    return x;
  }
  if (n % 2 == 0) {
    return mult2(n / 2, x + x);
  } else {
    return x  + mult2((n - 1) / 2, x + x);
  }
}