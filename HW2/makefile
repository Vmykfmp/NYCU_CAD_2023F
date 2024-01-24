CC = g++
FLAGS = -fdiagnostics-color=always -g -Wall -std=c++11

TOP  = CAD_HW2
# INPUT_FILE = example
# INPUT_FILE = c100000
# INPUT_FILE = c54321
# INPUT_FILE = c432
INPUT_FILE = c17
LIB_FILE = test_lib

all:
	clear
	$(CC) -c $(FLAGS) $(TOP).cpp
	$(CC) $(FLAGS) $(TOP).cpp -o $(TOP)

sim:
	time ./$(TOP) ./Test_Case/$(INPUT_FILE).v -l ./Test_Case/$(LIB_FILE).lib

clear:
	rm -rf *.o