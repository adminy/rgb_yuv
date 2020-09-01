#!/bin/bash
echo "Compiling ..."
g++ main.cpp -o main
echo "Done\nRunning ..."
./main
echo "Done\nStarting Web Server ..."
python2.7 -m SimpleHTTPServer 80
