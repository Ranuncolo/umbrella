//
//  geti2c.hpp
//  
//
//  Created by René Gaio on 14/03/2018.
//
//
//  Definition of the geti2c class

#ifndef geti2c_hpp
#define geti2c_hpp


// ******************************************************************
// Libraries:

// general
#include <iostream> // for cout <<
#include <fstream> // for open/write/close file
#include <ctime> // output file name and timing in the file.
#include <cmath> // for NAN, pow

// for i2c
#include <linux/types.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h> // for read()
#include <fcntl.h> // Required for O_RDWR
#include <bitset> // required for bit operation

using namespace std;

// ******************************************************************

class geti2c
{
public:
    geti2c(string _name, int _add, int _nbyte); //constructor
    ~geti2c(void); //destructor
    int add_register();
    int read_register(unsigned char &buffer);
    int error();
private:
    int openI2C(); //open the device, in constructor
    int closeI2C(); //in destructor
    string name;
    int address=0;
    int nbyte=0;
    static const char device; //it will be "/dev/i2c-1" for us
    int i2cDescriptor=-1;

};



#endif /* geti2c_hpp */
