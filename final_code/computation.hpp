//
//  computation.hpp
//  
//
//  Created by René Gaio on 27/03/2018.
//

#ifndef computation_hpp
#define computation_hpp

#include <stdio.h>
#include "geti2c.hpp"
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


//spi part
#include <cstdio>


using namespace std;

// ******************************************************************

double calc_temp();
double calc_press();
double calc_hum();
double calc_lux();
double wind();
int compass();
void epoch(char &tempo);

static int temp_add = 0x48;
static int hum_add = 0x27;
static int pres_add = 0xC0;
static int lux_add = 0x44;

#endif /* computation_hpp */
