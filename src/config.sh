#!/bin/sh
socat PTY,link=/tmp/ttyS0,raw,echo=0 PTY,link=/tmp/ttyACM0,raw,echo=0

