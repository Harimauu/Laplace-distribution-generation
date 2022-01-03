#include <random>
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

//Класс для генерации случайных чисел
class RandomGenerator
{
public:
	static std::mt19937 & getMt19937();

private:
	RandomGenerator();
	~RandomGenerator() {}
	static RandomGenerator& instance();

	RandomGenerator(RandomGenerator const&) = delete;
	RandomGenerator& operator= (RandomGenerator const&) = delete;

	std::mt19937 mMt;
};

RandomGenerator::RandomGenerator() {
	std::random_device rd;

	if (rd.entropy() != 0) {
		std::seed_seq seed{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
		mMt.seed(seed);
	}
	else {
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		mMt.seed(seed);
	}
}

RandomGenerator& RandomGenerator::instance() {
	static RandomGenerator s;
	return s;
}

std::mt19937 & RandomGenerator::getMt19937() {
	return RandomGenerator::instance().mMt;
}

//Среднее арифмитическое
double arithmeticMean(double *array, int length)
{
    double sum = 0;

    for (int i = 0; i < length; i++)
    {
        sum += array[i];
    }

    return (sum / length);
}

double D(double *array, double mean, int length)
{
    double sum = 0;

    for (int i = 0; i < length; i++)
    {
        sum += pow((array[i] - mean), 2);
    }

    return (sum / length);
}

double asymmetry(double *array, double mean, double D, int length)
{
    double sum = 0;

    for (int i = 0; i < length; i++)
    {
        sum += pow((array[i] - mean), 3);
    }

    return (sum / (length * pow(D, 3/2)));
}

double kurtosis(double *array, double mean, double D, int length)
{
    double sum = 0;

    for (int i = 0; i < length; i++)
    {
        sum += pow((array[i] - mean), 4);
    }

    return (sum / (length * pow(D, 2)));
}

int main()
 {
    int N;
    cout << "Enter N: ";
    cin >> N;
    cout << endl;
    const double v = 1.5;
    const double e = 2.718;
    double b = pow((2/(e * v)), 1/v);
    const double ep = 0.3;
    double m = 1.5;
    double s = 0;

	//Генерация псевдослучайной величины u
	std::mt19937 &mt = RandomGenerator::getMt19937();
	std::uniform_real_distribution<double> dist(0.0, 1.0);
    double u[N] = {};
    int i = 0;
    size_t j = 0;
    while (i < N)
    {
        u[i] = dist(mt);
        //cout << u[i] << "\n";
        i++;
        j++;
    }

	std::cout << "\n";

	//Генерация псевдослучайной величины r
	std::mt19937 &mt2 = RandomGenerator::getMt19937();
	std::uniform_real_distribution<double> dist2(0.0, 1.0);
    double r[N] = {};
    i = 0;
    j = 0;
    while (i < N)
    {
        r[i] = dist2(mt2);
        //cout << r[i] << "\n";
        i++;
        j++;
    }

    //Генерация псевдослучайной величины r1
	std::mt19937 &mt3 = RandomGenerator::getMt19937();
	std::uniform_real_distribution<double> dist3(0.0, 1.0);
    double r1[N] = {};
    i = 0;
    j = 0;
    while (i < N)
    {
        r1[i] = dist3(mt3);
        //cout << r[i] << "\n";
        i++;
        j++;
    }

    //Моделирование случайной величины x, имеющей стандартное чистое распределение
    i = 0;
    double x[N] = {};
    while (i < N)
    {
        x[i] = (b*(2*r[i] - 1))/u[i];

        //Если условие выполняется, то генерация i-го члена массива (выборки) происходит заново
        if(pow(u[i], 2) > exp(-(pow(abs(x[i]), v))))
        {
            u[i] = dist(mt);
            r[i] = dist(mt2);
            continue;
        }
        //cout << x[i] << endl;
        i++;
    }

    i = 0;
    while (i < N)
    {
        u[i] = dist(mt);
        //cout << u[i] << "\n";
        i++;
        j++;
    }

    i = 0;
    while (i < N)
    {
        r[i] = dist2(mt2);
        //cout << r[i] << "\n";
        i++;
        j++;
    }

    ofstream clearY("ClearY.dat");
    clearY << "ClearY\n0 " << N << "\n";

    ofstream dirtyY("DirtyY.dat");
    dirtyY << "DirtyY\n0 " << N << "\n";

    //Моделирование случайной величины y, имеющей стандартное симметричное засорённое распределение
    i = 0;
    double y[N] = {};
    while (i < N)
    {
        y[i] = (b*(2*r[i] - 1))/u[i];

        //Моделирование чистых значений по тому же принципу, что и в цикле до этого
        if(pow(u[i], 2) > exp(-(pow(abs(y[i]), v))))
        {
            u[i] = dist(mt);
            r[i] = dist2(mt2);
            continue;
        }

        //Моделирование засорённых значений, на которые влияют параметры масштаба и сдвига, если условие выполняется
        if(r1[i] > (1 - ep))
        {
            y[i] = s + m*y[i];
            dirtyY << y[i] << endl;
        }

        else clearY << y[i] << endl;
        //cout << y[i] << endl;
        i++;
    }

    i = 0;
    while (i < N)
    {
        u[i] = dist(mt);
        //cout << u[i] << "\n";
        i++;
        j++;
    }

    i = 0;
    while (i < N)
    {
        r[i] = dist2(mt2);
        //cout << r[i] << "\n";
        i++;
        j++;
    }

    i = 0;
    while (i < N)
    {
        r1[i] = dist3(mt3);
        //cout << u[i] << "\n";
        i++;
        j++;
    }

    ofstream clearZ("ClearZ.dat");
    clearZ << "ClearZ\n0 " << N << "\n";

    ofstream dirtyZ("DirtyZ.dat");
    dirtyZ << "DirtyZ\n0 " << N << "\n";

    //Моделирование случайной величины y, имеющей стандартное асимметричное засорённое распределение
    m = 2;
    s = 1;
    i = 0;
    double z[N] = {};
    while (i < N)
    {
        z[i] = (b*(2*r[i] - 1))/u[i];

        //Моделирование чистых значений по тому же принципу, что и в цикле до этого
        if(pow(u[i], 2) > exp(-(pow(abs(z[i]), v))))
        {
            u[i] = dist(mt);
            r[i] = dist2(mt2);
            continue;
        }

        //Моделирование засорённых значений, на которые влияют параметры масштаба и сдвига, если условие выполняется
        if(r[i] > (1 - ep))
        {
            z[i] = s + m*z[i];
            dirtyZ << z[i] << endl;
        }

        else clearZ << z[i] << endl;

        //cout << z[i] << endl;
        i++;
    }

    //Вывод значений чистой и засорённой выборок, элементы которых не совпадают друг с другом
    /*int k = 0;
    for(i = 0; i < N; i++)
    {
        if(x[i] != y[i])
        {cout << "#" << k << " x[" << i << "] = " << x[i] << " and y[" << i << "] = " << y[i] << endl;
        k++;}
    }*/

    //Запись значений чистой выборки
    ofstream Rx("ResultsX.dat");
    Rx << "ResultsX\n0 " << N << "\n";
    for (int i = 0; i < N; i++)
    {
        Rx << x[i] << endl;
    }

    //Запись значений засорённой выборки
    ofstream Ry("ResultsY.dat");
    Ry << "ResultsY\n0 " << N << "\n";
    for (int i = 0; i < N; i++)
    {
        Ry << y[i] << endl;
    }

    //Запись значений засорённой выборки
    ofstream Rz("ResultsZ.dat");
    Rz << "ResultsZ\n0 " << N << "\n";
    for (int i = 0; i < N; i++)
    {
        Rz << z[i] << endl;
    }

    cout << endl;

    //Арифмитическое среднее для выборок
    cout << "Arithmetic mean for X is: " << arithmeticMean(x, N) << endl;
    cout << "Arithmetic mean for Y is: " << arithmeticMean(y, N) << endl;
    cout << "Arithmetic mean for Z is: " << arithmeticMean(z, N) << endl;

    cout << endl;

    //Выборочная дисперсия
    cout << "D for X is: " << D(x, arithmeticMean(x, N), N) << endl;
    cout << "D for Y is: " << D(y, arithmeticMean(y, N), N) << endl;
    cout << "D for Z is: " << D(z, arithmeticMean(z, N), N) << endl;

    cout << endl;

    //Выборочная асимметрия
    cout << "Asymmetry for X is: " << asymmetry(x, arithmeticMean(x, N), D(x, arithmeticMean(x, N), N), N) << endl;
    cout << "Asymmetry for Y is: " << asymmetry(y, arithmeticMean(y, N), D(y, arithmeticMean(y, N), N), N) << endl;
    cout << "Asymmetry for Z is: " << asymmetry(z, arithmeticMean(z, N), D(z, arithmeticMean(z, N), N), N) << endl;

    cout << endl;

    //Выборочный эксцесс
    cout << "Kurtosis for X is: " << kurtosis(x, arithmeticMean(x, N), D(x, arithmeticMean(x, N), N), N) << endl;
    cout << "Kurtosis for Y is: " << kurtosis(y, arithmeticMean(y, N), D(y, arithmeticMean(y, N), N), N) << endl;
    cout << "Kurtosis for Z is: " << kurtosis(z, arithmeticMean(z, N), D(z, arithmeticMean(z, N), N), N) << endl;

    cout << endl;

    //Медиана
    sort(x, x+N);
    cout << "X median is " << (x[N/2] + x[N/2 - 1]) / 2 << endl;
    sort(y, y+N);
    cout << "Y median is " << (y[N/2] + y[N/2 - 1]) / 2 << endl;
    sort(z, z+N);
    cout << "Z median is " << (z[N/2] + z[N/2 - 1]) / 2 << endl;

	return 0;
}
