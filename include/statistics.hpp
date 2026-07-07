#ifndef STATISTICS_HPP
#define STATISTICS_HPP

//Call it by reference via &
#include<vector>
double mean_value(std::vector<double>& raw_data, double filter = 0.0);
double variance(std::vector<double>& raw_data, double& mean, double filter = 0.0);
std::vector<double> power_fitting(std::vector<double>& x_values, std::vector<double>& y_values);

#endif  // STATISTICS_HPP
