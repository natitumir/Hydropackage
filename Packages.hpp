#ifndef PACKAGES_HPP
#define PACKAGES_HPP




#include<iostream>
#include<vector>
#include<map>
#include<string>






#include "Grid1D.hpp"
#include "Parameter_Input.hpp"






//================================================================
//Below are some functions that are essential for any of the different hydro solutions.Some of these methods will be virtual 
//and as such overridden by the individual derived packages.


//Due to my architechture, this one has to have everything methods as found in the base classes.


class Hydropackage {
    public:
         
        virtual void step_initial_condition(grid1d& grid, ParameterInput& pin){} //Used to impose step initial condition. 
        virtual void convert_to_primitive(grid1d& grid){}// Used to convert to primitive. It will be overridden by individual packages
        virtual void convert_to_conserved(grid1d& grid){}
        virtual void flux_calculate (grid1d& grid){}
        //==============Helper methods=====================

        //===========Minmod slope limiter=================
        //minmod slope limiter for reconstruction
        inline double minmod(double alpha,double beta){
            if(abs(alpha)<abs(beta) && alpha*beta>0){
                return alpha;
            }
            else if(abs(alpha)>abs(beta) && alpha*beta>0){
                return beta;
            }
            else return 0;
        }

        //=============The main reconstruction processess=================================

        void minmod_reconstruction(grid1d& grid){    //Make sure that when you use this, the grid.q is alredy primitive

            for(int i = 0; i < grid.nvariable; i++) {
                for(int j = 0; j <= grid.ncell; j++) {
                    grid.qL[i][j] = grid.q[i][j+1] + (grid.dx/2.0) * minmod((grid.q[i][j+2] - grid.q[i][j+1])/grid.dx, (grid.q[i][j+1] - grid.q[i][j])/grid.dx);
                    grid.qR[i][j] = grid.q[i][j+2] - (grid.dx/2.0) * minmod((grid.q[i][j+3] - grid.q[i][j+2])/grid.dx, (grid.q[i][j+2] - grid.q[i][j+1])/grid.dx);
                }
            }         //////////////////////////////////////these 1,2 etc are actually functions of nghost. Figure it out
                      /////////////////////////////////////////////////////////////////////////
                      ///////////////////////////////////////////////////////////////////////
                    
        }

        
        

        

};












#endif