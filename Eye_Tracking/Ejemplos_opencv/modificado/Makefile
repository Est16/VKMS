all: leer eye_detect

eye_detect: CXXFLAGS=`pkg-config --cflags opencv` --std=c++11
eye_detect: LDLIBS=`pkg-config --libs opencv`
eye_detect: CC=g++
eye_detect: eye_detect.o

leer: leer.o

