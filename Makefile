#
# @Author: Jxtopher
# @License: *
# @Date: 2018-10
# @Version: 1
# @Description: Makefile
#
.PHONY: all test check run
MAKEFLAGS += --no-print-directory


all : check 
	@cd build && cmake ..
	@cd build && make

test: check
	export LD_LIBRARY_PATH=/usr/local/lib
	@cd build && cmake ..
	@cd build && make
	@cd build && make test

run: 
	export LD_LIBRARY_PATH=/usr/local/lib
	./build/aposd-Release -c configuration/aposd-webApps.json -ldebug

checks:
	@cd build && make clang-format
	@cd build && make cppcheck

tools-required:
	@#build-essential
	@hash cmake 2> /dev/null || sudo apt-get install -y cmake 2> /dev/null || ( echo >&2 "Please install cmake" ; exit -1)


