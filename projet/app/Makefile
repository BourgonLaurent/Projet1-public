# Compile & flash C/C++ for AVR.
#
# Team #4546
# 	Catalina Andrea Araya Figueroa
# 	Mehdi Benouhoud
# 	Laurent Bourgon
# 	Ihsane Majdoubi
#
# March 23, 2023
#
# Rules:
# 	`make all` (default): build the project
# 	`make install`: build & flash the project
#	`make debug`: build & flash the project with debug flags
# 	`make clean`: remove compilation artifacts

# Project name (in one word)
PROJECTNAME=app

# Source files (delimiter: space)
PRJSRC=$(wildcard *.cpp) $(wildcard **/*.cpp) $(wildcard **/**/*.cpp)

# Additional inclusions (ex: -I/path/to/mydir)
INC=-I../

# Library directory (must end with a '/')
LIBDIR=../lib/
# Library name
LIBNAME=1900

# Library flags
LIBS=-l$(LIBNAME) -L$(LIBDIR)

include ../../Makefile.exec.common

$(OBJDEPS): $(LIBDIR)lib$(LIBNAME).a
