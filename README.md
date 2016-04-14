# ATMEL-ATMOS

## File structure ( inside ./ATMOS/ )

* /sys/
    * inc
        * sysTimer.h
    * src
        * sysTimer.c

* /drivers/
    * inc
        * SPI.h
        * int_timer.h
        * etc.
    * src
        * SPI.c
        * int_timer.c
        * etc.

* /devices/
    * inc
        * BMP280.h
        * etc.
    * src
        * BMP280.c
        * etc.

* /wrapper/ 
    * base-class
        * inc
            * Base_Device.h
            * Base_Sensor.h
        * src
            * Base_Device.c
            * Base_Sensor.c
    * sensor
        * inc
            * BMP280_Sensor.h
            * etc.
        * src
            * BMP280_Sensor.c
            * etc.
    * other-device
        * inc
            * LW-mesh.h (TODO)
            * out-timer.h (TODO)
        * src
            * LW-mesh.c (TODO)
            * out-timer.c (TODO)

* /utilities/
    * inc
        * llist.h
        * common.h
        * config.h
    * src
        * llist.c
        * common.c

* /event/
    * inc
        * event.h
    * src
        * event.c

* /scheduler/
    * inc
        * scheduler.h (TODO)
    * src
        * scheduler.c (TODO)

* /handler/
    * inc
        * error_handler.h (TODO)
        * collectData_handler.h (TODO)
        * uploadData_handler.h (TODO)
        * sensorEvent_handler.h (TODO)
    * src
        * error_handler.c (TODO)
        * collectData_handler.c (TODO)
        * uploadData_handler.c (TODO)
        * sensorEvent_handler.c (TODO)
