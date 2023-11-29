#!/bin/bash  --login       
# $File: //ASP/tec/mc/ppmac/trunk/ppmacApp/opi/ui/ppmac-qegui-edit.sh $     
# $Revision: #6 $
# $DateTime: 2023/08/29 17:29:42 $
# Last checked in by: $Author: afsharn $
#
# Description:
# This script opens ppmac ui
#
 
# Determine current bundle
#
if [ -f /asp/config/host_setup.rc ] ; then
      . /asp/config/host_setup.rc
else
    xmessage "  /asp/config/host_setup.rc does not exist  "
    exit 2
fi
 
bundle=${HOST_BUNDLE:?}
 
# Include libraries from the bundle into library path.
export LD_LIBRARY_PATH=${bundle:?}/_lib/${EPICS_HOST_ARCH}:${LD_LIBRARY_PATH}
 
# OR for bundle 16 or later
export LD_LIBRARY_PATH=${bundle:?}/_opilib/${EPICS_HOST_ARCH}:${LD_LIBRARY_PATH}
 
# Include plugin libraries from the bundle.
export QT_PLUGIN_PATH=${QT_PLUGIN_PATH}:${bundle:?}/gui/qeframework/lib/${EPICS_HOST_ARCH}
export QT_PLUGIN_PATH=${QT_PLUGIN_PATH}:${bundle:?}/gui/qtfsm/lib/${EPICS_HOST_ARCH}
 
# Include any application specific plugins here
# export QT_PLUGIN_PATH=${QT_PLUGIN_PATH}:/beamline/perforce/opa/opis/myPlugin/lib/${EPICS_HOST_ARCH}
 
# Define the QE UI PATH
export QE_UI_PATH=/beamline/perforce/tec/mc/ppmac/ppmacApp/opi/ui
#"../../ASppmacIOCApp/opi/ui"

#run
#${bundle:?}/gui/qegui/bin/${EPICS_HOST_ARCH:?}/qegui -o  -m "PBRICK={{pblv_ID}},MTR1={{mtr1_pv}},MTR2={{mtr2_pv}},MTR3={{mtr3_pv}},MTR4={{mtr4_pv}},MTR5={{mtr5_pv}},MTR6={{mtr6_pv}},MTR7={{mtr7_pv}},MTR8={{mtr8_pv}}"  ppmac.ui &

#design
#cd /home/baldwinb/ASppmacIOC/ASppmacIOCApp/opi/ui
qe_designer ppmac.ui &

# end