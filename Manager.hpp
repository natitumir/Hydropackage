#ifndef MANAGER_HPP
#define MANAGER_HPP
#include<iostream>
#include<fstream>
#include "Parameter_Input.hpp"
#include "Grid1D.hpp"
#include "Burgers_Package.hpp"
#include "Euler_Package.hpp"
#include "Packages.hpp"





class Manager {
    public:
        std::ofstream file;
        
        std::unique_ptr<Hydropackage> hydro;  // Polymorphic hydro package=====================
        //=============================================================================================
        ParameterInput pin ;
        grid1d grid;
        BurgersEquation Burger;
        EulerEquation Euler ;//Add all the other individual packages as you see the need;
        std::string pinfilename;
        std::string problem_type, initial_condition_type, bc, reconstruction_method;
        int ncell, nghost, nvariable;
        double xi,xf,ti,tf;
        
        
        
        //============Constructor=========
        Manager(std::string pinfilename_){
            pinfilename= pinfilename_;
            file.open("data.txt");
        }


        void generate_problem() {
            pin.read_from_dotpin_file(pinfilename); //The pin object will read everything from the said file
                                                                //and make a map object that includes all these data
            //Now is the part where we read the map created by pin and extract different parameters from it. 
            //==================================================================================
            //problem type
            problem_type = pin.Getstring("problem_type","name");
            nvariable = pin.Getinteger("problem_type","nvariable");



            //Initial condition type
            initial_condition_type=pin.Getstring("initial_condition","type_of_initial_condition");
            
            
            //mesh characteristics
            ncell = pin.Getinteger("mesh","ncell");
            xi= pin.Getdouble("mesh","xi");
            xf= pin.Getdouble("mesh","xf");

            //time loop characteristics
            ti= pin.Getdouble("timeloop","ti");
            tf= pin.Getdouble("timeloop","tf");
            
        
            //ghost and boundary characteristics
            bc = pin.Getstring("boundary_condition","type_of_boundary_condition");
            nghost = pin.Getinteger("boundary_condition","nghost");
            //simulation methodology
            reconstruction_method = pin.Getstring("method_of_simulation","reconstruction_method");











            //===============================================================
            //===============================================================
             // **KEY PART: Create the appropriate hydro package based on user choice**
        
        
            if (problem_type == "burgers_equation") {
                hydro = std::make_unique<BurgersEquation>();
                std::cout << "Using Burgers_Package.hpp" << std::endl;
            }
            if (problem_type == "eulers_equation") {
                hydro = std::make_unique<EulerEquation>();
               std::cout << "Using Euler package" << std::endl;
            }
            

            //=================PRINT OUT THE PROBLEM====================

            std::cout << "=== PROBLEM CHARACTERISTICS ===" << std::endl;
            std::cout << "Input file: " << pinfilename << std::endl;
            std::cout << "Problem type: " << problem_type << std::endl;
            std::cout << "Mesh: " << ncell << " cells from " << xi << " to " << xf << std::endl;
            std::cout << "Initial condition: " << initial_condition_type;
            std::cout << std::endl;
            std::cout << "Boundary condition: " << bc << " with " << nghost << " ghost cells" << std::endl;
            std::cout << "Reconstruction method: " << reconstruction_method << std::endl;
            std::cout << "=================================" << std::endl;
        }
        
        
        
        //SETUP the initial conditions====================================================================
        //Will also write down the values in a file=======================================================
        
        void setup_initial_condition(){

            grid.get_grid_parameters(xi,xf,ncell,nghost,nvariable);
            
            if(initial_condition_type=="step"){
                hydro->step_initial_condition(grid,pin);
                (grid.*grid1d::write_ghosts[bc])();
                
            }
            //FOR NOT STEP, USE OTHER METHODS FROM THE BASE HYDRO CLASS
            
             //Up to now the mesh has the variable q filled, including the ghost cells.But they
            //are all in the primitive form. So I need to take an output in the output file and then convert to conserved 
            //before starting the loop.

            std::cout<< "Hhhhhhhhhhhhhhhhhh"<<grid.q[0][0]<<std::endl;

            for(int i = nghost; i <=ncell+nghost-1; i++) {
                file << ti<<" "<< grid.x[i] << " ";           // Write x coordinate
                for(int j = 0; j<nvariable; j++) {
                    file << grid.q[j][i] << " ";    // Write all variables for this x
                }
                file << std::endl;  //  THIS moves to next line for next spatial point
            }

            cout<<"huuuuuuuuu"<<std::endl;

            



        
        }

        void timelooping(){
            double t=ti;

            while(t<tf){



                


                //Now I need to find out the value of dt. For that I need the CFL condition, which says that the time
                //step must be samller than dx/(max charactersitic velocitiy). Since the characteristic velocities depend on the 
                //kind of equation we are using, I will make CFL functions on each of the individual equation packages. I will use the
                //cell center values right after the update to calculate dt through cfl condition. 

                double dt;
                dt= hydro->cfl(grid);


                





                hydro->convert_to_conserved(grid); //Back to conservative before the next loop iteration

                //For RK2, I need to hold the values of q at time step n.
                grid1d temp;
                temp.get_grid_parameters(xi,xf,ncell,nghost,nvariable);
                temp.q = grid.q;

                hydro->minmod_reconstruction(grid);
                hydro->flux_calculate(grid);


                

                //Then I update to time step n+1/2
                for(int j=0;j<nvariable;j++){
                    for(int i=nghost; i<=ncell+nghost-1;i++){
                       grid.q[j][i]= grid.q[j][i] + (dt/2.0)*((grid.numericflux[j][i-nghost]-grid.numericflux[j][i-nghost+1])/grid.dx); 
                    }
                }
                //apply the bc
                (grid.*grid1d::write_ghosts[bc])();



           
        

                //Once I have the t^n+1/2 varialbes, I will reconstruct them and then find the Fluxes,

                hydro->minmod_reconstruction(grid);
                hydro->flux_calculate(grid);

                //Using the fluxes calculated at t^n+1/2, I now update the q from t^n to t^n+1, albeit using temp instead of grid, because at this 
                //coding step, q has values in the t^n+1/2 time step.


                for(int j=0;j<nvariable;j++){
                    for(int i=nghost; i<=ncell+nghost-1;i++){
                        grid.q[j][i]= temp.q[j][i] + (dt)*((grid.numericflux[j][i-nghost]-grid.numericflux[j][i-nghost+1])/grid.dx); 
                    }
                }
                //apply the bc
                (grid.*grid1d::write_ghosts[bc])();

                
                //Now I first update the time and then convert to primitive, then write the output file and then go back to conserved
                t=t+dt;
                hydro->convert_to_primitive(grid);
                for(int i = nghost; i <=ncell+nghost-1; i++) {
                    file << t<<" "<< grid.x[i] << " ";           // Write x coordinate
                    for(int j = 0; j<nvariable; j++) {
                        file << grid.q[j][i] << " ";    // Write all variables for this x
                    }
                file << std::endl;  //  THIS moves to next line for next spatial point
                }
                

                





            }
        }






















    };






#endif