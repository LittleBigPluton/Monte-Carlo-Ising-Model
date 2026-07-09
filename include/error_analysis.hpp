#ifndef ERROR_ANALYSIS_HPP
#define ERROR_ANALYSIS_HPP

#include<vector>
double autocorrelation_time(std::vector<double> &raw_data, std::vector<double> &p_values, double filter);
void autocovariance(int &time, std::vector<double> &raw_data, double filter,
                       double &y_minus, double &y_plus, double &Csum);
void plusminus_average(int &time, std::vector<double> &raw_data, double filter,
                       double &y_minus, double &y_plus);
double ac_standard_error(double tau, double variance, int length, double filter);
double error_propagation(std::vector<double> &raw_data,std::vector<double> &g_values,double filter);
double blocking_method(std::vector<double> &raw_data, int NB, double filter);
double bootstrap_method(std::vector<double> &raw_data, int M,double tau,double filter);

#endif
