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
 $ g++ umbrella.cpp -o weather
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
#include <linux/i2c-dev>
#include <ioctl.h>
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
static int hum_add = 0x21;
static int pres_add = 0xC0;
static int lux_add = 0x44;

//+++ Check, only the temps one is correct.

int main()
{
    
    // ********************************************************************
    // Definition of variables required for I2C data retrieving:
    
    int fd, Tfd, Hfd, Pfd, Lfd;
    /* Those intigers are required for the ioctl function, they are called "file descriptors." Tfd, Hfd, Pfd and Lfd are respectively the file descriptor for the temperature, humidity, pressure and light sensor. fd is the general first descriptor for I2C bus in general.
     Further info at: http://man7.org/linux/man-pages/man2/ioctl.2.html
     */
    
    unsigned char T_bit[2];
    unsigned char H_bit[2];
    unsigned char P_bit[2];
    unsigned char L_bit[2];
    /* Those are arrays of char which will store the digital binary data for each sensor respectively.
     +++ check their length. Maybe not all of them have this length.
     */
    
    double Temperature;
    double Humidity;
    double Pressure;
    double Light;
    //+++ maybe it would be better to initialise them, I would like NaN.
    
    
    
    // ********************************************************************
    // Creation of the file:
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
    
    // ********************************************************************
    // Searching for sensor:
    cout << "Searching for sensors:" << endl;
    
    fd = open(device, O_RDWR);
    /* open the I2C pathname given. It returns a positive descriptor if succeeded.
     More info at: http://man7.org/linux/man-pages/man2/open.2.html
     */
    
    if (fd < 0)
    {
        cout << "Could not open I2C device!" << endl;
        return 1;
    }
    
    
    // I set all the other descriptors equal to fd. This would not be necessary, but I can keep each sensor with its own prefix.
    //+++ check, probably fd is the number of sensors read. Not sure.
    Tfd = fd;
    Hfd = fd;
    Pfd = fd;
    Lfd = fd;
    
    //+++ should be nice to print how many and which sensors we have.
    
    
    
    // ********************************************************************
    // Set the slave address for each sensor + status check:
    
    if (ioctl(Tfd, I2C_SLAVE, temp_add) < 0 )
    {
        cout << "Can't reach the temperature sensor!" << endl;
        Temperature = NAN;
    }
    
    if (ioctl(Hfd, I2C_SLAVE, hum_add) < 0 )
    {
        cout << "Can't reach the humidity sensor!" << endl;
        Humidity = NAN;
    }
    
    if (ioctl(Pfd, I2C_SLAVE, pres_add) < 0 )
    {
        cout << "Can't reach the pressure sensor!" << endl;
        Pressure = NAN;
    }
    
    if (ioctl(Lfd, I2C_SLAVE, lux_add) < 0 )
    {
        cout << "Can't reach the light sensor!" << endl;
        Light = NAN;
    }
    
    
    // ********************************************************************
    // Header of the output file:
    
    cout << "Epoch\t Temp [C]\t Hum [%]\t Pres [Pa]\t Light [lux]" << endl;
    file << "Epoch\t Temp [C]\t Hum [%]\t Pres [Pa]\t Light [lux]" << endl;
    
    //+++ CHECK PRESSURE AND LIGHT UNITS!!!!
    
    
    
    // Start the while loop:
    while(1) //+++ I LL HAVE TO CHOOSE A PROPER WHILE, WHILE THE DAY
    {
        
        // ****************************************************************
        // Read from the sensors + status check:
        /* The read function reads for the number of bytes written in its third argument and store it in the buffer (unsigned char) defined at the beginning. It returns the number of bytes read, so it is possible to check it.
         Further info at: http://man7.org/linux/man-pages/man2/read.2.html
         */
        
        
        if (read(Tfd,T_bit,2) != 2)
        {
            cout << "Failed to read temperature data." << endl;
            Temperature = NAN;
        }
        
        if (read(Hfd,H_bit,2) != 2)
        {
            cout << "Failed to read humidity data." << endl;
            Humidity = NAN;
        }
        
        if (read(Pfd,P_bit,2) != 2)
        {
            cout << "Failed to read pressure data." << endl;
            Pressure = NAN;
        }
        
        if (read(Lfd,L_bit,2) != 2)
        {
            cout << "Failed to read light data." << endl;
            Light  = NAN;
        }
        
        // TEMPERATURE:
        // concatenate the two byte inTO a 16 bit binary number:
        std::bitset<16> Tcombo;
        std::bitset<16>Tb1(T_bit[0]);
        std::bitset<16>Tb2(T_bit[1]);
        Tcombo = (Tb1 << 8) | Tb2;
        
        // last for 4 are just empty and they are not part of the number:
        Tcombo = Tcombo >> 4;
        // cast to int
        int Temp;
        Temp = (int)(combo.to_ulong());
        // following the conversion on the data sheet, pag 5, set the difference between negative and positive temperatures, then convert to T [C].
        // to convert, the data must be multiplied by 0.0625, the sensitivity (1/16)
        if (Temp > 2047)
        {
            Temp = Temp - 4096;
        }
        Temperature = Temp * 0.0625;
    
        
        // HUMIDITY:
        std::bitset<16> Hcombo;
        std::bitset<16>Hb1(H_bit[0]);
        std::bitset<16>Hb2(H_bit[1]);
        Hcombo = (Hb1 << 8) | Hb2;
        
        /* The first two bits are not part of the number, I must be sure they are zero. To do so, I first create a 14 bits binary number, called "bit14", full of 1 using the "set" function, then using the bitwise operation AND (&) I am sure I will have a 14 bit number.
         
         Further info at http://www.cplusplus.com/reference/bitset/bitset/set/
         */
        std::bitset<16> bit14;
        bit14.set();
        
        /* At the moment its, a 16bits number full of 1, 0b1111111111111111. To set to zero the first two bits I use again the set function. This function starts to count from right to left, from zero, so the 14th and 15th entry are the two bits I want to be zero. */
        
        bit14.set(14,0);
        bit14.set(15,0); //0b0011111111111111
        
        // Perform the & operation
        Hcombo = (Hcombo & bit14);
    
        // In this way I am sure that it is a 14bit number, and I can convert into humidity through the equation written at page 6 of the data sheet.
        
        double denominator = 16382; // 2^14 - 2. Double for c++ reasons or it will not convert Hum into a double.
        int Hum = (int)(Hcombo.to_ulong()); // cast into intiger
        Humidity = Hum / denominator * 100;
        
        
        // PRESSURE:
        //+++ on going
        
        
        // LIGHT:
        /* It has two component accordin to the data sheet, the first 4 bit are in the number Ebit, the latter 12 in the number Rbit.
         */
        
        // Definition of the two components:
        std::bitset<8>Ebit;
        std::bitset<16>Rbit;
        
        // Ebit:
        // set the first bit in a byte callet Lbit4, and the bit4 (11110000):
        std::bitset<8> Lbit4(L_bit[0]);
        std::bitset<8> bit4(string("11110000"));
        
        // AND operation e shift operator (I want 0000XXXX):
        Ebit = Lbit4 & bit4;
        Ebit = (Ebit >> 4);
        
        // Rbit:
        // This part is similar to the others, required for Rbit.
        std::bitset<16> Lcombo;
        std::bitset<16>Lb1(L_bit[0]);
        std::bitset<16>Lb2(L_bit[1]);
        Lcombo = (Lb1 << 8) | Lb2;
        
        std::bitset<16> bit12(string("0000111111111111"));
        Rbit = Lcombo & bit12;
        
        // Now I combine them according to the data sheet, pag 20:
        int E = (int)(Ebit.to_ulong());
        int R = (int)(Rbit.to_ulong());
        Light = 0.01 * pow(2.0,E) * R;
        
        
        // ****************************************************************
        // Epoch, the time of the actual data read:
        
        time_t data_epoch;      //name of the chosen object, epoch.
        struct tm *tepoch;
        char Epoch[10];
        time(&data_epoch);
        tepoch = localtime(&data_epoch); //
        strftime(Epoch,10,"%F %H:%M",tepoch);
        //+++ remember to change as Dan told you
        
        // ****************************************************************
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
        int msec = 15 * 60 * 1000 * 1000;
        usleep(msec); //usleep is in microsecond, so 900 x 10^6 microsec
        
        
        
        
        
        
        close(fd);
    } //end of while - sampling.
    
    
    
    
    
    
    
    // close the output file  +++ MAYBE IF END OF THE DAY
    file.close();
    return 0;
    
}
