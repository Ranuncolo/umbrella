/***********************************************************************
 *
  Program to calculate the rotational velocity of an anemometer with arm length 7cm.
  Anemometer consists of a magnetic reed switch and rotating propellers which carry a magnet
  that trips the reed switch twice per 360 degree rotation.

   mcp3008SpiTest.cpp. Sample program that tests the mcp3008Spi class.
 * an mcp3008Spi class object (a2d) is created. the a2d object is instantiated
 * using the overloaded constructor. which opens the spidev0.0 device with
 * SPI_MODE_0 (MODE 0) (defined in linux/spi/spidev.h), speed = 1MHz &
 * bitsPerWord=8.
 *
 * call the spiWriteRead function on the a2d object 20 times. Each time make sure
 * that conversion is configured for single ended conversion on CH0
 * i.e. transmit ->  byte1 = 0b00000001 (start bit)
 *                   byte2 = 0b1000000  (SGL/DIF = 1, D2=D1=D0=0)
 *                   byte3 = 0b00000000  (Don't care)
 *      receive  ->  byte1 = junk
 *                   byte2 = junk + b8 + b9
 *                   byte3 = b7 - b0
 *    
 * after conversion must merge data[1] and data[2] to get final result
 *
 *
 *
 * *********************************************************************/
#include "WindSpi.h"
#include <cstdio>
#include <ctime>
#include <iostream>
#include <fstream>
 
using namespace std;
 
int main(void)
{
    WindSpi a2d("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);
    	int i = 25000;
	int ii = 25000;
        int a2dVal = 0;
        int a2dChannel = 7;
        unsigned char data[3];
 	int a2dVal_new = 0;
	int a2dVal_old = 0;
	double highs = 0.0;
	double vel;

	 std::clock_t start;
    double duration;


	time_t moment;      //name of the chosen object, moment.
    struct tm *tpointer;
    char filename[20]; //it's not so long actually, but you never know
    time(&moment); // I'm giving in input the address of my obj moment
    tpointer = localtime(&moment); // And I store this in tpointer
    
    // Now I can "build up" the filename:
	strftime(filename,20,"%H_%M_%S.txt",tpointer);
	ofstream file;
	file.open(filename);
    
    // check:
    if (!file)
    {
        cout << "unable to open it." << endl;
        return 0;
    }
    
    // First output
    file << "Long time no see :)" << endl;
	file << endl;

    start = std::clock();	//start clock

    while(i > 0)
    {
	// SPI
        data[0] = 1;  //  first byte transmitted -> start bit
        data[1] = 0b10000000 |( ((a2dChannel & 7) << 4)); // second byte transmitted -> (SGL/DIF = 1, ODD/SIGN = 1 )
        data[2] = 0; // third byte transmitted....don't care
 
        a2d.spiWriteRead(data, sizeof(data) );
 
        a2dVal = 0;
                a2dVal = (data[1]<< 8) & 0b1100000000; //merge data[1] & data[2] to get result
                a2dVal |=  (data[2] & 0xff);


	a2dVal_new = a2dVal;	//update value
	if( (a2dVal_new > 800) && (a2dVal_old < 800)){	//check for initial high value
	highs = highs + 1.0;}		//count high values
	
        i--;
	
	a2dVal_old = a2dVal_new; 	// update old value for next iteration
    }
	 duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;		//check time duration
	if(highs == 1){
	vel = 0;
	} else {
	vel = 3.60*2.00*3.14*0.07*highs/(2.00*duration); // calculate velocity: vel = 3.6km/hr * 2pi * 7 cm * (number of high values/2)/(measurement time)
	}
	 cout << "Wind Speed is: " << vel  << " km/hr" <<  endl;
	 cout << "duration: " << duration <<  endl;
         cout << "highs: " << highs <<  endl;

	time_t epoch;      //name of the chosen object, moment.
        struct tm *tepoch;
        char times[10]; //it's not so long actually, but you never know
        time(&epoch); // I'm giving in input the address of my obj moment
        tepoch = localtime(&epoch); // And I store this in tpointe
        strftime(times,10,"%H:%M:%S",tepoch);
	

	file << "Epoch" << endl;
	file << times << "\t" << endl;
	file << "Wind Speed is: " << vel << "\n";
		
	// Voltafge bands indicate direction
	int band1 = 0;
	int band2 = 0;
	int band3 = 0;
	int band4 = 0;

    a2dChannel = 0;
    while(ii > 0)
    {
	data[0] = 1;  //  first byte transmitted -> start bit
        data[1] = 0b10000000 |( ((a2dChannel & 7) << 4)); // second byte transmitted -> (SGL/DIF = 1, ODD/SIGN = 0)
        data[2] = 0; // third byte transmitted....don't care

        a2d.spiWriteRead(data, sizeof(data) );

        a2dVal = 0;
                a2dVal = (data[1]<< 8) & 0b1100000000; //merge data[1] & data[2] to get result
                a2dVal |=  (data[2] & 0xff);
	
	
	if((a2dVal < 980) && (a2dVal > 908)){
	band1 = band1 + 1;}
        if((a2dVal < 190) && (a2dVal > 150)){
        band2 = band2 + 1;}
        if((a2dVal < 780) && (a2dVal > 750)){
        band3 = band3 + 1;}
        if((a2dVal < 510) && (a2dVal > 475)){
        band4 = band4 + 1;}
        if((a2dVal < 700) && (a2dVal > 645)){
        band3 = band3 + 1;}
        if((a2dVal < 1020) && (a2dVal > 960)){
        band4 = band4 + 1;}
		
        ii--;
    }

    int V [4] = {band1, band2, band3, band4};
    int  max = band1;
    int iii = 3;
  
    while( iii > 0 )
    { 
      if(max < V[iii]){
      max = V[iii];}
      iii--;
    }

//Band with the most counts is the current direction
if(max == band1){
file << "Direction is: N \n";
}
if(max == band2){
file << "Direction is: W \n";
}
if(max == band3){
file << "Direction is: S \n";
}
if(max == band4){
file << "Direction is: E \n";
}
	cout << band1 << endl;
        cout << band2 << endl;
	cout << band3 << endl;
	cout << band4 << endl;
	cout << "Max: " << max << endl;

file.close();	

	
    return 0;
}
