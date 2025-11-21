#ifndef EULER_PACKAGE_HPP
#define EULER_PACKAGE_HPP

#include<cmath>
#include<vector>
#include "Grid1D.hpp"
#include "Packages.hpp"
#include "Parameter_Input.hpp"


//===========This one will only hold stuff unique to Burgers equation



class EulerEquation : public Hydropackage {

    double gamma;

    void get_extra_parameters(ParameterInput& pin) override {
        gamma = pin.Getdouble("problem_type","gamma");
    }

    void convert_to_primitive (grid1d& grid) override {
        //Since there is a mix of all three variables in these transformations( in general), it is a good idea to create a temp and 
        //store grid.q into temp
        vector<vector<double>> temp ;
        temp.resize(grid.nvariable,vector<double>(grid.ntotal,0.0));
        temp= grid.q;
        for(int i=0; i<grid.ntotal; i++){
            grid.q[0][i]= temp[0][i];
            grid.q[1][i]= temp[1][i]/temp[0][i];
            grid.q[2][i] = (gamma-1)*(temp[2][i] - 0.5*((temp[1][i]*temp[1][i])/temp[0][i]));

        }
    }
    void convert_to_conserved (grid1d& grid) override {
        vector<vector<double>> temp ;
        temp.resize(grid.nvariable,vector<double>(grid.ntotal,0.0));
        temp= grid.q;
        for(int i=0; i<grid.ntotal; i++){
            grid.q[0][i]= temp[0][i] ;
            grid.q[1][i]= temp[0][i]*temp[1][i];
            grid.q[2][i] = (0.5*temp[0][i]* pow(temp[1][i],2) )+ (temp[2][i]/(gamma-1)) ;

        }

    }

    





    ////////////////////////////////////////////////////////////////////
    void flux_calculate(grid1d& grid) override {

        for(int i=0;i<=grid.ncell; i++){
            double q0= (grid.qL[0][i]+grid.qR[0][i])/2.0;
            double q1= (grid.qL[1][i]+grid.qR[1][i])/2.0;
            double q2= (grid.qL[2][i]+grid.qR[2][i])/2.0;

            double A[3][3];



            // Primitive variables
            double u = q1/q0;
            double p = (gamma - 1.0) * (q2 - 0.5*q1*q1/q0);
            double c = sqrt(gamma * p / q0);  // Speed of sound
    
    // Eigenvalues
            
            
            double lambda[3];
            lambda[0] = u - c;  // Corresponds to v1 (left-moving acoustic wave)
            lambda[1] = u + c;  // Corresponds to v2 (right-moving acoustic wave)
            lambda[2] = u;      // Corresponds to v3 (entropy/contact wave)

            
            


            double sqrt_term = sqrt(-2.0*(gamma*gamma - gamma)*q1*q1 + 4.0*(gamma*gamma - gamma)*q0*q2);

            A[0][0] = 1.0;
            A[1][0] = 0.5*(2.0*q1 - sqrt_term)/q0;
            A[2][0] = -0.5*((gamma - 1.0)*q1*q1 - 2.0*gamma*q0*q2 + sqrt_term*q1)/(q0*q0);
    
    // Second eigenvector (associated with λ = u + c)
            A[0][1] = 1.0;
            A[1][1] = 0.5*(2.0*q1 + sqrt_term)/q0;
            A[2][1] = -0.5*((gamma - 1.0)*q1*q1 - 2.0*gamma*q0*q2 - sqrt_term*q1)/(q0*q0);
    
    // Third eigenvector (associated with λ = u)
            A[0][2] = 1.0;
            A[1][2] = q1/q0;
            A[2][2] = 0.5*q1*q1/(q0*q0);
    
            double b[3] = {grid.qR[0][i]-grid.qL[0][i], grid.qR[1][i]-grid.qL[1][i], grid.qR[2][i]-grid.qL[2][i]};
            double w[3];

            if (cramers_rule(A, b, w)) {
                double partb[3];
                partb[0]= abs(lambda[0])*w[0]*A[0][0]+abs(lambda[1])*w[1]*A[0][1]+abs(lambda[2])*w[2]*A[0][2];
                partb[1]= abs(lambda[0])*w[0]*A[1][0]+abs(lambda[1])*w[1]*A[1][1]+abs(lambda[2])*w[2]*A[1][2];
                partb[2]= abs(lambda[0])*w[0]*A[2][0]+abs(lambda[1])*w[1]*A[2][1]+abs(lambda[2])*w[2]*A[2][2];
                
                grid.numericflux[0][i]= 0.5*(flux(grid.qL[0][i],grid.qL[1][i],grid.qL[2][i],gamma)[0]+flux(grid.qR[0][i],grid.qR[1][i],grid.qR[2][i],gamma)[0]-partb[0]);
                grid.numericflux[1][i]= 0.5*(flux(grid.qL[0][i],grid.qL[1][i],grid.qL[2][i],gamma)[1]+flux(grid.qR[0][i],grid.qR[1][i],grid.qR[2][i],gamma)[1]-partb[1]);
                grid.numericflux[2][i]= 0.5*(flux(grid.qL[0][i],grid.qL[1][i],grid.qL[2][i],gamma)[2]+flux(grid.qR[0][i],grid.qR[1][i],grid.qR[2][i],gamma)[2]-partb[2]);
            }
        }

        
    }


