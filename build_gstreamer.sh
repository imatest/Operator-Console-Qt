#!/bin/bash
VER=1.8.3
GSTREAMER_FOLDER=gst-1.8

SCRIPT_PATH=$(cd "$(dirname $0)" &&pwd -P)
PREFIX_DIR="$SCRIPT_PATH/$GSTREAMER_FOLDER/build"
LOG_DIR="$SCRIPT_PATH/$GSTREAMER_FOLDER/log"
SOURCE_PATH="$SCRIPT_PATH/$GSTREAMER_FOLDER/src"
DOWNLOAD_PATH="$SCRIPT_PATH/$GSTREAMER_FOLDER/download"

BASE_URL="https://gstreamer.freedesktop.org/src"


if [ ! -d "$LOG_DIR" ]; then
   mkdir -p "$LOG_DIR"
fi

if [ ! -d "$PREFIX_DIR" ]; then
   mkdir -p "$PREFIX_DIR"
fi

if [ ! -d "$SOURCE_PATH" ]; then
   mkdir -p "$SOURCE_PATH"
fi

echo "Downloading GStreamer $VER source code to $SOURCE_PATH"
for module in `echo gstreamer gst-plugins-base gst-plugins-good gst-plugins-bad gst-plugins-ugly gst-libav`; do
    TAR_FILE_ROOT=$module-$VER
    if [ ! -f "$DOWNLOAD_PATH/$TAR_FILE_ROOT.tar.xz" ]; then
        wget -P "$DOWNLOAD_PATH" "$BASE_URL/$module/$TAR_FILE_ROOT.tar.xz"
    fi 

    if [ ! -d "$SOURCE_PATH/$TAR_FILE_ROOT" ]; then
        mkdir "$SOURCE_PATH/$TAR_FILE_ROOT" 
    fi

    echo "Extracting $DOWNLOAD_PATH/$TAR_FILE_ROOT.tar.xz to $SOURCE_PATH"
    tar -xf "$DOWNLOAD_PATH/$TAR_FILE_ROOT.tar.xz" -C "$SOURCE_PATH"
done

cd "$SOURCE_PATH"

# Append the PKG_CONFIG_PATH with the path where the build process will deposit the *.pc files
# so that pkg-config can resolve the local gstreamer install while the plugins are being built
export PKG_CONFIG_PATH="$PREFIX_DIR/lib/pkgconfig":${PKG_CONFIG_PATH}

echo "Building Gstreamer to $PREFIX_DIR"
echo "All output from the build process is stored in $LOG_DIR"
for module in `echo gstreamer-$VER gst-plugins-base-$VER gst-plugins-good-$VER gst-plugins-bad-$VER gst-plugins-ugly-$VER gst-libav-$VER`; do
    INSTALL_LOG="$LOG_DIR/install.$module.log"
    BUILD_LOG="$LOG_DIR/build.$module.log"
    CONFIG_LOG="$LOG_DIR/config.$module.log"
    cd $module
    echo "Configuring build of $module:" `date`
    make clean > /dev/null 2>&1
    ./autogen.sh --disable-gtk-doc --disable-tests --disable-nls --prefix=$PREFIX_DIR > $CONFIG_LOG 2>&1
    
    echo "----------------------------------------------"  > $BUILD_LOG
    echo "Building $module:" `date` >> $BUILD_LOG
    echo " - Building $module:" `date`
    make -j4 >> $BUILD_LOG 2>&1
    
    echo "----------------------------------------------"  > $INSTALL_LOG
    echo "Installing $module:" `date` >> $INSTALL_LOG
    echo " - Installing $module:" `date`
    make install >> $INSTALL_LOG 2>&1
    cd - > /dev/null
done

echo "GStreamer $VER build complete"
echo "Please append"
echo "$PREFIX_DIR/lib:$PREFIX_DIR/lib/gstreamer-1.0"
echo "to your LD_LIBRARY_PATH variable"
