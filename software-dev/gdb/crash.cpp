// crash.cpp
#include <iostream>

int division(int a, int b) {
    return a / b;  // division par zéro si b == 0
}

void compute() {
    int x = 42;
    int y = 0;
    int result = division(x, y);  // CRASH ici
    std::cout << "Résultat : " << result << std::endl;
}

int main() {
    compute();
    return 0;
}

