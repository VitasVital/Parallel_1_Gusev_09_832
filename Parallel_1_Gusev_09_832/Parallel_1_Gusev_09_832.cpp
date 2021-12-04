#include <iostream>
#include <omp.h>
#include <random>

using namespace std;

void first_task()
{
#pragma omp parallel num_threads(4) //колличество нитей в группе
    {
        /*cout << "Hello World!\n";
        cout << "1234\n";*/
        int a = omp_get_num_threads();
        cout << a;
    }

    cout << endl;

    int s = 0;
#pragma omp parallel shared(s) //обща€ переменна€ на все потоки
    {
        cout << s;
    }

    cout << endl;

    int id = -1;
#pragma omp parallel private(id) //локальные переменные дл€ каждого потока
    {
        id = omp_get_thread_num(); //нужно инициализировать, чтобы заработало
        cout << id;
    }

    cout << endl;

    int p = 1;
#pragma omp parallel firstprivate(p) //инициализаци€ переменных из последовательного блока
    {
        cout << p;
    }

    cout << endl;

    int a[4] = { 1, 2, 3, 4 };
    int b[4] = { 5, 6, 7, 8 };
    int c[4] = { 0, 0, 0, 0 };
    int i = 0;
#pragma omp parallel shared(a, b, c, i)
    {
#pragma omp for
        for (i = 0; i < 4; i++)
            c[i] = a[i] + b[i];
    }
    cout << "Array" << endl;
    for (int j = 0; j < 4; j++)
        cout << c[j] << endl;


    cout << endl;


    double start = omp_get_wtime();
    const int N = 8;
    int d[N] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    int f[N] = { 5, 6, 7, 8, 9, 10, 11, 12 };
    int g[N] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int k = 0;
#pragma omp parallel for num_threads(8) shared(d, f, g, k)
    for (k = 0; k < N; k++)
    {
        g[k] = d[k] + f[k];
        cout << omp_get_thread_num() << endl;
    }
    double end = omp_get_wtime();

    double delta = end - start;
    cout << "Time " << delta << endl;

    cout << "Array" << endl;
    for (int q = 0; q < N; q++)
        cout << g[q] << endl;
}

void second_task()
{
    const int N = 10;
    int a[N] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int res = 0;
    int i = 0;
#pragma omp parallel for shared(a, res, i)
    for (i = 0; i < N; i++)
    {
        res += a[i];
    }

    cout << "Sum = " << res << endl;
}

void third_task()
{
    const int N = 10;
    int a[N] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int sum_a = 0;
    int const P = 8;
    int b[P] = { 0, 0, 0, 0, 0, 0, 0, 0 };
#pragma omp parallel
    for (int i = omp_get_thread_num(); i < N; i += omp_get_num_threads())
    {
        b[omp_get_thread_num()] += a[i];
    }
    for (int i = 0; i < P; i++)
    {
        sum_a += b[i];
    }
    cout << sum_a << endl;
}

double f(double x)
{
    return x + 1;
}

void fourth_task_first()
{
    double a = 10;
    double b = 20;
    int N = 100;
    double delta_x = (b - a) / N;
    double area = 0;
    int const P = 8;
    double threads_sum[P] = { 0, 0, 0, 0, 0, 0, 0, 0 };
#pragma omp parallel //без for, поэтому самосто€тельно распредел€ем по потокам
    for (int i = omp_get_thread_num(); i < N; i += omp_get_num_threads())
    {
        threads_sum[omp_get_thread_num()] += (((f(a + i * delta_x) + f(a + (i + 1) * delta_x)) / 2) * delta_x);
    }
    for (int i = 0; i < P; i++)
    {
        area += threads_sum[i];
    }
    cout << area << endl;
}

