

#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>

using namespace std;

const int number_of_parameters = 5;
const int number_of_iterations = 500;
const double CORRECTION = 1;
const double G = 6.67408e-11; // gravitational constant
const double DELTA_T = 1e9; // time quantum

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
    }
}

void proceed_epocha(int number_of_stars, double **stars, double **next_stars) {
    for (int i = 0; i < number_of_stars; i++) {

        double net_force_x = 0;
        double net_force_y = 0;

        double star_mass = stars[i][2];

        for (int j = 0; j < number_of_stars; j++) {
            if (i != j) {
                double delta_x = stars[j][0] - stars[i][0];
                double delta_y = stars[j][1] - stars[i][1];

                // TODO method and pragma
                double distance = sqrt((delta_x * delta_x) + (delta_y * delta_y));

                double force = (CORRECTION * G * star_mass * stars[j][2]) / (distance * distance);

                net_force_x += (force * delta_x) / distance;
                net_force_y += (force * delta_y) / distance;
            }
        }

        double acceleration_x = net_force_x / star_mass;
        double acceleration_y = net_force_y / star_mass;

        next_stars[i][3] = stars[i][3] + DELTA_T * acceleration_x;
        next_stars[i][4] = stars[i][4] + DELTA_T * acceleration_y;

        next_stars[i][0] = stars[i][0] + DELTA_T * next_stars[i][3];
        next_stars[i][1] = stars[i][1] + DELTA_T * next_stars[i][4];
        next_stars[i][2] = star_mass;
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

    for (int k = 0; k < number_of_iterations; ++k) {
        proceed_epocha(number_of_stars, stars, next_stars);
        double **foo_pointer = stars;
        stars = next_stars;
        next_stars = foo_pointer;
    }

    return 0;
};


