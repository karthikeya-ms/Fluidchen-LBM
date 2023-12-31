#include "LBstreaming.hpp"
#include "LBDefinitions.hpp"

void doStreaming(double *collideField, double *streamField,int *flagField,int xlength_lbm){
    int dx, dy, dz;
    double fi;
    // setting distribution function for each moving direction/lattice velocity of every particle
    for (int z = 1; z < xlength_lbm + 1; ++z) {
        for (int y = 1; y < xlength_lbm + 1; ++y) {
            for (int x = 1; x < xlength_lbm + 1; ++x) {
                for (int i = 0; i < Q_NUMBER; ++i) {

                    // dx = c_i_x*dt, where dt = 1, etc.
                    dx = LATTICEVELOCITIES[i][0];
                    dy = LATTICEVELOCITIES[i][1];
                    dz = LATTICEVELOCITIES[i][2];

                    /*New value for our distribution function (DF) of the index 'i'

                     (We set it to DF(i) of the next particle, whose i-th lattice velocity
                     points towards considered particle (x,y,z))

                     Position of that next particle is given by (x-dx, y-dy, z-dz)*/

                    fi = collideField [ Q_NUMBER * ((z-dz)*(xlength_lbm+2)*(xlength_lbm+2) + (y-dy)*(xlength_lbm+2) + x - dx) + i ];
                    streamField [Q_NUMBER * (z*(xlength_lbm+2)*(xlength_lbm+2) + y*(xlength_lbm+2) + x) + i ] = fi;
                }
            }
        }
    }
}
