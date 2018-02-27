//
//  1wpro.cpp
//  C++ code for addressing up to three one-wire
//  devices in Raspberry Pi.
//
//  Created by René Gaio on 26/02/2018.
//

//#include "1wpro.hpp"
#include <iostream>
#include <fstream> //open/write/close files
#include <ctime> //giving the file output name
#include <string>
#include <cstdlib> //for function
#include <dirent.h> //moving through directories
#include <unistd.h> // for unsleep
//#include <stdlib.h> // converts string to float/double etc
//#include <math> //for some mathematica functions
//#include <iomanip> //for formatting i/o

using namespace std;


int main()
{
    // Definition of parameters used for the one-wire protocol:
    
    // Name of the three sensors (ID)
    char sensor1[20];
    char sensor2[20];
    char sensor3[20];
    
    int num_sens = 0; // the number of sensors found. Used not to overwrite anything
    
    double temp2 = 999999;
    
    // Path for the one-wire protocol
    string path = "/sys/bus/w1/devices/";
    string path1;
    string path2;
    string path3;
    
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    // Creation of the file.
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
    
    // Now I create and open the file.
    ofstream file;
    file.open(filename);
    
    // check:
    if (!file)
    {
        cout << "unable to open it.";
        return 0;
    }
    
    // First output
    file << "Long time no see :)" << endl;
    file << endl;
    
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    // Let's search for sensors:
    cout << "Searching for sensors." << endl;
    
    // To open the directory I need the <dirent> library.
    
    DIR *pdir = NULL; // pointer to the directory, initialised.
    
    // Now I need to point to the structure where many info are stored
    struct dirent *pent = NULL;
    pdir = opendir("."); //for testing purposes, its the actual directory
    //pdir = opendir(path); //the proper one, remember to change.
    cout << "Have you changed the proper path or are you still in test?" << endl;
    
    // check
    if (pdir == NULL)
    {
        cout << "error, directory not found." << endl;
        file << "error, Temperature directory not found." << endl;
        return 0;
    }
    
    // Let's read what there is in this directory.
    while ((pent = readdir(pdir))) // meaning: as long as there is something else in the directory
    {
        if (pent == NULL)
        {
            cout << "error, pent could not be initialised correctly." <<endl;
            return 0;
        }
        
        else if (strstr(pent->d_name, "28-") != NULL)
            // d_name, in the structure, collects the name of everything in the directory.
        {
            if (num_sens == 0)
            {
                strcpy(sensor1, pent->d_name); //store the ID in sensor1
                num_sens += 1; // add the sensor.
            }
            
            else if (num_sens == 1)
            {
                strcpy(sensor2, pent->d_name); //store the ID in sensor2
                num_sens += 1; // add the sensor.
            }
            
            else if (num_sens == 2)
            {
                strcpy(sensor3, pent->d_name); //store the ID in sensor3
                num_sens += 1; // add the sensor.
            }
            
            else if (num_sens > 2)
            {
                cout << "There are too many sensors!" << endl;
                cout << "ABORT: too many sensors." << endl;
                return 0;
            }
        }
    }
    
    cout << "I have found " << num_sens << " sensors." << endl;
    cout << "These are the sensor I found:" << endl;
    cout << sensor1 << "\t" << sensor2 << "\t" << sensor3 <<endl;
    
    
    // I don't need it anymore:
    closedir(pdir);
    
    
    // I can now build up the whole link path
    
    if (num_sens == 1)
    {
        path1 = path;
        path1.append(sensor1);
        path1.append("/w1_slave");
    }
    
    if (num_sens == 2)
    {
        path1 = path;
        path2 = path;
        path1.append(sensor1);
        path2.append(sensor2);
        path1.append("/w1_slave");
        path2.append("/w1_slave");
    }
    
    if (num_sens == 3)
    {
        path1 = path;
        path2 = path;
        path3 = path;
        path1.append(sensor1);
        path2.append(sensor2);
        path3.append(sensor3);
        path1.append("/w1_slave");
        path2.append("/w1_slave");
        path3.append("/w1_slave");
    }
    
    
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    // Header of the output file
    file << "Epoch\t\t T1[C]\t\t T2[C]" << endl;
    cout << "Epoch\t\t T1[C]\t\t T2[C]" << endl;
    

    // Open the file and read it.
    // infinite loop
    while(1)
    {
        // open the file at the "path" address
        ifstream data1;
        ifstream data2;
        ifstream data3;
        //data1.open(path1); //it should open that file, not sure.
        data1.open(sensor1); //JFDSAJDSAJDJSAD SAD
        // check:
        if (!data1)
        {
            cout << "Unable to open the sensor";
            return 1; // abort
        }
        
        
        if (num_sens == 2)
        {
            //data2.open(path2);
            data2.open(sensor2); //JFDSAJDSAJDJSAD SAD
            if (!data2)
            {
                cout << "Unable to open one of the sensors.";
                return 1; // abort
            }
        }
        
        if (num_sens == 3)
        {
            data2.open(path2);
            data3.open(path3);
            if (!data2 || !data3) // not sure about this statement
            {
                cout << "Unable to open one of the sensors.";
                return 1; // abort
            }
        }
        
        //Now I have to read it, somehow.
        // Strings where i write the w1_slave
        string line1;
        string line2;
        string line3;
        
        // Need to save the value of the sensor.
        // You can save here, every time, the value.
        string value1;
        string value2;
        string value3;
        
        

        
        while (data1.good()) //as long as there is something to read, it copies the line in "string". There is just the second line, but it's fine for the moment.
        {
            getline(data1,line1);  // put the text in the string "line#"
        }
        
        
        if (line1.find("t=",0)!=string::npos) //if it finds "t=", ok, else close everything.
            {
                int start;                      // counter in the string
                start = line1.find("t=",0);    // search for the position
                value1 = line1.substr(start+2,6);  //extract the temp val
                cout << value1 << endl;
                
            }
            
            else
            {
                cout << "No data available." << endl;
                return 1; // abort
            }
        
        double temp1;  //convert string in a double
        temp1 = std::stoi(value1); // here, finally, the temperature
        
        if (num_sens == 2)
        {
            while (data2.good())
            {
                getline(data2,line2);
            }
        
            if (line2.find("t=",0)!=string::npos) //if it finds "t=", ok, else close everything.
            {
                int start;                      // counter in the string
                start = line2.find("t=",0);    // search for the position
                value2 = line2.substr(start+2,6);  //extract the temp val
                cout << value2 << endl;
            }
        
            else
            {
                cout << "No data available." << endl;
                return 1; // abort
            }
        
            //convert string in a double
            temp2 = std::stoi(value2); // here, finally, the temperature
        }
        
        // search for the proper time: epoch

        time_t epoch;      //name of the chosen object, moment.
        struct tm *tepoch;
        char times[10]; //it's not so long actually, but you never know
        time(&epoch); // I'm giving in input the address of my obj moment
        tepoch = localtime(&epoch); // And I store this in tpointe
        strftime(times,10,"%H:%M:%S",tepoch);
        
        
        
        
        
        cout << times << "\t" << temp1/1000 << "\t\t" << temp2/1000 << endl;
        file << times << "\t" << temp1/1000 << "\t\t" << temp2/1000 << endl;
        

        
        // WAIT 1 second.
        usleep(1000000); //ogni 1 secondo
        
       
        
        
        
        data1.close();
        data2.close();
        data3.close();
        
        
        
        
        
        
        
        
        
        
    }
    
    
    
    
    
    // close the file I'm writing in.
    file.close();
    return 0;
    
}

