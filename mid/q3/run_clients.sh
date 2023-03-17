#!/bin/bash
for i in {0..4}
do
    ./client1 $i
    ./client2 $i
done
