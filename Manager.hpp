#ifndef MANAGER_HPP
#define MANAGER_HPP


#include<iostream>
#include<fstream>


#include "Parameter_Input.hpp"

class Manager 
{
    public:
        std::string pinfilename;
        Manager(std::string pinfilename_){
            pinfilename= pinfilename_;
        }
        void generate_problem() {
            ParameterInput pin ;
            pin.read_from_dotpin_file(pinfilename); //The pin object will read everything from the said file
                                                                //and make a map object that includes all these data
            std::cout<<"In the problem defined, the mesh has "<<pin.Getdouble("mesh","nx") << "number of 1D mesh blocks"<<std::endl;
        }

};










#endif