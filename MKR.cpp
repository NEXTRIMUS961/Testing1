#include <iostream>
//#include <conio.h>
#include <stdio.h>
#include <math.h>

double getLambda(double x, double y) {
    return 0.25 <= x && x <= 0.65 &&
    0.1 <= y && y <= 0.25 ? 0.01 : 0.0001;
}

double avg(double left, double right) {
    return (left + right) / 2;
}

void solveThomas(const double* F,
                    const double* lambda,
                    const int length,
                    const double min,
                    const double max,
                    const double step,
                    const double timeStep,
                    double* y) {
    const auto coefficient = 1.0 / (2 * step * step);

    double A[length], B[length], C[length];
    for (int i = 0; i < length; ++i) {
        A[i] = - avg(lambda[i + 1], lambda[i]) / 2 * coefficient;
        B[i] = - avg(lambda[i + 1], lambda[i + 2]) / 2 * coefficient;
        C[i] = 1 / timeStep - A[i] - B[i];
    }

    double alpha[length], beta[length];
    alpha[0] = alpha[length - 1] = 0;
    beta[0] = min;
    beta[length - 1] = max;

    for (int i = 0; i < length - 2; ++i) {
        alpha[i + 1] = - B[i + 1] / (C[i + 1] + A[i + 1] * alpha[i]);
        beta[i + 1] = (F[i + 1] - A[i + 1] * beta[i]) / (C[i + 1] + A[i + 1] * alpha[i]);
    }

    y[length - 1] = max;
    for (int i = length - 2; i >= 0; i--) {
        y[i] = alpha[i] * y[i + 1] + beta[i];
    }
}

void restoreF(const double* y,
              const double* lambda,
              const int size,
              const double step,
              const double timeStep,
              double* F) {
    const double coefficient = 1 / (2 * step * step);
    for (int i = 1; i < size - 1; ++i) {
        double lambdaPlusHalf = avg(lambda[i], lambda[i + 1]);
        double lambdaMinusHalf = avg(lambda[i], lambda[i - 1]);
        double temp = lambdaPlusHalf * (y[i + 1] - y[i]) - lambdaMinusHalf * (y[i] - y[i - 1]);
        F[i] = y[i] / timeStep + temp * coefficient;
    }
}

int main(int argc, char** argv) {
    const int size = 10;
    const double timeStep = 0.1;
    const double tLeft = 600;
    const double tRight = 1200;
    const double xMin = 0;
    const double xMax = 1;
    const double yMin = 0;
    const double yMax = 0.5;
    const double xStep = (xMax - xMin) / size;
    const double yStep = (yMax - yMin) / size;

    // y - исходная температура
    double temperatureByX[size][size];
    double lambdaByX[size][size];
    // F
    double FByX[size][size];
    for (int i = 0; i < size; ++i) {
        auto x = i * xStep;
        for (int j = 0; j < size; ++j) {
            auto y = j * yStep;
            lambdaByX[i][j] = getLambda(x, y);
            temperatureByX[i][j] = 300;
            FByX[i][j] = 0;
        }
    }

    for (int i = 0; i < size; ++i) {
        restoreF(temperatureByX[i], lambdaByX[i], size, xStep, timeStep, FByX[i]);
    }

    // Меняю строки и столбцы
    double temperatureByY[size][size];
    double FByY[size][size];
    double lambdaByY[size][size];

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            temperatureByY[y][x] = temperatureByX[x][y];
            FByY[y][x] = FByX[x][y];
            lambdaByY[y][x] = lambdaByX[x][y];
        }
    }


    const int iterations = 100;
    for (int t = 0; t < iterations; ++t) {
        // Вычисляю yn+1/2
        for (int x = 0; x < size; ++x) {
            solveThomas(FByX[x], lambdaByX[x], size, tLeft, tRight, xStep, timeStep, temperatureByX[x]);
        }

        // Вычисляю Fn+1/2
        for (int x = 0; x < size; ++x) {
            restoreF(temperatureByX[x], lambdaByX[x], size, xStep, timeStep, FByX[x]);
        }

        // Обновляю значения температуры и F по Y
        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                FByY[y][x] = FByX[x][y];
            }
        }

        // Вычисляю yn+1
        for (int y = 0; y < size; ++y) {
            double x = xMin + y * xStep;
            solveThomas(FByY[y], lambdaByY[y], size, 600 * (1 + x), 600 * (1 + x * x * x), yStep, timeStep, temperatureByY[y]);
        }

        // Вычисляю Fn+1
        for (int y = 0; y < size; ++y) {
            restoreF(temperatureByY[y], lambdaByY[y], size, yStep, timeStep, FByY[y]);
        }


        // Обновляю значения температуры и F по X
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                temperatureByX[x][y] = temperatureByY[y][x];
                FByX[x][y] = FByY[y][x];
            }
        }
    }

}