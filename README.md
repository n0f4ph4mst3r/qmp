# Qmp
Description
-----------
Simple multimedia player that can play audio and or video files using various codecs.

Build
-----
## Prerequisites

* **cmake** >= 3.31.2
* **vcpkg** more recent than 2025-01-01 (e.g. commit id [4b6c50d962cc20aaa3ef457f8ba683b586263cfb](https://github.com/microsoft/vcpkg/commit/4b6c50d962cc20aaa3ef457f8ba683b586263cfb))
* **A working compiler supporting C++ 17 functions



### Cmake

On Windows, please use the [prebuilt binaries](https://cmake.org/download/). Ensure you select one of the options to add cmake to the system path.

On Linux, `cmake` is usually available from the system package manager. Alternatively, `sudo pip3 install cmake` can be used to install the latest version of CMake.

### Vcpkg
Clone the repo with the `--recurse-submodules` flag

    git clone --recurse-submodules https://github.com/n0f4ph4mst3r/qmp

Run `./vcpkg/bootstrap-vcpkg.sh` or `.\vcpkg\bootstrap-vcpkg.bat`.

## Building

### Windows
I build using Microsoft Visual Studio 2022. However, it is likely possible to build it with Visual Studio 2019.


### Linux
I use Clang 18, but any compilers supporting C++ 17 functions should work. Also, `qtbase` requires packages from the system package manager. They
can be installed on Ubuntu systems via:

	sudo apt-get install '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev libegl1-mesa-dev

Qt does not maintain a comprehensive package list for all distributions. Some distributions, such as Red Hat and its derivatives, may require additional repositories for certain packages (e.g., EPEL for RHEL). Consult your distribution's documentation for specific package names. 

#### Building on Windows

1. Clone this repository from GitHub project.

For example, in the git-bash run:

    git clone --recurse-submodules https://github.com/n0f4ph4mst3r/qmp

2. Run `.\vcpkg\bootstrap-vcpkg.bat -disableMetrics`.

3. Configure project using CMake:

       $ mkdir build && cd build
       $ cmake .. -G "Visual Studio 17 2022"

4. Open Visual Studio project:

       $ .\Qmp.sln

    and build using the IDE.

5. Deploy Qt libraries using `windeployqt.exe`:

        $ .\vcpkg_installed\x64-windows-mixed\tools\Qt6\bin\windeployqt.exe .\Release\qmp.exe

For debug configuration use `windeployqt.debug.bat`:

        $ .\vcpkg_installed\x64-windows-mixed\tools\Qt6\bin\windeployqt.debug.bat .\Debug\qmp.exe

#### Linux 

1. Clone repository from GitHub project.

       git clone --recurse-submodules https://github.com/n0f4ph4mst3r/qmp

2. Run `./vcpkg/bootstrap-vcpkg.sh -disableMetrics`.

3. Configure project using CMake:

       $ mkdir build && cd build
       $ cmake .. -G "Unix Makefiles" 
	   
   `Ninja` generator working too:

       $ cmake .. -G "Ninja" 
	   
	Use `-DCMAKE_BUILD_TYPE=Debug` for debug configuration.

4. Install project
   
        $ cmake --install . 

5. Test the build:

       $ cd ../install
       $ ./qmp

#### *Troubleshooting*

*Qt* package for Linux having problem with *syncqt* file. For solve it, just create symlink via:

    $ ln -sf /path/to/your/build/vcpkg_installed/x64-linux/tools/Qt6/bin/syncqt /path/to/your/build/vcpkg_installed/x64-linux/bin/syncqt