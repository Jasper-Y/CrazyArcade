# make sure you have the data files and the YS libraries installed

des_path="./runs/"
install_path="test.app/Contents/MacOS/test"

while getopts "p:" optname
do
    case "${optname}" in
        "p")
            des_path=$OPTARG
    esac
done

echo "Copying files to $des_path ..."
cp Src/* $des_path
cp Inc/* $des_path
cp Data/Map/* $des_path
cp Data/Sound/* $des_path

cd $des_path
clang++ map.cpp main.cpp fssimplewindowcpp.cpp fssimplewindowobjc.o \
        ysglfontdata.c yspng.cpp yssimplesound.cpp \
        player.cpp bubble.cpp \
        yssimplesound_macosx_objc.o -framework cocoa -framework opengl \
        -framework AVFoundation -o $install_path -std=c++11 -Wno-everything
