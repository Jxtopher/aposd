#
# @Author: *
# @License: *
# @Date: *
# @Version: 1
# @Description: Makefile
#

CC = mpic++ #g++
CFLAGS = -std=c++0x -Wall
OFLAGS = -Os # Optimization flags
#GFLAGS =  -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
DFLAGS = -DDEBUG
FLAGS = $(CFLAGS) $(OFLAGS) $(GFLAGS) $(DFLAGS)
EXEC_PATH = build
EXEC_NAME = AdaptivePortfolioSelectionDistributed 
# Adaptive Portfolio Selection Distributed

#INCLUDES =
#LIBS =
#OBJ_FILES = fichier_1.o fichier_2.o
#INSTALL_DIR = /usr/bin
#SOURCES_DIR = ./sources/

#all : $(EXEC_NAME)

#$(EXEC_NAME): $(SOURCES_DIR)main.cpp
all : ./sources/main.cpp
	$(CC) $(FLAGS) $< -o $(EXEC_NAME) -lboost_program_options

run:
	mpirun -np 4 $(EXEC_NAME)

unittest: ./sources/unitTest.cpp
	-mkdir $(EXEC_PATH)
	$(CC) $(FLAGS) $< -o $(EXEC_PATH)/unitTest_$(EXEC_NAME)  -lcppunit
	./$(EXEC_PATH)/unitTest_$(EXEC_NAME)
	

packagesDebain:
	apt-get install g++ build-essential libopenmpi-dev openmpi-bin

.PHONY: clean

clean:
	rm $(EXEC_NAME)

mrproper:
	rm $(EXEC_NAME) *.o


