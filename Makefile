all: main.cpp
	g++ --std=c++17 -o main main.cpp && ./main

clean: 
	rm main