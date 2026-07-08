#ifndef MEASUREMENTS_HPP
#define MEASUREMENTS_HPP

#include <vector>

struct Measurements
{
    std::vector<double> energies;
    std::vector<double> magnetizations;
};

struct MeasurementSummary
{
    double energy_density = 0.0;
    double magnetization_density = 0.0;

    double specific_heat = 0.0;
    double magnetic_susceptibility = 0.0;

    double energy_standard_error = 0.0;
    double magnetization_standard_error = 0.0;
};

void calculate_specific_heat(double& variance, double& Temperature, int& grid_points, MeasurementSummary& summary);
void calculate_magnetic_susceptibility(double& variance, double& Temperature, int& grid_points, MeasurementSummary& summary);

#endif
