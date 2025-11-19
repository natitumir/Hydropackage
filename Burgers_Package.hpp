#ifndef BURGERS_PACKAGE_HPP
#define BURGERS_PACKAGE_HPP


#include "Grid1D.hpp"
#include "Packages.hpp"
#include "Parameter_Input.hpp"


//===========This one will only hold stuff unique to Burgers equation



class BurgersEquation : public Hydropackage {

    void convert_to_primitive (grid1d& grid) override {}//Well, yeah it does nothing. I just add it here because Euler, SR etc. will 
                                                         //need to use it
    void convert_to_conserved (grid1d& grid) override {}



    

    void flux_calculate (grid1d& grid) override {// ++++++++++++++++++++++++++++ALWAYS CHECK if the logic of the loop is right+++++++++
        for(int j=0; j<=grid.ncell; j++){
            grid.numericflux[0][j]= 0.5*(0.5*(grid.qL[0][j]*grid.qL[0][j]+grid.qR[0][j]*grid.qR[0][j])-0.5*(grid.qL[0][j]+grid.qR[0][j])*(grid.qR[0][j]-grid.qL[0][j]));
        }
    }



    void step_initial_condition(grid1d& grid, ParameterInput& pin) override {
                
                vector<double> left_value(1,0.0);
                vector<double> right_value(1,0.0);
                double  midpoint;
                


                left_value[0]=pin.Getdouble("initial_condition","left_value");
                right_value[0] = pin.Getdouble("initial_condition","right_value");
                midpoint = pin.Getdouble("initial_condition","midpoint"); 


                grid.step_initial_condition(left_value,right_value,midpoint);

                
                  //////////////////////////CHECK/////////////
    }

};











#endif




















