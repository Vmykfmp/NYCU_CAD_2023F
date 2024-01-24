CC = g++
FLAGS = -fdiagnostics-color=always -g -Wall -std=c++11

TOP  = CAD_HW3
# INPUT_FILE = example
# INPUT_FILE = c100000
# INPUT_FILE = c54321
INPUT_FILE = c432
# INPUT_FILE = c17
LIB_FILE = test_lib

# FILE_PATH = ./input
INPUT_FILE_PATH  = ./hidden/input
GOLDEN_FILE_PATH = ./hidden/golden

all:
	clear
	$(CC) -c $(FLAGS) $(TOP).cpp
	$(CC) $(FLAGS) $(TOP).cpp -o $(TOP)

sim:
	rm -rf *.txt
	time ./$(TOP) ./input/$(INPUT_FILE).v ./input/$(INPUT_FILE).pat ./input/$(LIB_FILE).lib

run_hidden_1:
	rm -rf *.txt
	time ./$(TOP) $(INPUT_FILE_PATH)/hidden_1.v $(INPUT_FILE_PATH)/hidden_1.pat $(INPUT_FILE_PATH)/$(LIB_FILE).lib
	
run_hidden_2:
	rm -rf *.txt
	time ./$(TOP) $(INPUT_FILE_PATH)/hidden_2.v $(INPUT_FILE_PATH)/hidden_2.pat $(INPUT_FILE_PATH)/$(LIB_FILE).lib

run_hidden_3:
	rm -rf *.txt
	time ./$(TOP) $(INPUT_FILE_PATH)/hidden_3.v $(INPUT_FILE_PATH)/hidden_3.pat $(INPUT_FILE_PATH)/$(LIB_FILE).lib

clear:
	rm -rf *.txt
	rm -rf *.o
	rm -rf $(TOP)