//
//  computation.cpp
//  
//
//  Created by René Gaio on 27/03/2018.
//

/* this .cpp file include the four different functions required to convert digital data into proper data, for each sensor.
 */

#include "computation.hpp"

using namespace std;


// ******************************************************************

double calc_temp()
{
    unsigned char T_bit[2];
    int Temp = 0;
    double Temperature = NAN;
    
    //instance of class:
    geti2c tempsensor(string("temperature"), temp_add, 2);
    
    //address register + check status, else change in NAN:
    int flag = -1;
    flag = tempsensor.add_register();
    if (flag <0)
    {
        Temperature = NAN;
        return Temperature;
    }
    //read register + check status, else change in NAN:
    flag = tempsensor.read_register(*T_bit);
    if (flag <0)
    {
        Temperature = NAN;
        return Temperature;
    }
    // Convert into value:
    std::bitset<16> Tcombo;
    std::bitset<16>Tb1(T_bit[0]);
    std::bitset<16>Tb2(T_bit[1]);
    Tcombo = (Tb1 << 8) | Tb2;
    
    // last for 4 are just empty and they are not part of the number:
    Tcombo = Tcombo >> 4;
    // cast to int
    
    Temp = (int)(Tcombo.to_ulong());
    // following the conversion on the data sheet, pag 5, set the difference between negative and positive temperatures, then convert to T [C].
    // to convert, the data must be multiplied by 0.0625, the sensitivity (1/16)
    if (Temp > 2047)
    {
        Temp = Temp - 4096;
    }
    Temperature = Temp * 0.0625;
    
    return Temperature;
}


// ******************************************************************

double calc_hum()
{
    unsigned char H_bit[2];
    int Hum = 0;
    double Humidity = NAN;
    
    //instance of class:
    geti2c humsensor(string("humidity"), hum_add, 2);
    
    //address register + check status, else change in NAN:
    int flag = -1;
    flag = humsensor.add_register();
    if (flag <0)
    {
        Humidity = NAN;
        return Humidity;
    }
    //read register + check status, else change in NAN:
    flag = humsensor.read_register(*H_bit);
    if (flag <0)
    {
        Humidity = NAN;
        return Humidity;
    }
    // Convert into value:
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
    Hum = (int)(Hcombo.to_ulong()); // cast into intiger
    Humidity = Hum / denominator * 100;
    
    return Humidity;
}


// ******************************************************************

double calc_press()
{
    unsigned char P_bit[4];
    double Pressure = NAN;
    
    //instance of class:
    geti2c presensor(string("pressure"), pre_add, 4);
    
    //address register + check status, else change in NAN:
    int flag = -1;
    flag = presensor.add_register();
    if (flag <0)
    {
        Pressure = NAN;
        return Pressure;
    }
    //read register + check status, else change in NAN:
    flag = presensor.read_register(*P_bit);
    if (flag <0)
    {
        Pressure = NAN;
        return Pressure;
    }
    // Convert into value:
    std::bitset<24> Pint(string("000000111111111111111111"));;
    std::bitset<24>Pb1(P_bit[0]);
    std::bitset<24>Pb2(P_bit[1]);
    std::bitset<24>Pb3(P_bit[2]);
    std::bitset<24>Pb4(P_bit[3]);
    
    //P_bit1 contains only status variables, ignored for the moment.
    // Pb2      = 00000000 00000000 10101010
    Pb2 = Pb2 << 10;
    // Pcombo  = 00000010 10101000 00000000
    
    // Pb3     = 00000000 00000000 11011011
    Pb3 = Pb3 << 2;
    // Pb3     = 00000000 00000011 01101100
    
    // Pb4     = 00000000 00000000 11010111
    Pb4 = Pb4 >> 6;
    // Pb4     = 00000000 00000000 00000011
    
    // Pnum   = 00000000 00000000 00000000
    Pint = Pint | Pb2;
    Pint = Pint | Pb3;
    Pint = Pint | Pb4;
    
    int Pre = (int)(Pint.to_ulong());
    Pressure = Pre / 101325.0; // Pressure in atm
    return Pressure;
}


// ******************************************************************

double calc_lux()
{
    unsigned char L_bit[2];
    double Light = NAN;
    
    //instance of class:
    geti2c lightsensor(string("light"), lux_add, 2);
    
    //address register + check status, else change in NAN:
    int flag = -1;
    flag = lightsensor.add_register();
    if (flag <0)
    {
        Light = NAN;
        return Light;
    }
    //read register + check status, else change in NAN:
    flag = lightsensor.read_register(*L_bit);
    if (flag <0)
    {
        Light = NAN;
        return Light;
    }
    // Convert into value:
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
    
    return Light;
}


// ******************************************************************

void epoch(char &tempo)
{
    time_t data_epoch;
    struct tm *tepoch;
    char *epoch_time;
    epoch_time = &tempo;
    time(&data_epoch);
    tepoch = localtime(&data_epoch);
    strftime(epoch_time,17,"%F %H:%M",tepoch);
}

// ******************************************************************

double wind()
{
    WindSpi a2d("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);
    int i = 25000;
    int a2dVal = 0;
    int a2dChannel = 7;
    unsigned char data[3];
    int a2dVal_new = 0;
    int a2dVal_old = 0;
    double highs = 0.0;
    double vel;
    double duration;

    std::clock_t start;
    start = std::clock();    //start clock
    
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
        
        
        a2dVal_new = a2dVal;    //update value
        if( (a2dVal_new > 800) && (a2dVal_old < 800)){    //check for initial high value
            highs = highs + 1.0;}        //count high values
        
        i--;
        
        a2dVal_old = a2dVal_new;     // update old value for next iteration
    }
    
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;        //check time duration
    if(highs == 1)
    {
        vel = 0;
    }
    else
    {
        vel = 3.60*2.00*3.14*0.07*highs/(2.00*duration); // calculate velocity: vel = 3.6km/hr * 2pi * 7 cm * (number of high values/2)/(measurement time)
    }
    
    return vel;
}

int compass()
{
    int a2dChannel = 0;
    int ii = 25000;
    int a2dVal = 0;
    unsigned char data[3];
    int band1 = 0;
    int band2 = 0;
    int band3 = 0;
    int band4 = 0;
    
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
        return 1;
    }
    if(max == band2){
        return 2;
    }
    if(max == band3){
        return 3;
    }
    if(max == band4){
        return 4;
    }
}

