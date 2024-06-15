#include <iostream>
#include <random>
#include <chrono>

double** create2darray(int n, int m) {
    double** array = new double*[n];
    for (int i = 0; i < n; ++i) {
        array[i] = new double[m];
    }
    return array;
}

void deallocate2darray(double** array, int n) {
    for (int i = 0; i < n; ++i) {
        delete[] array[i];
    }
    delete[] array;
}

class Generator {
private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> distr;

public:
    Generator() : gen(rd()), distr(0.0, 1.0) {}

    double** generateRandom2DArray(int rows, int cols, double min, double max) {
        distr.param(std::uniform_real_distribution<>::param_type(min, max));
        double** array = create2darray(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                array[i][j] = distr(gen);
            }
        }
        return array;
    }

    double** generateRandom2DArrayNewDistr(int rows, int cols, double min, double max) {
        std::uniform_real_distribution<> new_distr(min, max);
        double** array = create2darray(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                array[i][j] = new_distr(gen);
            }
        }
        return array;
    }
};

int main() {
    Generator gen;
    int rows = 1000;
    int cols = 1000;
    double min = 0.0;
    double max = 10.0;
    double** array;

    // Measure time for reusing the distribution
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; i++) {
    double** array = gen.generateRandom2DArray(rows, cols, min, max);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time for reusing distribution: " << elapsed.count() << " seconds\n";

    // Measure time for creating a new distribution each time
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; i++) {
    array = gen.generateRandom2DArrayNewDistr(rows, cols, min, max);
    }

    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Time for creating new distribution each time: " << elapsed.count() << " seconds\n";

    return 0;
}

