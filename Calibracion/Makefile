all: calibration lectura

save_capture: CXXFLAGS=`pkg-config --cflags opencv` --std=c++11
save_capture: LDLIBS=`pkg-config --libs opencv`
save_capture: CC=g++
save_capture: save_capture.o

calibration: CXXFLAGS=`pkg-config --cflags opencv` --std=c++11
calibration: LDLIBS=`pkg-config --libs opencv`
calibration: CC=g++
calibration: calibration.o

lectura: CXXFLAGS=`pkg-config --cflags opencv` --std=c++11
lectura: LDLIBS=`pkg-config --libs opencv`
lectura: CC=g++
lectura: lectura.o
