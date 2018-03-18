![Umbrella!](https://github.com/Ranuncolo/umbrella/blob/master/header.jpg)

# umbrella

Umbrella is a weather station based on Raspberry Pi.


## Introduction
Have you ever wondered what the local weather conditions are in your garden? Wouldn't you like a simple way to get all the necessary data for growing the best possible plants in your garden? 

We proposed to create a Raspberry Pi Weather station that collects and displays all its data in a very easy to understand format. The device will initially feature temperature, humidity and air pressure sensors.
It will also provide you weather data with up-to-the-minute precision, as well as daily, weekly and monthly summaries.

This device will be significantly smaller and cheaper than a conventional weather station :)

## Required Components

1. Raspberry Pi Model B+ - (newer models are available and are recommended due to faster processor and onboard WiFi)

2. Sensors

   A. [NCT75DMR2G - I<sup>2</sup>C Digital Temperature sensor](http://uk.farnell.com/on-semiconductor/nct75dmr2g/temperature-sensor-3deg-c-msop/dp/2627996)

   B. [HIH6030 - I<sup>2</sup>C Digital Humidity sensor](http://uk.farnell.com/honeywell/hih6030-021-001/sensor-humidity-no-filter-4-5/dp/2356755?st=hih6030-021-001)

   C. [MPL3115A2 - I<sup>2</sup>C Digital Pressure sensor](http://uk.farnell.com/nxp/mpl3115a2/pressure-sensor-20-110kpa-8lga/dp/2009084)

   D. [OPT3001DNPT - I<sup>2</sup>C Digital Ambient Light Sensor](http://uk.farnell.com/texas-instruments/opt3001dnpt/ambient-light-sensor-uson-6/dp/2504234)

## Technical Description

Although the weather can change relatively quickly, you don't need to sample the weather very frequently to plot interesting weather trends.  This code will sample and record the weather every 15 minutes.  A latency of around 1 minute would be acceptable, but this code will save the sensor data in a much shorter time period than this.  Only a single sample needs to be stored in the memory buffer at any one time as data can be saved to the SD card onboard the Pi.

Communication with the sensors is accomplished using the I<sup>2</sup>C protocol.  While the SPI protocol is much simpler and requires a lower processing overhead, the I<sup>2</sup>C protocol  only requires two lines connected to the Pi allowing for simpler circuitry.  Processing speed was not a priority in this design.

The project is split into roughly three equal parts.  Equal responsibility was ensured through constant communication with other group members.  Due to the These part were: 

1. Hardware and electronic circuit design - [Paul Harrison](http://github.com/pbh001)
2. Data processing onboard the Raspberry Pi - [Rene Gaio](http://github.com/Ranuncolo)
3. Data visualisation in a web browser - [Daniel Marwick](http://github.com/danmarwick)

The connectivity of these parts of the system can be see in the block diagram below.

![Block Diagram](https://github.com/Ranuncolo/umbrella/blob/master/BlockDiagram.jpg)

Data is read from the sensors in binary format, converted to numeric values and stored in human readable csv files with conventional SI units.  Due to the age of the Pi used in this initial project this stage can take a few seconds.  A web server script is running on the Pi listening for data requests in port 8000.  When a request is made by a web browser on the network, our web app is transmitted to said web browser.  The web app plots the data using the Google Charts functionality within Javascript.  The benefit of Javascript is the client side execution that allows the data to be plotted on the user's web browser without adding too much load to the processor on the Pi.

<!-- 6. Low level implementation: kernel or userspace and why?
11. How many threads are needed and/or how can the load be distributed to allow a responsive application?
13. Structure of the software in classes, associated unit tests to turn it into reliable software
15. How much time is allocated to hard, software and debugging and how is this interleaved?
16. Which version control software is used? How are releases / milestones done and numbered?
17. What is the release strategy / publication / publicity? How is that measured and deemed to be successful?
18. Measurement of success of the application in the context of the problem given and evaluation of it. -->

## Installation Instructions

### Hardware
Hardware installation instructions

#### Notes
Tips, tricks and troubleshooting on hardware set up

### Data Processing Software
Data processing installation instructions

#### Notes
Tips, tricks and troubleshooting on C++ code set up

### Javascript Web App
Adding webserver functionality to your Raspberry Pi is simple!  All you have to do is install Node.Js for Debian.  This can be accomplished using two commands:

> wget http://<i></i>node-arm.herokuapp.com/node_latest_armhf.deb

> sudo dpkg -i node_latest_armhf.deb

The first line should download the latest version of Node.Js for Debian.  The second command should install the package.

At this stage you should be able to run .js scripts from the command line using the command "node filename.js".  You can go one step further to get your web app code to launch when the Raspberry Pi boots up by editing your rc.local file.  As always, care should be taken when editing files that affect how the system boots up.  Small errors can easily prevent the device from starting at all!

First open the file with:

> sudo nano /etc/rc.local

then add the following line of code above the "exit 0" line.  Do not delete the "exit 0" line!

> sudo node /usr/local/webserver.js &

If you save your code in a different folder then change the full path of the webserver.js file accordingly.

#### Notes
- The "&" symbol in the rc.local file is crucial for this code. It creates a new branch for the webserver code and allows it to run in the background.  If we were to omit this symbol then the Pi would wait for the script to finish executing before continuing with the boot process.  However, this code runs continuously so the boot process would never finish!
- The webserver code includes the IP address of the Pi and the port over which web app will be transmitted.  The IP address of your Pi on your network can be determined by typing "ifconfig" in the command line.  If you are unsure what ports are available for communication over your local network, then contact your network administrator.
  
### Contact    
    
Email: UmbrellaRPI@gmail.com
    
Twitter: https://twitter.com/UmbrellaRPI
    
Facebook: https://www.facebook.com/UmbrellaRPI
