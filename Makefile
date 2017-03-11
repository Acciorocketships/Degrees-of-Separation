#
# Makefile for HW6
#
CXX      = clang++
CXXFLAGS = -g -Wall -Wextra
LDFLAGS  = -g

degrees: main.cpp people.cpp people.h course.cpp course.h person.cpp person.h
	${CXX} ${CXXFLAGS} -o degrees main.cpp people.cpp course.cpp person.cpp

clean:
	rm -f degrees

provide:
	provide comp15 hw6 main.cpp Makefile ReadMe.md people.cpp people.h course.cpp course.h person.cpp person.h
