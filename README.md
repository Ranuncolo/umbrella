![Umbrella!](https://github.com/Ranuncolo/umbrella/blob/master/header.jpg)

# umbrella

Umbrella is a weather station based on Raspberry Pi.

![Umbrella!](https://github.com/Ranuncolo/umbrella/blob/master/Assembled.jpg_large)

Video demonstration:

https://www.youtube.com/watch?v=GxtKGGudiI4


## Introduction
Have you ever wondered what the local weather conditions are in your garden? Wouldn't you like a simple way to get all the necessary data for growing the best possible plants in your garden? 

We have created a Raspberry Pi based Weather station that collects and displays all its data in a very easy to understand graphed format. The device contains temperature, humidity, air pressure, ambient light and wind sensors.
It will also provide you weather data with up-to-the-minute precision.

This device will be significantly smaller and cheaper than a conventional weather station :)

## Technical Description

Although the weather can change relatively quickly, you don't need to sample the weather very frequently to plot interesting weather trends.  As this code will sample and record the weather every minute, a latency of a few seconds would be acceptable.  Only a single sample needs to be stored in the memory buffer at any one time as data can be saved to the SD card onboard the Pi.

Code is implemented in userspace.  The reduction in latency gained by running in kernel space is not required.  As the program is paired with a web app it is safer to deny the program access to the kernel, ensuring that the kernel remains protected.  This software required two threads - one for the data processing software and one for the web app.

Communication with the sensors is accomplished using the I<sup>2</sup>C protocol.  While the SPI protocol is much simpler and requires a lower processing overhead, the I<sup>2</sup>C protocol  only requires two lines connected to the Pi allowing for simpler circuitry.  Processing speed was not a priority for these devices.  For the wind speed however, an ADC with SPI was used to allow many samples to be taken across a short period of time.  The sensors contain internal reed switches that are triggered as the device rotates.  Counting rotations over a period of 3s allows a good average wind speed to be detected.

The project is split into roughly three equal parts.  Equal work share was ensured through constant communication with other group members and offering help where possible.   These parts were: 

1. Hardware and electronic circuit design - [Paul Harrison](http://github.com/pbh001)
2. Data processing onboard the Raspberry Pi - [Rene Gaio](http://github.com/Ranuncolo)
3. Data visualisation in a web browser - [Daniel Marwick](http://github.com/danmarwick)

Hardware was therefore allocated approximately one third of the time was spent on hardware and the rest on software.  Debugging of the hardware was done on a bread board as components arrived.  Debugging of software was done as features were coded. The connectivity of these parts of the system can be see in the block diagram below.

![Block Diagram](https://github.com/Ranuncolo/umbrella/blob/master/BlockDiagram.png)

Data is read from the sensors in binary format, converted to numeric values and stored in human readable csv files with conventional SI units.  A web server script is running on the Pi listening for data requests in port 32000.  When a request is made by a web browser on the network, our web app is transmitted to said web browser.  The web app plots the data using the dygraphs package within Javascript.  The benefit of Javascript is the client side execution that allows the data to be plotted on the user's web browser without adding too much load to the processor on the Pi.

The device and software are marketed using Twitter and Facebook, sharing on relevant hashtags to reach our target market.  Version control is handled using git, with version 1.0 being our initial release when we reach a minimum viable product.  Hardware and software has been split between the team members as described above.

This project is deemed succesful when it is able to detect, record and display weather data in both real time and graphed formats.

## Required Components

1. Raspberry Pi Model B+ - (newer models are available and are recommended due to faster processor and onboard WiFi)

2. Sensors

   A. [NCT75DMR2G - I<sup>2</sup>C Digital Temperature sensor](http://uk.farnell.com/on-semiconductor/nct75dmr2g/temperature-sensor-3deg-c-msop/dp/2627996)

   B. [HIH6030 - I<sup>2</sup>C Digital Humidity sensor](http://uk.farnell.com/honeywell/hih6030-021-001/sensor-humidity-no-filter-4-5/dp/2356755?st=hih6030-021-001)

   C. [MPL3115A2 - I<sup>2</sup>C Digital Pressure sensor](http://uk.farnell.com/nxp/mpl3115a2/pressure-sensor-20-110kpa-8lga/dp/2009084)

   D. [OPT3001DNPT - I<sup>2</sup>C Digital Ambient Light Sensor](http://uk.farnell.com/texas-instruments/opt3001dnpt/ambient-light-sensor-uson-6/dp/2504234)
   
   E. [Maplin Anemometer](https://www.ebay.co.uk/p/Maplin-Replacement-Anemometer-for-Use-With-Wireless-Weather-Stations-N96gy/2254341215)
   
   F. [Maplin Wind Vane](https://www.ebay.co.uk/itm/Maplin-Replacement-Wind-Direction-Sensor-For-N96Fy-N96Gy-External-/201278499640)
   
3. [MCP3002 - SPI Analogue to Digital Converter](http://uk.farnell.com/microchip/mcp3002-i-p/ic-adc-10bit-200ksps-pdip-8/dp/1852015)

   
#### Notes
 - The I<sup>2</sup>C sensors require only four pins for the entire set of sensors.
 
        - Pin 1 - 3.3V
            
        - Pin 3 - Data
            
        - Pin 5 - Clock
           
        - Pin 6 - Ground
           
- Most devices used in this project are surface mount ICs that can be difficult to solder without access to the right equipment.  Hobbyists may wish to look for equivalent through-hole sensors that can be mounted to veroboard more easily for a simpler version that can be easily assembled at home.  

- The pressure sensor has an awkward package with a metal case and pads on the bottom surface. It is especially difficult to solder!

- Our wind sensors are mounted on a 3D printed arm with slightly imprecise measurements that required the wind sensors to be awkwardly bolted on.  When creating 3D printed elements be sure that your component will be compatible with the sensors.

- Due to time constraints we were unable to create a case or an outdoor power supply solution.  These are left as a project for you, dear reader!


## Installation Instructions

### Data Processing Software
When running your Raspberry Pi for the first time run the following in the terminal to locate and install any updates to the default packages.

 > sudo apt-get update
 
 > sudo apt-get upgrade

To enable I<sup>2</sup>C devices on your Raspberry Pi you must install the following package: 

 > sudo apt-get install i2c-tools
 
To enable our devices on your Raspberry Pi you must make the following change to the OS:

 > sudo raspi-config

Then go to Interfacing Options > Advanced > I2C > Enable
Also Interfacing Options > Advanced > SPI > Enable

In the final code it is possible to run all the sensors together compiling:

> g++ umbrella.cpp geti2c.cpp computation.cpp WindSpi.cpp -o weather

> ./weather

#### Notes
- When working with I<sup>2</sup>C sensors you must be careful to only read in the number of bits specified by the datasheet.  While you must read whole bytes from the chip, the relevant data is often comprised of a non-integer number of bytes - such as 12 bits.  Reading in these extra bits into your data can give erroneous readings.
- While all I<sup>2</sup>C sensors transmit data in a similar format, there are minor variations between each sensor and component manufacturers don't always write the clearest instructions.

### Javascript Web App
Adding webserver functionality to your Raspberry Pi is simple!  All you have to do is install Node.Js for Debian.  This can be accomplished using two commands:

> wget http://<i></i>node-arm.herokuapp.com/node_latest_armhf.deb

> sudo dpkg -i node_latest_armhf.deb

The first line should download the latest version of Node.Js for Debian.  The second command should install the package.

At this stage you should be able to run .js scripts from the command line using the command "node filename.js".  You can go one step further to get your web app code to launch when the Raspberry Pi boots up by editing your rc.local file.  As always, care should be taken when editing files that affect how the system boots up.  Small errors can easily prevent the device from starting at all!

First open the file with:

> sudo nano /etc/rc.local

then add the following line of code above the "exit 0" line.  Do not delete the "exit 0" line!

> sudo node ~/final/webserver.js &

If you save your code in a different folder then change the full path of the webserver.js file accordingly.

#### Notes
- The "&" symbol in the rc.local file is crucial for this code. It creates a new branch for the webserver code and allows it to run in the background.  If we were to omit this symbol then the Pi would wait for the script to finish executing before continuing with the boot process.  However, this code runs continuously so the boot process would never finish!
- The webserver code includes the IP address of the Pi and the port over which web app will be transmitted.  The IP address of your Pi on your network can be determined by typing "ifconfig" in the command line.  If you are unsure what ports are available for communication over your local network, then contact your network administrator.
  
### Contact    
    
Email: UmbrellaRPI@gmail.com
    
Twitter: https://twitter.com/UmbrellaRPI
    
Facebook: https://www.facebook.com/UmbrellaRPI
