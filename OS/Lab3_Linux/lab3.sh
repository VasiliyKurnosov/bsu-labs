#!/bin/bash

build() {
	# Static library
	g++ -c -o Number/Number.o Number/Number.cpp
	ar rvs Number/libNumber.a Number/Number.o
	
	# Dynamic library
	g++ -c -o Vector/Vector.o -INumber/ Vector/Vector.cpp
	g++ -shared -o Vector/libVector.so Vector/Vector.o Number/libNumber.a
	
	# Build project
	g++ -o lab3 -INumber/ -IVector/ main.cpp -LNumber/ -LVector/ -lNumber -lVector
	export LD_LIBRARY_PATH=Vector/:$LD_LIBRARY_PATH
	
	return
}

run() {
	./lab3
	return
}

build
run

