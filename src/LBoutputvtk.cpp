#include "LBoutputvtk.hpp"
#include "LBDefinitions.hpp"
#include "LBcomputeCellValues.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <string>


void writeVtkOutput(const double* const collideField, const int* const flagField, std::string _case_name, unsigned int t, int xlength_lbm, std::string _dict_name)
{
    int len = xlength_lbm + 2;
    // save filename as a combination of passed filename and timestep
    std::string fn = _dict_name + "/" + _case_name + "." + std::to_string(t) + ".vtk";

    std::ofstream fp(fn);
    if (!fp)
    {
        std::cerr << "Failed to open file!" << std::endl;
        return;
    }

    // write header
    fp << "# vtk DataFile Version 2.0\n";
    fp << "generated by CFD-lab course output \n";
    fp << "ASCII\n\n";
    fp << "DATASET STRUCTURED_GRID\n";
    fp << "DIMENSIONS " << xlength_lbm << " " << xlength_lbm << " " << xlength_lbm << " \n";
    fp << "POINTS " << (xlength_lbm) * (xlength_lbm) * (xlength_lbm) << " float\n\n";

    // print lattice points
    double step = 1.0 / (xlength_lbm - 1);
    for (double x = 0; x <= xlength_lbm - 1; x += 1)
    {
        for (double y = 0; y <= xlength_lbm - 1; y += 1)
        {
            for (double z = 0; z <= xlength_lbm - 1; z += 1)
            {
                fp << x * step << " " << y * step << " " << z * step << "\n";
            }
        }
    }

    double density;
    double vel[D];
    const double* currentCell;

    // write density data
    fp << "\nPOINT_DATA " << (xlength_lbm) * (xlength_lbm) * (xlength_lbm) << " \n";
    fp << "SCALARS density float 1 \n";
    fp << "LOOKUP_TABLE default \n";

    for (int x = 1; x < xlength_lbm + 1; ++x)
    {
        for (int y = 1; y < xlength_lbm + 1; ++y)
        {
            for (int z = 1; z < xlength_lbm + 1; ++z)
            {
                currentCell = collideField + Q_NUMBER * (z * len * len + y * len + x);
                computeDensity(currentCell, &density);
                fp << density << "\n";
            }
        }
    }

    // compute velocities for all cells
    fp << "\nVECTORS velocity float\n";

    for (int x = 1; x < xlength_lbm + 1; ++x)
    {
        for (int y = 1; y < xlength_lbm + 1; ++y)
        {
            for (int z = 1; z < xlength_lbm + 1; ++z)
            {
                currentCell = collideField + Q_NUMBER * (z * len * len + y * len + x);
                computeDensity(currentCell, &density);
                computeVelocity(currentCell, &density, vel);
                fp << vel[0] << " " << vel[1] << " " << vel[2] << "\n";
            }
        }
    }

    // close the file (automatically closed when 'fp' goes out of scope)
}
