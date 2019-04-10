# Operator-Console-Qt
This project was built with Qt 5.1.11, and Imatest IT 5.1.
The project will build and run, but not all features are implemented yet.
Image Acqusition was tested using Qt's camera intefrace, using a built-in webcam.

## Ubuntu 14.04
Currently the Operator Console on Ubuntu 14.04 must be compiled with a static build of QT 5.8.

## Dependencies
To begin, install all the packages listed below (cf. https://wiki.qt.io/Building_Qt_5_from_Git)
````
sudo apt-get install libxcb-xinerama0-dev build-essential perl python git '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev  flex bison gperf libicu-dev libxslt-dev ruby libssl-dev libxcursor-dev libxcomposite-dev libxdamage-dev libxrandr-dev libdbus-1-dev libfontconfig1-dev libcap-dev libxtst-dev libpulse-dev libudev-dev libpci-dev libnss3-dev libasound2-dev libxss-dev libegl1-mesa-dev gperf bison libasound2-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev
````


Next, gcc 4.9 needs to be installed for the MATLAB Runtime

````
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-4.9 g++-4.9
````

## Getting the QT source
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

## Copy fonts
Currently fonts are not embedded in the OperatorConsole, so they need to be copied to the expected directory (`~/qt5/build/lib/fonts`).

````
mkdir ~/qt5/build/lib/fonts
cp /usr/share/fonts/truetype/dejavu/*.* ~/qt5/build/lib/fonts/

````

## Install Imatest IT
[Install Imatest IT](http://www.imatest.com/docs/installation/) and [activate your license](http://www.imatest.com/activation/).

## Get the Operator Console source
The source code can either be downloaded as a *.zip file or cloned from this repo. For this repo, the clone operation would be as follows:

````
cd ~
git clone https://github.com/imatest/Operator-Console-Qt.git
````

This will download the source to `~/Operator-Console-Qt`.

## Building the Operator Console

Open a terminal and cd to the top-level folder for the Operator Console source (i.e. `~/Operator-Console-Qt` in the above example). 

Run qmake to generate makefiles for the project with the following:
````
export PATH=$HOME/qt5/build/bin:$PATH

qmake qmake OperatorConsole.pro -spec linux-g++ CONFIG+=qtquickcompiler
````

Next build the Operator Console with 
````
make
````

If there is a build error where libImatest.h cannot be found it might be due to a sporadic bug with qmake. Open `/Operator-Console-Qt/app/app.pro` and add `message($$FOLDERS)` after the definition of `FOLDERS` on line 26. Then repeat the call to `qmake`, `make clean` and `make` a few times.

## Running the Operator Console

To run the OperatorConsole `cd` into the `app` sub-folder and run the executable

````
cd ~/Operator-Console-Qt/app
./OperatorConsole
````

Note that it may take about 30 seconds to start. To improve the startup time consider setting the `MCR_CACHE_ROOT` and `MCR_CACHE_SIZE` environment variables, as described in [Best Practices for Calling Imatest IT Libraries](http://www.imatest.com/2015/10/best-practices-for-calling-imatest-it-libraries/).


