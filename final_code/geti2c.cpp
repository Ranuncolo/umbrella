//
//  geti2c.cpp
//
//
//  Created by René Gaio on 14/03/2018.
//

/*
 geti2c is the C++ class whose instances address each one of the different sensors. The costructor has a string name for the sensor, its address (in hex) and the number of byte that it has to read.
 
 The two main member functions are:
 - ioctl(): reach the sensor at the proper address and register;
 - read(): read the binary digital data and store it in a proper buffer.
 */
 

#include "geti2c.hpp"

using namespace std;


// ******************************************************************

// Constructor:
geti2c::geti2c(string _name, int _add, int _nbyte) // int _des
{
    name = _name;
    static const char *device = "/dev/i2c-1";
    address = _add;
    nbyte = _nbyte;
    //cout << "Open I2C Device" << endl;
    openI2C();
}


// ******************************************************************

// Destructor
geti2c::~geti2c()
{
    closeI2C();
}


// ******************************************************************

// Definition of open
int geti2c::openI2C()
{
    i2cDescriptor = open(device, O_RDWR);
    
    if (i2cDescriptor<0)
    {
        cout << "Could not open I2C device!" << endl;
    }
    
    return i2cDescriptor;
}

/* open the I2C pathname given. It returns a positive descriptor if succeeded.
 More info at: http://man7.org/linux/man-pages/man2/open.2.html
 */


// ******************************************************************

// Definition of close
int geti2c::closeI2C()
{
    close(i2cDescriptor);
}


// ******************************************************************

// Definition of the proper address

int geti2c::add_register()
    
{
    int ioctl_value = -1;
    ioctl_value = ioctl(i2cDescriptor, I2C_SLAVE, address);
    if(ioctl_value < 0)
    {
        cout << "Can't reach the " << name << " sensor!" << endl;
    }
    return ioctl_value;
    // Further info at: http://man7.org/linux/man-pages/man2/ioctl.2.html
}



// ******************************************************************

// Definition of read_register
int geti2c::read_register(unsigned char &buffer)
{
    int read_value = -1;
    unsigned char *buff;
    buff = &buffer;
    read_value = read(i2cDescriptor, buff, nbyte);
    if(read_value<0)
   {
        cout << "Failed to read the " << name << " data!" << endl;
    }
    return read_value;
    /* The read function reads for the number of bytes written in its third argument and store it in the buffer (unsigned char) defined at the beginning. It returns the number of bytes read, so it is possible to check it.
     Further info at: http://man7.org/linux/man-pages/man2/read.2.html
     */
}

