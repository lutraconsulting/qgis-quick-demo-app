# qgis-quick-demo-app
Android GIS application with usage of QgsQuick 


# Building on Linux

## Build QGIS with Quick Library

1. check out `qgis-quick-library` branch from Peter Petrik's QGIS repository: https://github.com/PeterPetrik/QGIS

   (hopefully soon the branch will be merged to QGIS master)

2. configure and compile QGIS with Quick library - this will install the libs to `/home/bob/apps`:

   ```
   mkdir build
   cd build
   cmake -G Ninja -DWITH_QUICK=TRUE -DWITH_GUI=FALSE -DWITH_DESKTOP=FALSE -DCMAKE_PREFIX_PATH=/home/bob/apps ..
   ninja install
   ```

## Build the Demo App

1. check out this repository
2. copy `config.pri.default` to `config.pri`
3. adjust paths in `config.pri`
4. open `qgis-quick-demo-app.pro` in Qt Creator
5. build and run the application in Qt Creator


# Building on Android

TODO: verify this is still correct

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
# download Qt Online Installer and install Qt5.9.2 gcc_x64, android ARMv7 build tools.
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
/opt/Qt (Qt 5.9.2 for android arm and 64b desktop, QtCreator)
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
export QTSDK="/opt/Qt/5.9.2"
export O4A_qgis_DIR="path/to/QGIS"
export ARCHES=("armeabi-v7a" "x86")
```
  
Now run command `./distribute -mqgis -dqgis` to build the libraries.

## Build qgis_quickapp

To build the application:
  
- cd QGIS/tests/src/quickgui/app/
- cp config.pri.default config.pri
- modify config.pri (add path to your OSGeo4A repository)
- open QGIS/tests/src/quickgui/app/qgis_quickapp.pro in QtCreator and configure the project with your armeabi-v7a kit
- compile the application

Note that if the application is build outside the main QGIS application for desktops, a build variable QML2_IMPORT_PATH has to be defined and target to the /qml subfolder of instalation folder (e.g. {QGIS_INSTALL_PATH}/qml).

Also make sure that Qt library contains all required dependencies.
  
## Deploy APK

If you have connected tablet/mobile with USB cable and enabled developer tools and USB debugging, you should be able to also
"Run" qgsquick_app from QtCreator. When launched, it asks which device to use and the application will be directly launched
on the device.

Otherwise, your APK is located in the build directory in subfolder android-build/build/outputs/apk/. APK should contain
all dependencies of the application, so when copied it can installed on the device.