void fourth_task_second()
{
    double a = 10;
    double b = 20;
    int N = 100;
    double delta_x = (b - a) / N;
    double area = 0;
    int const P = 8;
    double threads_sum[P] = { 0, 0, 0, 0, 0, 0, 0, 0 };
#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        threads_sum[omp_get_thread_num()] += (((f(a + i * delta_x) + f(a + (i + 1) * delta_x)) / 2) * delta_x);
    }
    for (int i = 0; i < P; i++)
    {
        area += threads_sum[i];
    }
    cout << area << endl;
}

void fifth_task_first()
{
    const int N = 100000;
    int a[N];
    for (int i = 0; i < N; i++)
    {
        a[i] = i;
    }
    int res = 0;
    int i = 0;
#pragma omp parallel for reduction(+ : res)
    for (i = 0; i < N; i++)
    {
        res += a[i];
    }

    cout << "Sum = " << res << endl;
}

void fifth_task_second()
{
    double a = 10;
    double b = 20;
    int N = 100;
    double delta_x = (b - a) / N;
    double area = 0;
    int const P = 8;
#pragma omp parallel for reduction(+ : area)
    for (int i = 0; i < N; i++)
    {
        area += (((f(a + i * delta_x) + f(a + (i + 1) * delta_x)) / 2) * delta_x);
    }
    cout << area << endl;
}

void fifth_task_third()
{
    double a = 10;
    double b = 20;
    const int N = 100000;
    double area = 0;
    double y_max = 100;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> udistr1(a, b);
    uniform_real_distribution<> udistr2(0, y_max);

    double M = 0;
    for (int i = 0; i < N; i++)
    {
        double x = udistr1(gen);
        double y = udistr2(gen);
        if (f(x) >= y)
        {
            M += 1;
        }
    }
    area = (M / N) * ((b - a) * y_max);
    cout << area << endl;
}

void fifth_task_fourth()
{
    double a = 10;
    double b = 20;
    const int N = 100000;
    double area = 0;
    double y_max = 100;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> udistr1(a, b);
    uniform_real_distribution<> udistr2(0, y_max);

    double M = 0;
#pragma omp parallel for reduction(+ : M)
    for (int i = 0; i < N; i++)
    {
        double x = udistr1(gen);
        double y = udistr2(gen);
        if (f(x) >= y)
        {
            M += 1;
        }
    }
    area = (M / N) * ((b - a) * y_max);
    cout << area << endl;
}
//разобратьс€ с atomic и critical


void sixth_task_first()
{
    const int N = 100;
    double a[N];
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> udistr1(0, 1000);
    for (int i = 0; i < N; i++)
    {
        a[i] = udistr1(gen);
    }
    //#pragma omp parallel for даЄт неверные значени€
    for (int i = 0; i < N; i++)
    {
#pragma omp parallel for //теоретически может быть ошибка
        for (int j = i + 1; j < N; j++)
        {
            if (a[j] < a[i])
            {
                int k = a[i];
                a[i] = a[j];
                a[j] = k;
            }
        }
    }
    for (int i = 0; i < N; i++)
    {
        cout << a[i] << endl;;
    }
}

void sixth_task_second()
{
    const int N = 100;
    int a[N];
    for (int i = 0; i < N; i++)
    {
        a[i] = N - i;
    }
    //#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        int k;
        if (i % 2 == 0)
        {
            k = 0;
        }
        else
        {
            k = 1;
        }
#pragma omp parallel for
        for (int j = k; j < N - 1; j += 2)
        {
            if (a[j + 1] < a[j])
            {
                int k = a[j + 1];
                a[j + 1] = a[j];
                a[j] = k;
            }
        }
    }
    for (int i = 0; i < N; i++)
    {
        cout << a[i] << endl;;
    }
}

void sixth_task_third()
{
    const int N = 100;
    int a[N];
    for (int i = 0; i < N; i++)
    {
        a[i] = N - i;
    }
#pragma omp parallel
    for (int i = 0; i < N; i++)
    {
        int k;
        if (i % 2 == 0)
        {
            k = 0;
        }
        else
        {
            k = 1;
        }
#pragma omp for
        for (int j = k; j < N - 1; j += 2)
        {
            if (a[j + 1] < a[j])
            {
                int m = a[j + 1];
                a[j + 1] = a[j];
                a[j] = m;
            }
        }
    }
    for (int i = 0; i < N; i++)
    {
        cout << a[i] << endl;;
    }
}

