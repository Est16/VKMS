all: prueba prueba2

prueba: CXXFLAGS=`pkg-config --cflags opencv` --std=c++11
prueba: LDLIBS=`pkg-config --libs opencv`
prueba: CC=g++
prueba: prueba.o

prueba2: CXXFLAGS=`pkg-config --cflags opencv` --std=c++11
prueba2: LDLIBS=`pkg-config --libs opencv`
prueba2: CC=g++
prueba2: prueba2.o



