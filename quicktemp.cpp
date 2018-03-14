//
//  quicktemp.cpp
//  
//
//  Created by René Gaio on 14/03/2018.
//

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>


static const char *device = "/dev/i2c-1";
static int address = 0x44;



int main()
{
    int file;
    
    uint8_t buffer[2];
    int16_t data;
    double temperature;
    
    // open i2c device
    if ((file = open(device, O_RDWR)) < 0)
    {
        cout << "Can't open i2c device!" << endl;
        return 1;
    }
    
    // Set the slave address
    
    if (ioctl(file, I2C_SLAVE, address) < 0 )
    {
        cout << "Can't reach the slave!" << endl;
        return 1;
    }
    // Read from the sensor
    cout << "Reading from the sensor" << address << "on" << device << endl;
    if (read(file,buffer,2) != 2)
    {
        cout << "Failer to read :(" << endl;
        return 1;
    }
    cout << buffer[0] << "aaaand" << buffer [1] << endl;
    
    
    
    close(file);
    return 0;
}
