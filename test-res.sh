#!/bin/bash

gcc -o pizza_delivery leitourgika.c -pthread
./pizza_delivery 100 1000
