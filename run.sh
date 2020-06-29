#!/bin/bash

mode='debug'
mode='release'

if [ "$1" == "s" ]; then
	LD_LIBRARY_PATH="$PWD/netsi/build/release/lib" ./build/${mode}/bin/server
elif [ "$1" == "t" ]; then
	# ./build/${mode}/tests/bin/packet_helper_test
	LD_LIBRARY_PATH="$PWD/netsi/build/release/lib" ./build/${mode}/tests/bin/glm_test
elif [ "$1" == "r" ]; then
	LD_LIBRARY_PATH="$PWD/netsi/build/release/lib" ./build/${mode}/bin/client "generic-sauce.de" "alok"
else
	LD_LIBRARY_PATH="$PWD/netsi/build/release/lib" ./build/${mode}/bin/client "127.0.0.1" "player0"
fi
