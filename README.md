# Operator-Console-Qt
This project was built with Qt 5.1.11, and Imatest IT 5.1.
The project will build and run, but not all features are implemented yet.
Image Acqusition was tested using Qt's camera intefrace, using a built-in webcam.

## Ubuntu 14.04
Currently the Operator Console on Ubuntu 14.04 must be compiled with a static build of QT 5.8.

## Dependencies
To begin, install all the packages listed on https://wiki.qt.io/Building_Qt_5_from_Git Note that the `sudo apt-get build-dep qt5-default` command currently fails on Ubuntu 14.04.5, but it can be ignored.

Next, gcc 4.9 needs to be installed for the MATLAB Runtime

````
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-4.9 g++-4.9
````

## Getting the source
Following the instructions in https://wiki.qt.io/Building_Qt_5_from_Git#Getting_the_source_code, the 5.8.0 source code can be checked out with the following git commands.

````
$ git clone https://code.qt.io/qt/qt5.git                     # cloning the repo
$ cd qt5
$ git checkout v5.8.0                                         # checking out the specific release or branch
$ git submodule update --init --recursive                     # updating each submodule to match the supermodule
````

## Building Static QT libraries
First, create a `build` folder within `~/qt5` to house the build libraries and other build products

````
PREFIX_DIR="$HOME/qt5/build"


./configure -static \
-release \
-qt-freetype \
-qt-harfbuzz \
-qt-libjpeg \
-qt-libpng \
-recheck-all \
-opensource \
-confirm-license \
-recheck-all \
-no-pch \
-prefix $PREFIX_DIR \
-skip webengine \
-skip 3d \
-nomake tools \
-nomake tests \
-nomake examples \
````

When ./configure finishes running, it will report the status of various dependencies. If there are no serious errors reported, start the build with

````
make
````

and then install the products to ~/qt5/build with 
````
make install
````

If you need to rebuild, run 
````
make distclean -j<number of cores>
````
before re-running `configure` or `make`.

## Building the Operator Console

Open a terminal and cd to the top-level folder for the Operator Console source. 

Run qmake to generate makefiles for the project with the following:
````
export PATH=$HOME/qt5/build/bin:$PATH

qmake qmake OperatorConsole.pro -spec linux-g++ CONFIG+=qtquickcompiler
````

Next build the Operator Console with 
````
make
````


