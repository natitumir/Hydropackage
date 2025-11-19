
//This code has a class called ParameterInput. An object of this class can take an input file of .pin format and
//read the file and make a map( which is a key value pair thing) in which the keys and values from the .pin file is 
//stored. The manager will have a method called GenerateProblem() which will use this and work. 


#ifndef PARAMETER_INPUT_HPP
#define PARAMETER_INPUT_HPP


#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<stdio.h>

class ParameterInput {
    public:
        std::map<std::string, std::map<std::string, std::string>> parameters;

        void read_from_dotpin_file(const std::string filename){
            std::ifstream myfile ;
            myfile.open(filename);

            std::string line;
            std::string current_block;
            


            //The next loop will do some tasks 
            //1. It will read each line of the pin file
            //2. It will trim the line from all leading and trailing whitespace
            //3. If the line is just a comment or just an empty line, it will continue to the next iteration
            //4. If a line is a heading, then it will be inside the first slot of parameters.
            //5. If line has a = sign, then everything in the left goes to the key, that is second slot of parameters
            // and everything on the right goes to the value, that is the third slot of parameters
            
            while(getline(myfile,line)){
                std::string trimmed_line= trim(line); //current line is a new string variable taking the trimmed version of line
                if (trimmed_line[0]=='#'|| trimmed_line.empty()) continue; //continue simply takes out of the loop iteration
                                                                    //and goes to the next loop iteration

                                                                    //ANOTHER point to remember: single quote is for char
                                                                    //double quote is for strings
                if (trimmed_line[0]=='<') {
                    current_block= trimmed_line.substr(1, trimmed_line.length()-2);
                    continue; 
                }
                //here I divide the lines into two based on the position of the equal sign

                size_t pos_eq = trimmed_line.find('=');  //You might wonder what will happen if there is no = ? Well, nothing.
                                                          //Because the other possibilities are already taken care of
                std::string key= trimmed_line.substr(0,pos_eq);
                std::string value = trimmed_line.substr(pos_eq+1);

                parameters[current_block][trim(key)]=trim(value) ;

                
            }
            printf("left_value equals %s\n",parameters["initial_condition"]["left_value"].c_str());

        }
        /*
        double Getdouble(std::string block, std::string key) {
            return std::stod(parameters[block][key]);   //stod is the function that converts from string to double
        }
        int Getinteger(std::string block, std::string key) {
            return std::stoi(parameters[block][key]);     
        }
        std::string Getstring(std::string block, std::string key){
            return parameters[block][key];
        }*/
        double Getdouble(std::string block, std::string key) {

            


        std::string value = parameters[block][key];
        if (value.empty()) {
        throw std::invalid_argument("Empty value for: " + block + ":" + key);
         }
        
        return std::stod(value);
}

int Getinteger(std::string block, std::string key) {
    std::string value = parameters[block][key];
    if (value.empty()) {
        throw std::invalid_argument("Empty value for: " + block + ":" + key);
    }
    return std::stoi(parameters[block][key]);
}

std::string Getstring(std::string block, std::string key) {
    std::string value = parameters[block][key];
    if (value.empty()) {
        throw std::invalid_argument("Empty value for: " + block + ":" + key);
    }
    return value;
}


    private:
        //the following function will just trim any leading and trailing whitespace when reading the .pin file
        std::string trim(std::string str){
            size_t position_of_initial_content = str.find_first_not_of(" \t\r\n");
            size_t position_of_final_content = str.find_last_not_of(" \t\r\n");
            if (position_of_initial_content==std::string::npos){
                return "";  //This is returning an empty string
            }
            else {
                return str.substr(position_of_initial_content, position_of_final_content-position_of_initial_content+1);    
            }  
        }    
};




#endif