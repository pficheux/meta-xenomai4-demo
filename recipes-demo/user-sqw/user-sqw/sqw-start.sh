#!/bin/sh

# Script to setup sqw program
CHRT_PID="99"
CPU="3"

sqw &
SQW_PID=$!

echo "'sqw' program PID: ${SQW_PID}"
echo "'sqw' configuration:"
chrt -r --pid ${CHRT_PID} ${SQW_PID}
chrt -p ${SQW_PID}

taskset -cp ${CPU} ${SQW_PID}

exit 0
