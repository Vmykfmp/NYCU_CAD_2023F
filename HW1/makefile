CC = g++
FLAGS = -fdiagnostics-color=always -g -Wall -std=c++11
TOP  = CAD_HW1
INPUT_FILE = 7

all:
	$(CC) -c $(FLAGS) $(TOP).cpp
	$(CC) $(FLAGS) $(TOP).cpp -o $(TOP).o

sim:
	time ./$(TOP).o ./testcase/input$(INPUT_FILE).txt ./testcase/output$(INPUT_FILE).txt

clear:
	rm -rf *.o