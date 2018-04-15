# Wind Sensor Documentation

## Electronics and magnet of the Anemometer.
![Figure 1. Inner Electronics and magnet of the Anemometer.!](https://github.com/pdh001/umbrella/blob/master/Wind%20Sensor%20Documentation/AnemometerElectronics.png)

The figure above shows the electronics inside the anemometer which consists of a reed switch. A small magnet is attached to the rotator which closes the switch after every rotation by 180 degrees. Using C++ program WindSpiMain.cpp we can count the number times the switch is closed in a given time period.

## Electronics and magnet of the wind direction sensor (wind vane)
![Figure 2. Inner Electronics and magnet of the wind direction sensor (wind vane).!](https://github.com/pdh001/umbrella/blob/master/Wind%20Sensor%20Documentation/Wind%20Direction%20Sensors%20Electronics.png) 

The figure above shows the electronics and magnet of the wind direction sensor (wind vane). It shows 8 reed switches arranged into a circle. Each switch is wired parallel to a different resistor with values:	
33k*
6.57k
8.2k*
891R
1k*
678R
2.1k*
1.41k
3.9k*
3.13k
16k*
14.12k
120k*
42.12k
64.9k*
21,88k.	
The physical resistor values are given by the valus with an asterisk and the resistance formed by two switches being closed simultaneously are left without an asterisk. As the magent rotates it closes one or two switches at a time, which varies the voltage to the ADC and the digitized voltage output from the ADC is shown in the figure below. This way the direction of the vane is indicated. The figure below shows the voltage output from the ADC wind vane as it's rotated through 360 degress (and more). The voltages are plateaus indicating the direction.
![Figure 3. Voltage output from the wind vane as the rotator is turned through more than 360 degrees.!](https://github.com/pdh001/umbrella/blob/master/Wind%20Sensor%20Documentation/ADCVoltage.png)

## Calculations to determine the number of kilo-samples per second needed

![Figure 4. Calculations to determine the number of kilo-samples per secend needed.!](https://github.com/pdh001/umbrella/blob/master/Wind%20Sensor%20Documentation/ksps_calculations.png)

The figure above shows the number of kilo-samples per second (ksps) required to measure a wind-speed of 100mph (167 km/hr). First the angular veocity is calculated from the tangential velocity (167km/hr) divided by the length of the rotator arm (7cm). Then the angular velocity is divided by pi which is the angle traversed between closed swtiches (voltage highs). For the wind vane the angular velocity is divided by pi/4 since there are 4 voltage plateaus in a 180 degree rotation as shown in voltage output from the ADC. To satisfy the Nyquist theorem and measure higher wind speeds a sampling rate of approximately 25 ksps was chosen. Our ADC was a MCP3002 which can handle up to 200ksps. 
## Wind sensor holder (3D printed)
![Figure 5. CAD designs of wind sensor holder.](https://github.com/pdh001/umbrella/blob/master/Wind%20Sensor%20Documentation/WSHolder3.png)

To hold the wind sensors together in a convenient way a holder was 3D printed. The figure above shows the CAD drawings that were made. Screw holes were added to attach the sensors to and the object is hollow so that the wires can be feedthrough to the RPi. The figure below shows the finished result.

![Figure 6. Three-D printed wind sensor holder.](https://github.com/pdh001/umbrella/blob/master/Wind%20Sensor%20Documentation/WindSensorHolder.png)
