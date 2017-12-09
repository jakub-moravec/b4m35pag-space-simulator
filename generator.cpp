#include <vector>
#include <random>
#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace std::chrono;

constexpr double solarmass = 1.98892e30;
constexpr double pi = 3.14159265358979323;
constexpr double radius = 1e18;

void generateInstance(int N);

void generateInstance(int N) {
  if (N <= 0)
    throw invalid_argument("The number of bodies must be a positive number!");
  cout << N << endl;

  default_random_engine generator;
  generator.seed(time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count());
  double reductionMult = 0.5 * exp(-1.8);
  uniform_int_distribution<int> signumChoice(0, 3);
  uniform_real_distribution<double> distMult(-0.5, 0.5), wheel(0.0, 1.0);

  for (int i = 1; i < N; i++) {
    double px = 1.5 * wheel(generator);
    double py = 0.5 * sqrt(1.0 - (px / 1.5) * (px / 1.5));
    int choice = signumChoice(generator);
    if (choice == 1)
      px = -px;
    if (choice == 2)
      py = -py;
    if (choice == 3) {
      px = -px;
      py = -py;
    }

    double rotation, pxn, pyn, offset;
    if (wheel(generator) < 0.5) {
      rotation = pi / 4.0;
      offset = 0.1;
    } else {
      rotation = -pi / 4.0;
      offset = -0.1;
    }

    pxn = cos(rotation) * px - sin(rotation) * py;
    pyn = sin(rotation) * px + cos(rotation) * py;
    px = pxn;
    py = pyn;
    px += offset;
    py += offset;

    double radiusScaling =
        max(0.3, 1.3 * wheel(generator)) * radius * reductionMult;
    px *= radiusScaling;
    py *= radiusScaling;

    double mass = 2.0 * wheel(generator) * solarmass + 1e20;
    cout << px << " " << py << " " << mass << endl;
  }

  // Put a heavy body in the center (coordinates and speeds are already zero)
  cout << 0.0 << " " << 0.0 << " " << 1e6 * solarmass << endl;
}

int main(int argc, char *argv[]) {
  int n;
  if (argc > 1) {
    n = atoi(argv[1]);
  } else {
    cout << "Number of stars to generate: ";
    cin >> n;
  }
  cout.precision(17);
  generateInstance(n);
  return 0;
}
