#include "error_analysis.hpp"
#include "statistics.hpp"

#include <cmath>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

double autocorrelation_time(std::vector<double> &raw_data, std::vector<double> &p_values, double filter){
  //Initialize time
  int time = 0;
  //Initialize necessary values to pass functions
  double tau=0.0,Czero=0.0, Csum=0.0, y_minus=0.0, y_plus=0.0;
  //Calculate zeroth covariance
  plusminus_average(time,raw_data,filter,y_minus,y_plus);
  autocovariance(time,raw_data,filter,y_minus,y_plus,Csum);
  //Initialize zeroth covariance
  Czero = Csum;
  while(true){
    time+=1;
    //Calculate Y_minus and Y_plus values for net time value
    plusminus_average(time,raw_data,filter,y_minus,y_plus);
    //Calculate covariance for new time value with fresh Y_minus and Y_plus
    autocovariance(time,raw_data,filter,y_minus,y_plus,Csum);
    double Py = Csum/Czero;
    if(Py<0)break;
    tau+=Py;
    p_values.push_back(Py);
  }
  return (tau+0.5);
}

void autocovariance(int &time, std::vector<double> &raw_data, double filter,
                       double &y_minus, double &y_plus, double &Csum){
  //Calculate length of the given vector
  int length =raw_data.size();
  //Make sure Csum is initialized
  Csum=0;
  for(std::vector<double>::iterator i=raw_data.begin()+filter*length;i!=raw_data.end()-time;i++){
    Csum+=(*i-y_minus)*(*(i+time)-y_plus);
  }
  //Normalize Csum
  //(1-filter)*length means clean data's length
  Csum/=(1-filter)*length-time;
}

void plusminus_average(int &time, std::vector<double> &raw_data, double filter,
                       double &y_minus, double &y_plus){
  //Make sure Y_minus and Y_plus initialized
  y_minus=0;y_plus=0;
  //Calculate lenth of the given vector
  int length = raw_data.size();
  for(std::vector<double>::iterator i=raw_data.begin()+filter*length;i!=raw_data.end()-time;i++){
    y_minus+=*i;
    y_plus+=*(i+time);}
  //Normalize these summations with used data part
  //(1-filter)*length means clean data's length
  y_minus/=(1-filter)*length-time;
  y_plus/=(1-filter)*length-time;
}

double ac_standard_error(double tau, double variance, int length, double filter){
  //Because the data is raw data, calculate filtered data's length
  int N = length*(1-filter);
  //Calculate standard error
  variance *=2*tau/N;
  return sqrt(variance);
}

double error_propagation(std::vector<double> &raw_data, std::vector<double> &g_values, double filter){
  //Calculate length of the given vector
  int length = raw_data.size();
  //Calculate average of the given measurements
  double average = mean_value(raw_data,filter);
  //Create effective vector
  std::vector<double> g;
  for(std::vector<double>::iterator iter=raw_data.begin()+filter*length;iter!=raw_data.end();iter++){
    //Calculate effective observable
    double temp = *iter*(*iter);
    temp-=2*average*(*iter);
    g.push_back(temp);
  }
  //Calculate new g vector's average and variance
  average = mean_value(g,0);
  double var = variance(g,average,0);
  //Calculate integrated time for effective observable values
  double g_tau = autocorrelation_time(g,g_values,0);
  std::cout<<"Effective tau = "<<g_tau<<std::endl;
  //Calculate standard error on effective observable
  return ac_standard_error(g_tau,var,length,0);
}

double blocking_method(std::vector<double> &raw_data, int Number_Blocks, double filter){
  //Calculate lenght of the given vector
  int length = raw_data.size();
  //Calculate length of the blocks
  int block_length = length*(1-filter)/Number_Blocks;
  std::vector<double> variances;
  for(int i=0;i<Number_Blocks;i++){
    //Store some part of the measurement values as a small block
    std::vector<double> temporary;
    for(std::vector<double>::iterator iter=raw_data.begin()+filter*length+i*block_length;
        iter!=raw_data.begin()+filter*length+(i+1)*block_length;iter++){
      temporary.push_back(*iter);
    }
    //calculate average and variance of this block and store it
    double average = mean_value(temporary,0);
    double var = variance(temporary,average,0);
    variances.push_back(var);
  }
  //Calculate variance of the variance of the blocks
  double average = mean_value(variances,0);
  return sqrt(variance(variances,average,0)/(double)Number_Blocks);
}

double bootstrap_method(std::vector<double> &raw_data, int M,double tau,double filter){
  //Calculate length of the given vector
  int length = raw_data.size();
  //Calculate length of the bootstrap
  int bootstrap_length = length/2/tau;
  std::vector<double> variances;
  //Define random integer generator and seed it
  std::uniform_int_distribution<int> unidist(filter*length,length-1);
  std::mt19937 gen;
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  gen.seed(seed);
  for(int i=0;i<M;i++){
    std::vector<double> temporary;
    for(int j=0;j<bootstrap_length;j++){
      //Choose random elements of the measurements
      temporary.push_back(raw_data[unidist(gen)]);
    }
    //Calculate variance of this unique block
    double average = mean_value(temporary,0);
    variances.push_back(variance(temporary,average,0));
  }
  //Calculate variance of these blocks
  double average = mean_value(variances,0);
  return sqrt(variance(variances,average,0));
}