void sixth_task_fourth()
{
    const int N = 8;
    int a[N];
    for (int i = 0; i < N; i++)
    {
        a[i] = i;
    }
    int step = 0;
    int length;
    if (N % 2 != 0)
    {
        length = N - 1;
        a[0] += a[N - 1];
    }
    else
    {
        length = N;
    }
    while (length >= 2 * step)
    {
        step += 1;
        //#pragma omp for
        for (int i = 0; i < length; i += step)
        {
            a[i] += a[i + step];
        }
    }
    cout << a[0] << endl;;
}

void task_1_consistent() //последовательна€ реализаци€
{
    const int N = 100;
    int array[N];
    cout << endl << "»нициализированный массив" << endl;
    for (int i = 0; i < N; i++)
    {
        array[i] = i + 1;
        cout << array[i] << endl;
    }

    cout << endl << "ћассив с частичными суммами" << endl;
    for (int i = 1; i < N; i++)
    {
        array[i] += array[i - 1];
    }
    for (int i = 0; i < N; i++)
    {
        cout << array[i] << endl;
    }
}

void task_1_OMP()
{
    const int N = 10;
    int array[N];
    const int num_all_threads = 4; //нужно указать количество потоков
    bool array_summed[num_all_threads] = { false }; //завершил ли каждый поток подсчЄт
    bool array_last[num_all_threads] = { false }; //заполнил ли последний элемент поток

    cout << endl << "»нициализированный массив" << endl;
    for (int i = 0; i < N; i++)
    {
        array[i] = i + 1;
        cout << array[i] << endl;
    }

    cout << endl << "ћассив с частичными суммами" << endl;
#pragma omp parallel shared(array_last) num_threads(num_all_threads)
    {
        int thread_num = omp_get_thread_num(); //номер потока

        int N_for_thread = N / (num_all_threads - 1); //количество элементов на каждый поток
        for (int i = N_for_thread * thread_num + 1; i < N_for_thread * (thread_num + 1) and i < N; i++) //поток суммирует свою часть
        {
            array[i] += array[i - 1];
        }
        array_summed[thread_num] = true;

        if (thread_num == 0)
        {
            array_last[thread_num] = true;
        }
        
        if (thread_num != 0) //если поток нулевой, то выход
        {
            while (true)
            {
                if (array_last[thread_num - 1] and array_summed[thread_num - 1])
                {
                    int index_last_element = N_for_thread * thread_num - 1; //последний индекс массива предыдущего потока
                    int start_i = N_for_thread * (thread_num + 1) - 1; //начинает суммировать с последнего элемента массива потока
                    if (thread_num == num_all_threads - 1) //если поток последний, то начинать с конца массива array
                    {
                        start_i = N - 1;
                    }
                    for (int i = start_i; i > index_last_element; i--)
                    {
                        array[i] += array[index_last_element];
                        array_last[thread_num] = true; //поток увеличил последний элемент следующей части
                    }
                    break;
                }
            }
        }
    }
    
    for (int i = 0; i < N; i++)
    {
        cout << array[i] << endl;
    }
}

void task_2()
{
    int M = 10;
	while (true)
	{
		
	}
}

int main()
{
    setlocale(LC_CTYPE, "Russian");
    //first_task();
    //second_task();
    //third_task();
    //fourth_task_first();
    //fifth_task_first();
    /*fifth_task_second();
    fifth_task_third();
    fifth_task_fourth();*/
    //sixth_task_first();
    //sixth_task_second();
    //sixth_task_third();
    //sixth_task_fourth();
    
    //task_1_consistent();
    task_1_OMP();
}