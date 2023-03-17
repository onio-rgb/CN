#!/bin/bash
for i in {0..3}
do
    ./client $i 
    sleep 1
done