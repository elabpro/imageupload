#!/bin/sh
PWD=$(pwd)

apt-get -y install libboost-all-dev libfastjson-dev libpthread-stubs0-dev libcurl4-gnutls-dev

echo "Installing library to parse multipart requests"
cd libs
tar -zxf MPFDParser-1.1.1.tar.gz
cd MPFDParser-1.1.1
rm -f build && mkdir build
cd build
cmake ..
make
make install
cd ../../..
echo "Installing library for making REST API on C++"
cd libs
tar -zxf pistache.tar.gz
cd pistache
rm -f build && mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make
make install
cd ../../..
echo "Installing library for communication with Redis DB"
cd libs
tar -zxf redisclient.tar.gz
cd redisclient
rm -f build && mkdir build
cd build
cmake ..
make
make install
cp ../src/redisclient/impl/*.cpp /usr/local/include/redisclient/impl/
cd ../../..

mkdir images.full images.thumb
cp tests/data/test.png images.full/1
chmod 777 images.full images.thumb

