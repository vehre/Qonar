#!/bin/bash
# This installer assumes the appropriate qt framework is installed
# to build the target qtcreator at which the Qonar plug-in is targetting. 
# Follow instructions on https://doc.qt.io/ for the required qt framework
# to install. 

echo "Installing Qonar Plugin"

SOURCE_DIR="./source"
BUILD_DIR="./build"
QT_CREATOR_DIR="/opt/qt/creator"

# find qtcreator version
if [ -d $QT_CREATOR_DIR ]
then 
    QT_CREATOR_VERSION=$(find $QT_CREATOR_DIR -mindepth 1 -maxdepth 1 -type d -exec basename {} \;)
    echo "Found QtCreator Version" $QT_CREATOR_VERSION
else
    echo "Error! No QtCreator found."
fi

# download qtcreator source
echo "Downloading QtCreator source"
wget https://github.com/qt-creator/qt-creator/archive/v$QT_CREATOR_VERSION.tar.gz -P $SOURCE_DIR --no-check-certificate

# extract qtcreator source
echo "Extract QtCreator source"
tar -xf $SOURCE_DIR/v$QT_CREATOR_VERSION.tar.gz -C $SOURCE_DIR
export QTC_SOURCE=$SOURCE_DIR/qt-creator-$QT_CREATOR_VERSION
export QTC_BUILD=$BUILD_DIR/qt-creator-$QT_CREATOR_VERSION

# build qtcreator
mkdir $QTC_BUILD
pushd $QTC_BUILD
qmake -r $QTC_SOURCE
make -j$(nproc)
popd 

# download Qonar source
echo "Downloading Qonar source"
wget https://github.com/jpillon/Qonar/archive/master.zip -P $SOURCE_DIR --no-check-certificate
echo "Extract Qonar source"
tar -xf $SOURCE_DIR/master.zip -C $SOURCE_DIR

# build Qonar
QONAR_SOURCE=$SOURCE_DIR/Qonar-master
QONAR_BUILD=$BUILD_DIR/qonar
mkdir $QONAR_BUILD
pushd $QONAR_BUILD
qmake -r $QONAR_SOURCE/qonar.pro
make -j$(nproc)
popd
