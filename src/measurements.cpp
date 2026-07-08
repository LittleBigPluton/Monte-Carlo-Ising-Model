#include "measurements.hpp"

//Specific heat
void calculate_specific_heat(double& variance, double& Temperature, int& grid_points, MeasurementSummary& summary)
{
  //Calculate varience of the rest part
  summary.specific_heat = variance/Temperature/Temperature/grid_points;
}

//Magnetic susceptibility
void calculate_magnetic_susceptibility(double& variance, double& Temperature, int& grid_points, MeasurementSummary& summary)
{
  //Calculate varience value of the equilibrated part
  summary.magnetic_susceptibility = variance/Temperature/grid_points;
}
