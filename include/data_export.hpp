#ifndef DATA_EXPORT_HPP
#define DATA_EXPORT_HPP

#include <string>
#include <vector>

void dataexport(
    const std::string& data_name,
    const std::string& file_type,
    const std::vector<double>& data,
    const std::string& x_column,
    const std::string& y_column,
    double filter,
    int step,
    int grid_points,
    bool divide_by_grid_points = true
);

#endif
