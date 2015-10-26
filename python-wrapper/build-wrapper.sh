#!/bin/bash

if [ "$1" == "clean" ]; then

	echo "cleaning python-wrapper..."
	rm -rf ./build
	rm -rf ./plotsdl/build
	rm -f ./plotsdl/*.so ./plotsdl/*.c ./plotsdl/*pyc

else

	python setup.py build_ext -i

fi