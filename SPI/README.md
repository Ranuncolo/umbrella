===========================================================================================================

Description: The WindSpi class enables the Raspberry Pi to communicate with the MCP002 SPI ADC using spidev. The class can be easily modified to enable the Raspberry Pi to communicate with other SPI devices.

============================================================================================================

To build on the Raspberry Pi:

g++ -c WindSpiMain.cpp
g++ -fpermissive -Wall WindSpi.cpp WindSpiMain.cpp -o outBin


Make sure that the WindSpi.h header file is in the work directory.
Run ./outBin. The output is a file with current time as the filename and contains the wind speed and direction. 
