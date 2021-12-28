pacman -Syu
pacman -Sy --noconfirm --needed base-devel git subversion mercurial
pacman -Sy --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-cmake mingw-w64-x86_64-clang
pacman -Sy --noconfirm --needed mingw-w64-x86_64-qt5 mingw-w64-x86_64-qt-creator mingw-w64-x86_64-qwt-qt5
pacman -Sy --noconfirm --needed mingw-w64-x86_64-boost mingw-w64-x86_64-fmt mingw-w64-x86_64-fftw mingw-w64-x86_64-python mingw-w64-x86_64-libxml2 
pacman -Sy --noconfirm --needed mingw-w64-i686-toolchain mingw-w64-i686-gcc mingw-w64-i686-gdb mingw-w64-i686-cmake mingw-w64-i686-clang
pacman -Sy --noconfirm --needed mingw-w64-i686-qt5 mingw-w64-i686-qt-creator mingw-w64-i686-qwt-qt5
pacman -Sy --noconfirm --needed mingw-w64-i686-boost mingw-w64-i686-fmt mingw-w64-i686-fftw mingw-w64-i686-python mingw-w64-i686-libxml2
