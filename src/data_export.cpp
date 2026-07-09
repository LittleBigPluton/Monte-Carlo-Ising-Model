#include "data_export.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Export data to desired file type
void dataexport(
    const std::string& data_name,
    const std::string& file_type,
    const std::vector<double>& data,
    const std::string& x_column,
    const std::string& y_column,
    double filter,
    int step,
    int grid_points,
    bool divide_by_grid_points
)
// Body of dataexport function
{
    std::string filename = data_name + "." + file_type;
    std::ofstream write(filename,std::ios::out);

    if (!write)
    {
        std::cerr << "Could not create file: " << filename << std::endl;
        return;
    }

    int length = static_cast<int>(data.size());
    int start =  static_cast<int>(filter * length);

    if (step <= 0)
    {
      step = 1;
    }

    write << x_column << " " << y_column << "\n";

    for (int i = start; i < length; i += step)
    {
        double value = data[i];

        if (divide_by_grid_points && grid_points > 0)
        {
            value /= grid_points;
        }

        write << i + 1 << " " << value << "\n";
    }
}
