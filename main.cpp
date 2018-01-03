#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>

#include <chrono>

using namespace std;
using namespace std::chrono;

const int number_of_parameters = 5;
const int number_of_iterations = 500;
const double G = 6.67408e-11; // gravitational constant
const double DELTA_T = 2e5; // time quantum

void get_input(int number_of_stars, string file_name, double **stars) {

    std::ifstream ifile(file_name, std::ios::in);

    //check to see that the file was opened correctly:
    if (!ifile.is_open()) {
        cerr << "There was a problem opening the input file!\n";
        exit(1);//exit or do additional error checking
    }

    //keep storing values from the text file so long as data exists:
    for (int i = 0; i < number_of_stars; ++i) {
        ifile >> stars[i][0];
        ifile >> stars[i][1];
        ifile >> stars[i][2];
        //initial velocity
        stars[i][3] = stars[i][1] / 1e9;
        stars[i][4] = - stars[i][0] / 1e9;
    }
}

#pragma omp declare simd
void linear_move(double *next, double origin, double parameter, double delta) {
    *next = origin + parameter * delta;
}

#pragma omp declare simd
void acceleration_move(double *next, double origin, double delta, double acceleration) {
    *next = origin + acceleration * delta * delta / 2;
}

#pragma omp declare simd
double get_distance(double delta_x, double delta_y) { return sqrt((delta_x * delta_x) + (delta_y * delta_y)); }

#pragma omp declare simd
double get_force(double *const *stars, int i, int j, double distance) { return (G * stars[i][2] * stars[j][2]) / (distance * distance); }

#pragma omp declare simd
double get_net_force(double delta, double distance, double force) { return (force * delta) / distance; }

void proceed_epocha(int number_of_stars, double **stars, double **next_stars) {
    for (int i = 0; i < number_of_stars; i++) {
        double net_force_x = 0;
        double net_force_y = 0;
#pragma omp parallel for simd schedule(guided)
        for (int j = 0; j < number_of_stars; j++) {
            double delta_x = stars[j][0] - stars[i][0];
            double delta_y = stars[j][1] - stars[i][1];

            double distance = get_distance(delta_x, delta_y);
            double force = get_force(stars, i, j, distance);

            net_force_x += get_net_force(delta_x, distance, force);
            net_force_y += get_net_force(delta_y, distance, force);
        }

        double acceleration_x = net_force_x / stars[i][2];
        double acceleration_y = net_force_y / stars[i][2];

        acceleration_move(&next_stars[i][4], stars[i][4], DELTA_T, acceleration_y);
        acceleration_move(&next_stars[i][3], stars[i][3], DELTA_T, acceleration_x);
        linear_move(&next_stars[i][0], stars[i][0], DELTA_T, next_stars[i][3]);
        linear_move(&next_stars[i][1], stars[i][1], DELTA_T, next_stars[i][4]);

        next_stars[i][2] = stars[i][2];
    }
}

int main(int argc, char* argv[]) {

    int number_of_stars = atoi(argv[1]); // number of stars

    string inputFile = argv[2]; // input file

    // stars: x, y, weight, velocity x, velocity y
    double **stars = new double *[number_of_stars];
    for (int i = 0; i < number_of_stars; ++i) {
        stars[i] = new double[number_of_parameters];
        for (int j = 0; j < number_of_parameters; ++j) {
            stars[i][j] = 0.f;
        }
    }

    double **next_stars = new double *[number_of_stars];
    for (int i = 0; i < number_of_stars; ++i) {
        next_stars[i] = new double[number_of_parameters];
        for (int j = 0; j < number_of_parameters; ++j) {
            next_stars[i][j] = 0.f;
        }
    }

    get_input(number_of_stars, inputFile, stars);

    high_resolution_clock::time_point start = high_resolution_clock::now();

    for (int k = 0; k < number_of_iterations; ++k) {
        proceed_epocha(number_of_stars, stars, next_stars);
        double **foo_pointer = stars;
        stars = next_stars;
        next_stars = foo_pointer;

    }

    double totalTime = duration_cast<duration<double>>(high_resolution_clock::now() - start).count();

    cout << number_of_stars << " - " << totalTime << "\n";

    return 0;
};