    ///////////////////////////////////////////////////////////////


    void step_initial_condition(grid1d& grid, ParameterInput& pin) override {
                
                vector<double> left_value(3,0.0);
                vector<double> right_value(3,0.0);
                double  midpoint;
                


                left_value[0]=pin.Getdouble("initial_condition","rho_l");
                right_value[0] = pin.Getdouble("initial_condition","rho_r");
                left_value[1]=pin.Getdouble("initial_condition","u_l");
                right_value[1] = pin.Getdouble("initial_condition","u_r");
                left_value[2]=pin.Getdouble("initial_condition","p_l");
                right_value[2] = pin.Getdouble("initial_condition","p_r");
                midpoint = pin.Getdouble("initial_condition","midpoint"); 


                grid.step_initial_condition(left_value,right_value,midpoint);

                
                  //////////////////////////CHECK/////////////
    }


    double cfl(grid1d& grid) override { //LATER
      //  double dt;
      //  vector<double> absolute_q ;
      //  absolute_q.resize(grid.ntotal,0.0);
     //   for(int i=0;i<grid.ntotal;i++){
     //       absolute_q[i] = abs(grid.q[0][i]);    //Just a reminder, in this case there is only one variable.
     //   }

       // dt= 0.3*grid.dx/( Hydropackage::FindLargest(absolute_q));
       double dt=0.1;
        return dt;
    }








    /////////////////////////////////////////////////////////////////////////////////////
    private:
    // Function to calculate 3x3 determinant
        double det3x3(double a11, double a12, double a13,
              double a21, double a22, double a23,
              double a31, double a32, double a33) {
            return a11 * (a22 * a33 - a23 * a32)
            - a12 * (a21 * a33 - a23 * a31)
            + a13 * (a21 * a32 - a22 * a31);
        }

// Solve system using Cramer's rule
// Input: coefficient matrix A[3][3] and right-hand side b[3]
// Output: solution x[3]
// Returns true if successful, false if determinant is zero
        bool cramers_rule(const double A[3][3], const double b[3], double x[3]) {
    // Calculate determinant of coefficient matrix
            double det_A = det3x3(A[0][0], A[0][1], A[0][2],
                          A[1][0], A[1][1], A[1][2],
                          A[2][0], A[2][1], A[2][2]);
    
    // Check if system has unique solution
         //   if (std::abs(det_A) < 1e-10) {
          //      std::cerr << "Error: Determinant is zero, no unique solution\n";
          //      return false;
         //   }
    
    // Calculate x[0] - replace first column with b
            double det_A1 = det3x3(b[0],    A[0][1], A[0][2],
                           b[1],    A[1][1], A[1][2],
                           b[2],    A[2][1], A[2][2]);
            x[0] = det_A1 / det_A;
    
    // Calculate x[1] - replace second column with b
            double det_A2 = det3x3(A[0][0], b[0],    A[0][2],
                           A[1][0], b[1],    A[1][2],
                           A[2][0], b[2],    A[2][2]);
            x[1] = det_A2 / det_A;
    
    // Calculate x[2] - replace third column with b
            double det_A3 = det3x3(A[0][0], A[0][1], b[0],
                           A[1][0], A[1][1], b[1],
                           A[2][0], A[2][1], b[2]);
            x[2] = det_A3 / det_A;
    
            return true;
        }






        //////////////////Function to compute fluxes////////////////////////////////
        // Compute fluxes from conserved variables and return as vector
        vector<double> flux(double q0, double q1, double q2, double gamma) {
            vector<double> f(3);
    
    // Flux 1 (mass)
            f[0] = q1;
    
    // Flux 2 (momentum)
            double p = (gamma - 1.0) * (q2 - 0.5*q1*q1/q0);  // pressure
            f[1] = q1*q1/q0 + p;
    
    // Flux 3 (energy)
            f[2] = (q1/q0) * (q2 + p);
    
            return f;
        }
   


};











#endif




















