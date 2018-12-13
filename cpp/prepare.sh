#!/bin/sh
PWD=$(pwd)
cd $PWD\libs

mkdir images.full images.thumb
cp tests/data/test.png images.full/1
chmod 777 images.full images.thumb

