#!/bin/sh
PWD=$(pwd)

sudo apt-get -y install libboost-all-dev

echo "Installing library to parse multipart requests"
cd libs/MPFDParser-1.1.1
mkdir build
cd build
cmake ..
make
sudo make install
cd ../../..
echo "Installing library for making REST API on C++"
cd libs/pistache
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
cd ../../..
echo "Installing library for communication with Redis DB"
cd $PWD/libs/redisclient
mkdir build
cd build
cmake ..
make
sudo make install
cd ../../..

mkdir images.full images.thumb
cp tests/data/test.png images.full/1
chmod 777 images.full images.thumb

