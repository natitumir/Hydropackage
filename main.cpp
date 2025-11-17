#include<iostream>
#include "Manager.hpp"
using namespace std;

int main() {
    cout<<"Hello World, this is my first Mac program"<<endl;
    Manager mymanager("Burgers_shocktube.pin");
    mymanager.generate_problem();
    return 0;
}