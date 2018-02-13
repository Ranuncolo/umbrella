


# umbrella

Umbrella is a weather station based on Raspberry Pi.


## Problem
Have you ever wondered what the local weather conditions are in your garden? Wouldn't you like a simple way to get all the necessary data for growing the best possible plants in your garden? 

We proposed to create a Raspberry Pi Weather station that collects and displays all its data in a very easy to understand format. The device will initially feature temperature, humidity and air pressure sensors.
It will also provide you weather data with up-to-the-minute precision, as well as daily, weekly and monthly summaries.

This device will be significantly smaller and cheaper than a conventional weather station :)

## Solution

Umbrella is formed by three main parts: 
- **Hardware**: various meteorological sensors;
- **Software**: written in C/C++;
- **Display**: JavaScript based web app.

### Components

1. [Temperature sensor](http://uk.farnell.com/maxim-integrated-products/max31820mcr/temperature-sensor-2deg-c-to-92/dp/2515555)

2. [Humidity sensor](http://uk.farnell.com/honeywell/hih6030-021-001/sensor-humidity-no-filter-4-5/dp/2356755?st=hih6030-021-001)

3. [Pressure sensor](http://uk.farnell.com/honeywell/abpllnt010bgaa5/pressure-sensor-10bar-axial-barbless/dp/2490461)

4. Further sensors:
    1. [Photoresistor](http://uk.farnell.com/advanced-photonix/nsl-19m51/light-dependent-resistor-550nm/dp/3168335)

    2. [Hall sensor - wind speed](http://uk.farnell.com/allegro-microsystems/a1302kua-t/sensor-hall-effect-linear-3sip/dp/1791388)

    3. [Reed switches - wind direction, tipping rain gauge](http://uk.farnell.com/standexmeder/mk23-85-e-2/reed-switch-spst-no-1a-1kv-smd/dp/2453625)
