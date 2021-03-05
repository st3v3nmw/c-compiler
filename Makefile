main: main.cpp
	g++ --std=c++17 -o main main.cpp && ./main

o: main.cpp
	g++ --std=c++17 -o main main.cpp && ./main $(f)

clean: 
	rm main