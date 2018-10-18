# qgis-quick-demo-app
Android GIS application with usage of QgsQuick.  


# Building on Linux

## Build QGIS with Quick Library

1. check out `master` branch from QGIS repository: https://github.com/qgis/QGIS

2. configure and compile QGIS with Quick library with flag -DWITH_QUICK=TRUE. 
Additionally, you can use to speed-up the build with some flags (see below). 
This will install the libs to `/home/bob/apps`:

   ```
   mkdir build
   cd build
   cmake \
   -GNinja \
   -DWITH_QUICK=TRUE \
   -DCMAKE_PREFIX_PATH=/home/bob/apps \
   -DWITH_GUI=FALSE \
   -DWITH_QTWEBKIT=FALSE \
   -DENABLE_TESTS=FALSE \
   -DWITH_BINDINGS=FALSE \
   ..
   ninja install
   ```

## Build the Demo App

1. check out this repository
2. copy `config.pri.default` to `config.pri`
3. adjust paths in `config.pri`
4. open `qgis-quick-demo-app.pro` in Qt Creator
5. build and run the application in Qt Creator


# Building on Android (Linux Cross-Compilation)
## Install Android build dependencies

Install:
- JDK v6 or later (OpenJDK is also good)
- Apache ant (http://ant.apache.org/bindownload.cgi) v1.8 or later
- Qt5 for android >= 5.9 with ARMv7 arch support
- Android SDK (https://developer.android.com/studio/index.html#downloads just command line tools, API 16+)
- Android NDK (Crystax NDK 10.3.2)

- For a Kubuntu 16.04 machine:

```
sudo apt-get install openjdk-8-jre
sudo apt-get install ant 1.8+
# download Qt Online Installer and install Qt5.x.y gcc_x64 (>=5.9), android ARMv7 build tools.
# install Android studio and with it install SDK version 16
# install 32bit libraries
sudo dpkg --add-architecture i386
sudo apt -get update
sudo apt-get install libc6:i386 libncurses5:i386 libstdc++6:i386 lib32z1 libbz2-1.0:i386
# Install Crystax NDK (https://www.crystax.net/en required by CMake)
```
  
When installed, there should be the following directory structure:
```
/opt/android-studio (GUI can be used to update/install other SDKs)
/opt/android/sdk (folder selected by android-studio for SDKs)
/opt/android/crystax-ndk-10.3.2 (NDK for building C++)
/opt/Qt (Qt 5.x.y for android arm and 64b desktop, QtCreator)
```

Now try to open QtCreator, setup Kit for Android developent (GCC compiler for NDK, select SDK in "devices", select Qt for android).
Once done, try to compile any QML example from QtCreator ("New project-> select QtQuick template), deploy on your device and test.

See (OSGeo4A)[https://github.com/opengisch/OSGeo4A/blob/master/README.md] and (Qt5)[http://doc.qt.io/qt-5/androidgs.html] for details.

## Build OSGeo4A

- Clone OSGeo4A repo from https://github.com/opengisch/OSGeo4A
- Create a file `config.conf` in the root folder by copying the `config.conf.default` and modify based on your folders. For example:

```
### Currently suggested versions ####
export ANDROIDNDKVER=r12b
export ANDROIDAPI=16
export ANDROIDSDK="/opt/android/sdk"
export ANDROIDNDK="/opt/android/crystax-ndk-10.3.2"
export QTSDK="/opt/Qt/5.x.y"
export O4A_qgis_DIR="path/to/QGIS"
export ARCHES=("armeabi-v7a" "x86")
```
  
Now run command `./distribute -mqgis -dqgis` to build the libraries.

## Build qgis_quickapp

To build the application:
1. check out this repository
2. copy `config.pri.default` to `config.pri` in root folder
3. adjust paths in `config.pri` (add path to your OSGeo4A repository)
4. open `qgis-quick-demo-app.pro` in Qt Creator and configure the project with your armeabi-v7a kit
5. build and run the application in Qt Creator

Note that if the application is build outside the main QGIS application for desktops, a build variable QML2_IMPORT_PATH has to be defined and target to the /qml subfolder of instalation folder (e.g. {QGIS_INSTALL_PATH}/qml).

Also make sure that Qt library contains all required dependencies.
  
## Deploy APK

If you have connected tablet/mobile with USB cable and enabled developer tools and USB debugging, you should be able to also
"Run" qgsquick_app from QtCreator. When launched, it asks which device to use and the application will be directly launched
on the device.

Otherwise, your APK is located in the build directory in subfolder android-build/build/outputs/apk/. APK should contain
all dependencies of the application, so when copied it can installed on the device.

# Building on MacOS
Requirements:
 - All QGIS dependencies for qgis-3 receipt from: https://github.com/OSGeo/homebrew-osgeo4mac

You can either build qgis_quick library or use ony from QGIS 3.4+ installation. Use same flags as for Linux.
Now you need to edit `config.pri` with paths to your QGIS installation and build with qmake

To run the application from build tree, you need to:

```
#!/bin/bash
APP=~/path/to/Applications

DYLD_FRAMEWORK_PATH=$DYLD_FRAMEWORK_PATH:$APP/QGIS.app/Contents/MacOS/lib:$APP/QGIS.app/Contents/Frameworks \
QML2_IMPORT_PATH=$APP/QGIS.app/Contents/MacOS/qml \
QGIS_PREFIX_PATH=$APP/QGIS.app/Contents/MacOS \
$APP/bin/qgis-quick-components-test
```

1. append QGIS Frameworks paths to `DYLD_FRAMEWORK_PATH`
2. append QML path for `qgis_quick` qml dir

# Building on Android (MacOS Cross-Compilation)

Same requirements as for Cross-Compilation for Android

Quick guide:
- `brew tap caskroom/versions` 
- `brew cask install java8`
- `brew install ant`
- `brew install bison`
- `sudo mkdir -p /opt; sudo chown <your name>:admin /opt`
- download SDK command line tools and unzip to `/opt/android-sdk`
- sdk: install lldb, build tools, platform android X, cmake, platform-tools
- download QT armv7 to `/opt/Qt`
- download crystax and install to `/opt/crystax-10.3.2`
- compile OSGeo4a from: https://github.com/opengisch/OSGeo4A
- open QtCreator -> Manage Kits -> add SDK and NDK. compilers should be autodetected
- enable connection on the device from MacOS when requested

