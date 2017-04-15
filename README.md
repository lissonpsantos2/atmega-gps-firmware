# atmega-gps-firmware
Firmware to be used with a Atmega microcontroller to read the GPS NMEA data

### Select NMEA message
To select a NMEA message you have only to set the GPS_SENTENCE and GPS_SENTENCE_LENGTH, with the respective gps sentence header and its length.

### Return message
At the end of the algorithm a array of strings is returned, what makes the desired data extraction easier.
