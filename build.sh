#!/bin/sh

gcc -lm -o main ./main.c ./include/libraylib.a && ./main && rm main
