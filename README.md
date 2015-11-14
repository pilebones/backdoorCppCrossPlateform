# backdoorCppCrossPlateform

_/!\ Work in progress, not a stable release /!\_

##Main goal

A fork of my own project named : "pilebones/backdoorBash" (see: https://github.com/pilebones/backdoorBash) but instead of using Bash as programming language (Unix-like only) this new one will work on Windows too by using a C++ API (cross-plateform) developed from scratch (as much as possible).

##Requirements

###Windows
- Download and install "MinGW" with gcc compiler (see: http://www.mingw.org/wiki/howto_install_the_mingw_gcc_compiler_suite)
- Download and install "Boost C++ library" >= v1.59.X (see: http://www.boost.org/users/download/)
- After Boost C++ library installation : Build library for MinGW like below (see: http://www.boost.org/doc/libs/1_59_0/more/getting_started/unix-variants.html)
```bash
cd C:\local\boost_1_59_0
bootstrap.bat mingw
b2.exe install toolset=gcc
```

###Linux
```bash
# Archlinux:
 pacman -S boost-libs boost
# Debian/Ubuntu :
(sudo) apt-get install libboost-dev
```

##Compilation

```bash
git clone https://github.com/pilebones/backdoorCppCrossPlateform.git
cd backdoorCppCrossPlateform
```

###Windows
```bash
cmake -G "MinGW Makefiles" .
make
bin\main.exe -h
```

###Linux
```bash
cmake . && make
./bin/main.exe -h
```

##Usage

_TODO_