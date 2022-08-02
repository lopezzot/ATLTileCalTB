#!/bin/bash
# Usage: ./cpconf.sh configs/version.sh /path/to/geant-config-generator/configs/geant4/ /your/path/with/executable/to/be/added
sed "s%@YOURPATH@%$3%" $1 > $2$(basename $1)
