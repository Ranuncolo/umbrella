//
//  umbrella.cpp
//
//

/* C++ code for umbrella - weather station on RPi.
 
 umbrella - weather station sensor are:
 - temperature sensor;
 - humidity sensor;
 - pressure sensor;
 - light sensor.
 
 They all follow I2C bus protocol where, according to https://pinout.xyz/pinout/i2c:
 PIN 1 - 3V3 Power;
 PIN 3 - BCM2 Data;
 PIN 5 - BCM3 Clock;
 PIN 6 - Ground.
 
 Data are sampled once every 15 minutes (value that can be easily changed), stored in a .txt file and finally shown online through web browser. The name of the file is the daily data: YY-MM-DD.
 
 +++ NEED TO CHOOSE HOW TO CHANGE THE DAY, IF AT 00.00?
 
 To compile, easily type: g++ umbrella.cpp -o <your_name>
 where "<your_name>" can be anything you wish.
 
 ex:
 $ g++ umbrella.cpp geti2c.cpp computation.cpp -o weather
 $ ./weather
 
 */
// ************************************************************************
// Libraries:

// general
#include <iostream> // for cout <<
#include <fstream> // for open/write/close file
#include <ctime> // output file name and timing in the file.
#include <cmath> // for NAN, pow

// for i2c +++ check if .h is required
#include <linux/types.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h> // for read()
#include <fcntl.h> // Required for O_RDWR
#include <bitset> // required for bit operation

//check if needed
//#include <stdint.h> //for things like int8_t, maybe not required anymore
//#include <stdio.h> // check, not required
//#include <stdlib.h> // conversion, maybe not useful. For atoi for example.


//+++ not sure if required by time stuff and so on.
//#include <cstring>
//#include <string>

#include "computation.hpp"
using namespace std;


// ************************************************************************
// Definition of constants:

static const char *device = "/dev/i2c-1"; //for our RPi, other may have 0

// Definition of each different address.
/* Every sensors have its own address. It is possible to retrieve it typing: $ sudo i2cdetect -y 1
 
 -y:  is just for avoiding a "are you sure you want to continue?"
 1 : is because we have a rev2 RPi, 0 for rev1 RPi.
 
 Each address is written in hex - notation.
 */

static int temp_add = 0x48;
static int hum_add = 0x27;
static int pres_add = 0xC0;
static int lux_add = 0x44;

//+++ Check, not all are correct.

int main()
{
    // Definition of values:
    double Temperature = NAN;
    double Humidity = NAN;
    double Pressure = NAN;
    double Light = NAN;
    // ********************************************************************
    // Creation of the output file:
    // File name is the current local time. It requires <ctime>.
    // More info at http://www.cplusplus.com/reference/ctime/strftime
    
    time_t moment;      //name of the chosen object, moment.
    struct tm *tpointer;
    char filename[20]; //it's not so long actually, but you never know
    time(&moment); // I'm giving in input the address of my obj moment
    tpointer = localtime(&moment); // And I store this in tpointer
    
    // Now I can "build up" the filename:
    strftime(filename,20,"%H_%M_%S.txt",tpointer);
    cout << "Data of this run are stored in " << filename << endl;
    
    // Now I create and open the output file.
    ofstream file;
    file.open(filename);
    
    // check:
    if (!file)
    {
        cout << "unable to open it." << endl;
        return 1;
    }
    
    // First output
    file << "umbrella - weather station." << endl;
    file << "Welcome!" << endl;
    file << endl;
    file << "------------------------------------------" << endl;
    file << endl;
    
    //  ********************************************************************
    // Header of the output file:
    
    cout << "Epoch\t Temp [C]\t Hum [%]\t Pres [Pa]\t Light [lux]" << endl;
    file << "Epoch\t Temp [C]\t Hum [%]\t Pres [Pa]\t Light [lux]" << endl;
    
    // Start the while loop:
    while(1) //+++ I LL HAVE TO CHOOSE A PROPER WHILE, WHILE THE DAY
    {
        char Epoch[18] = {};
        epoch(*Epoch);
        
        Temperature = calc_temp();
        Humidity = calc_hum();
        Pressure = calc_press();
        Light = calc_lux();
        
        // Writing in the output file:
        
        cout << Epoch   << "\t" <<  Temperature
                        << "\t" <<  Humidity
                        << "\t" <<  Pressure
                        << "\t" <<  Light
                        << endl;
        
        file << Epoch   << "\t" <<  Temperature
                        << "\t" <<  Humidity
                        << "\t" <<  Pressure
                        << "\t" <<  Light
                        << endl;
        
        
        // ****************************************************************
        // Wait 15 minutes
        int min = 15; // minutes you want to wait --> sample time
        int msec = min * 60 * 1000 * 1000;
        msec = 1000000; //+++
        usleep(msec); //usleep is in microsecond, so 900 x 10^6 microsec
        
        
        
    }
    
    file.close();
    return 0;
}
