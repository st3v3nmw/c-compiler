main: main.cpp
	g++ --std=c++17 defs.cpp icg.cpp parser.cpp scanner.cpp -o main main.cpp && ./main

o: main.cpp
	g++ --std=c++17 defs.cpp icg.cpp parser.cpp scanner.cpp -o main main.cpp && ./main $(f)

clean: 
	rm main