EXECUTABLES=TestSimulation

#compilers
cc=g++

#flags
debug=-g
std=-std=c++11
#std=-std=gnu++11
thread=-pthread
#numeric=-fext-numeric-literals #enable numeric literals

EXPORT=win_x64
#script
all: ${EXECUTABLES}

#$@: first %, $^: all the rest object files, $<: %.cpp
%: %.cpp Matrix.h Rand.h Simulation.h Simulation.cpp
	${cc} ${debug} ${std} ${thread} -I. -o $@ $^

.PHONY: clean purify import export extract run
clean:
	rm -f ${EXECUTABLES}.o ${EXECUTABLES}.exe

purify:
	rm -f *.h *.cpp

import:
	cp include/* src/* .

export:
	mv *.exe ${EXPORT}

extract:
	make purify clean import && make all export purify

run:
	${EXPORT}/${EXECUTABLES}.exe
