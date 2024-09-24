#include <cstdlib>
#include <iostream>
//#include <matplot/matplot.h>
#include <vector>
#include <fstream>

using namespace std;

// double *T = nullptr;
// double *TT = nullptr;
// double *x = nullptr;
// double *y = nullptr;

int main()
{
    setlocale(0, "");

    int n;
    double h, xbeg, xend, t_end;
    const int k = 100, T0 = 10, Tc = 100;

    cout << "Введите количество точек на интервале (<=100) n=";
    cin >> n;

    if (n <= 0 || n > 100) {
        cout << "Некорректное значение n. Введите значение в пределах (0, 100]." << endl;
        return -1;
    }

    cout << "x и y-координаты начала интервалов=";
    cin >> xbeg;
    cout << "x и y-координаты конца интервалов=";
    cin >> xend;
    h = (xend - xbeg) / (n - 1); // 5/2 = 2.5

    cout << "Введите время распределения температуры: ";
    //cin >> Tn;
    cin >> t_end;

    std::vector<double> T(n);
    //double *T = new double[n];
    std::vector<double> TT(n);
    //double *TT = new double[n];

    //Определяем значения температуры на каждом временном слое
    T[0] = T0;
    T[n-1] = Tc;
    //Определяем поле температуры в начальный момент времени
    for(int i = 1; i < n-1; i++)
    {
        T[i] = T0;
    }

    std::vector<double> x(n);
    //double *x = new double[n];
    std::vector<double> y(n);
    //double *y = new double[n];
    for(int i = 0; i < n; i++)
    {
        x[i] = xbeg + i * h;
        y[i] = xbeg + i * h;
    }

    double time = 0;

    double tau = (h * h) / (4 * k);

    if( tau < (h * h) / (2 * k))
        cout << "Условие устойчивости выполняется" << endl;
    else 
        cout << "Условие устойчивости не выполняется" << endl;

    cout << tau << endl;

    do
    {
        time += tau;

        //Запоминаем поле температуры на предыдущем слое по времени
        for(int i = 0; i < n; i++)
        {
            TT[i] = T[i]; 
            
        }
        //Определяем неизвестное поле температуры по соотношениям
        for(int i = 1; i < n-1; i++)
        {
            T[i] = TT[i] + k*tau/(h*h)*(TT[i+1]-2*TT[i]+TT[i-1]);
        }
    }
    while(time <= t_end);

    std::ofstream graphic("Data_1.txt");

    for (int i = 0; i < n; i++)
    {
        cout << "x(" << i << ")= " << x[i] << "  " << "y(" << i << ")=" << y[i] << " "<< "T[" << i << "]= " << T[i] << endl;
        graphic << x[i] << " " << y[i] << " " << T[i] << endl;
    }
    
    //using namespace matplot;

    //surf(x, y, T);

    //plot(x,y,"-o");
    //show();
    
    //delete[] x;
    //delete[] y;
    //delete[] T;
    //delete[] TT;

    return 0;
}
