## C-Language Compiler Implementation

A compiler for a [subset](specification.txt) of the C programming language.

See the grammar [here](specification.txt).

### Project Requirements
1. C++17 or later (g++)

### Projet Setup & Technologies
1. ANTLR
2. C++

### Project Launch
Clone the project into your computer. You should have C++17 or g++ installed. Lauch the project by running the `main.cpp` file. It would be easier to run the project using JetBrain's CLION IDE. The test files are foud in the `tests` folder. To switch from one testcase to another, change the name in the test file specifier on the `main.cpp` file.

### Generating testcases using ANTLR

1. Install [ANTLR4](https://www.antlr.org/download.html)
2. Install [grammarinator](https://github.com/renatahodovan/grammarinator)
3. Run [generate.sh](generate.sh) which generates some test cases & runs them

### Project Status

### Illustrations
1. Scanner output
![Scanner output](../output-illustrations/output1.PNG)