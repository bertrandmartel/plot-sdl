#!/bin/bash

if [ "$1" == "distclean" ]; then

	cd ./app/src/main/jni
	echo -e "\x1B[01;93mcleaning ndk build\x1B[0m"
	ndk-build distclean
	echo -e "\x1B[01;93mremoving external repo\x1B[0m"
	rm -rf SDL_ttf
	rm -rf SDL
	echo -ne "\x1B[0m"

elif [ "$1" == "clean" ]; then

	echo -ne "\x1B[01;93m"
	cd ./app/src/main/jni
	echo "cleaning ndk build"
	ndk-build distclean
	echo -ne "\x1B[0m"

elif [ "$1" == "build" ]; then

	cd ./app/src/main/jni
	echo -e "\x1B[01;93mndk-build\x1B[0m"
	ndk-build

else

	echo -e "\x1B[01;93mRetrieving SDL_ttf ...\x1B[0m"
	hg clone http://hg.libsdl.org/SDL_ttf ./app/src/main/jni/SDL_ttf
	echo -e "\x1B[01;93mRetrieving SDL ...\x1B[0m"
	hg clone http://hg.libsdl.org/SDL     ./app/src/main/jni/SDL
	cd ./app/src/main/jni/SDL
	echo -e "\x1B[01;93mCheking out to SDL release-2.0.3\x1B[0m"
	hg checkout release-2.0.3

fi

exit 0