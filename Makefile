#using variables in a makefile is not necessary but it can make it easier to maintain 
#and update the makefile. Here is an example of how you can use variables in a makefile:

#All variables must be defined in uppercase letters by convention, 
#but this is not a strict requirement. 

CXX = g++  # This variable holds the name of the C++ compiler
CXXFLAGS = -Wall  # This variable holds the compiler flags. Wall is for all warnings and Wextra is for extra warnings.
                          
TARGET = program  # This variable holds the name of the target executable

SOURCES = main.cpp
HEADERS = Manager.hpp Parameter_Input.hpp

# Notice how variables are used here, with
#the dollar sign and parantheses.

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean: 
	rm -f $(TARGET)	


 