#!/bin/bash

SERVER_FILE="../build/release/bin/server"
NETSI_INCLUDES="../netsi/build/release/include/netsi"
NETSI_LIB="../netsi/build/release/lib/libnetsi.so"

if [ ! -f ${SERVER_FILE} ]; then
	echo "You have to build the server in release mode first. Could not find file ${SERVER_FILE}. Aborting."
	exit 1
fi

if [ ! -d ${NETSI_INCLUDES} ]; then
	echo "You have to build the netsi in release mode first. Could not find directory ${NETSI_INCLUDES}. Aborting."
	exit 1
fi

if [ ! -f ${NETSI_LIB} ]; then
	echo "You have to build netsi in release mode first. Could not find file ${NETSI_LIB}. Aborting."
	exit 1
fi


cp ${SERVER_FILE} ./server
cp -r ${NETSI_INCLUDES} ./netsi_includes
cp ${NETSI_LIB} ./libnetsi.so

docker build -t bruno1996/shepherd-server:1.0 .
