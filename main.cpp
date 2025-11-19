#include<iostream>
#include "Manager.hpp"
using namespace std;


//It would be a nice way to try the following. Right now I am not very familiar with the Makefile thing to do this so let me just use an elementary way.

/*int main(int argc, char* argv[] ) {       //Here argc is an integer that has the number of command line arugmetents. for example ""./program burgers.pin"
                                         // has 2 arguements. these are then put into a char type list argv[].
   cout<<"Hello World, this is my first hydro package."<<endl;
   string pinfilename= argv[1]; //In our implementation, the second argument is the .pin file's name.
   Manager mymanager(pinfilename);
   mymanager.generate_problem();
   return 0;
} */



int main(){

    int which;
    cout<<"This is a program to do a hydro simulation. There are a few options for the files that you can run."<<endl;
    cout<<"The follwoing are the available options.Please write which one you want."<<endl;
    cout<<"1) Burgers_step.pin : This one is for Burgers equation with a step function initial condition "<<endl;

    cin>>which;

    Manager mymanager("");  // Declare first with dummy filename
    
    if(which == 1) {
        mymanager = Manager("Burgers_step.pin");  // Reassign
    }
    //More if clauses for other pin files

    mymanager.generate_problem();
    
    mymanager.setup_initial_condition();

    cout<<"initial condition done"<<endl;

    mymanager.timelooping();
    cout<<"Looping is done"<<endl;

    return 0;
}