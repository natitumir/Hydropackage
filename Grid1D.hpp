#ifndef GRID1D_HPP
#define GRID1D_HPP


#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<map>


using namespace std;


//================================================================
//This is the 1D grid that holds the data along the space axis x
//================================================================


class grid1d {
    public:
        //Grid parameters
        double xi, xf;
        double dx;
        int ncell;
        int nghost;
        int ntotal;
        int nvariable;   //How many dependnet variables are there in the problem. e.g in 1D Burgers there is 
                         //just one variable
    
        
        vector<double> x ; //cell center positions

        vector<vector<double>> q ; //the main vector of conserved variables. q[variable_index][cell_index]

        vector<vector<double>> qL ; //Reconstructed variables on the left side of each cell boundaries

        vector<vector<double>> qR; //on the right side of each cell boundaries

        vector<vector<double>> numericflux ; //Numerical fluxes at each cell boundary


        



        //==========================================================================
        //======Grid paramters=========================================================

        void get_grid_parameters(double xi_, double xf_, int ncell_, int nghost_, int nvariable_){
            xi= xi_;
            xf= xf_;
            ncell= ncell_;
            nghost= nghost_;
            ntotal= ncell+ nghost*2;
            dx= (xf-xi)/(double)ncell;
            nvariable= nvariable_;
        

            //Initializing all of the vectors(1d and 2d) with zeros

            x.resize(ntotal, 0.0);
            q.resize(nvariable,vector<double>(ntotal,0.0));
            qL.resize(nvariable,vector<double>(ncell+1,0.0));
            qR.resize(nvariable,vector<double>(ncell+1,0.0));
            numericflux.resize(nvariable,vector<double>(ncell+1,0.0));




            //cell center positions
            for(int i=0; i<ntotal; i++){
                x[i]= xi+ (i+0.5)*dx ;
            }





        }




        //=============================
        //INITIAL CONDITION INPUT======
        //Inside the next few functions, there will be options of taking different initial conditions.
        //Right now, I only have the step condition. I will again use a technique similar to the case of Boundary conditions.
        //See the notes with the boundary conditions to understand fully.


        //STEP initial condition. 
        void step_initial_condition(vector<double> leftside, vector<double> rightside, double mid){ //Leftside is a vector that has the left
                                                                                          // side values of each variable
            
            for (int i = 0; i < nvariable; i++)
            {
                for (int j = 0; j < ntotal; j++)
                {
                    if (x[j] < mid)
                    q[i][j] = leftside[i];
                    else
                    q[i][j] = rightside[i];
                }
            }
        }


            
        
        //ADD OTHER INITIAL CONDITION GENERATORS

        


        //================Boundary conditioning=====================
        //Here I will include 3 types of Boundary conditions. Right now I only have outflow.
        //THE technique here is to make a map where I will take key value pairs of {name,pointer to boundary condition method}
        //In manager, when I use the boundary condition writeup, I use whatever is needed based on the user input through .pin file

        void outflow_bc(){
            for(int i=0; i<nvariable;i++){
                for(int j=0;j<nghost;j++){
                    q[i][j]=q[i][j+nghost];
                }
                for(int k=ncell+nghost;k<=(ncell+2*nghost-1);k++){
                     q[i][k]=q[i][k-nghost];
                }
            }
            
            //ADD REFLECTIVE and PERIODIC

        }
        void reflective_bc(){
            for(int i=0; i<nvariable;i++){
                for(int j=0;j<nghost;j++){
                    q[i][j]=-q[i][j+nghost];
                }
                for(int k=ncell+nghost;k<=(ncell+2*nghost-1);k++){
                     q[i][k]=-q[i][k-nghost];
                }
            }
            
            //=============BE SURE IF THIS IS REFLECIVE

        }

        static map<string, void (grid1d::*)()> write_ghosts; //This is a map with the values being pointers to methods of this class. As a 
        //static member it must be defined outside the class. However, REMEMBER THIS. Each time I add a new boundary condition, I must update
        //this map to keep the logical flow of the whole program.

        







};

map<string, void (grid1d::*) ()>  grid1d::write_ghosts = {
        {"outflow", &grid1d::outflow_bc},
        {"reflective", &grid1d::reflective_bc},
        {"periodic", &grid1d::outflow_bc}};   //For now just remember that this is how it is done. Next week you can chat with AI to know more
        //details about this code ====================================================================================





#endif



//==============================possible errors, the map of string function pointer thing=========either here or in manager