//Call it by reference via &
#include<vector>
double mean(std::vector<double>& raw_data, int& length);
double varience(std::vector<double>& raw_data, double& mean, int& length);
std::vector<double> power_fitting(std::vector<double>& x_values, std::vector<double>& y_values, int& length);
