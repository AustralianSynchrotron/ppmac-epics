# AS2 PMAC package

This repo contains Australian Synchrotron copy of `dls-controls pmac` https://github.com/dls-controls/pmac 

The driver is only used for PowerPmac controllers, both PC and ARM versions. Compatibility with other variants not tested.

## Fixes and improvements to make the driver compatible with PPMAC ARM 

There has been modifications and fixes to the driver particularly:

.../ppmacApp/powerPmacAsynPortSrc/sshDriver.cpp
.../ppmacApp/src/pmacAxis.cpp
.../ppmacApp/src/pmacCommandStore.cpp
.../ppmacApp/src/pmacController.cpp
.../ppmacApp/src/pmacController.h
.../ppmacApp/src/pmacHardwarePower.cpp
.../ppmacApp/src/pmacMessageBroker.cpp
.../ppmacApp/src/pmacMessageBroker.h

## Compatibility with motorFloat64 motor record

Minor changes to make the driver compatible with motorFloat64 motor record in these files:

.../ppmacApp/src/pmacCSAxis.cpp
.../ppmacApp/src/pmacCSController.cpp

## Minor improvements

Minor changes for various improvements to these files:

.../ppmacApp/src/pmacCSController.h
.../ppmacApp/src/pmacCSMonitor.h
.../ppmacApp/src/pmacHardwareInterface.cpp

## IOC templates and UI files

Adopted specifically for Australian Synchrotron's BaseConfig support.

.../ppmacApp/Db
.../ppmacApp/opi/ui

## Contact
nadera@ansto.gov.au
Controls Systems
ANSTO - Australian Synchrotron
800 Blackburn Road, Clayton, Victoria 3168, Australia.



<font size="-1">Last updated: </font>
<br>