#include <iostream>
#include <omp.h>
#include <random>
#include <ctime>
#include <fstream>

using namespace std;

void task_1_consistent(int n) //последовательная реализация
{
    cout << "Последовательная реализация" << endl;
    int* array = new int[n];
    cout << endl << "Инициализированный массив" << endl;
    for (int i = 0; i < n; i++)
    {
        array[i] = i + 1;
        cout << array[i] << endl;
    }

    cout << endl << "Массив с частичными суммами" << endl;
    for (int i = 1; i < n; i++)
    {
        array[i] += array[i - 1];
    }
    for (int i = 0; i < n; i++)
    {
        cout << array[i] << endl;
    }
    delete[] array;
}

void task_1_OMP(int n, int num_all_threads)
{
    cout << "Параллельная реализация" << endl;
    int* array = new int[n];
    bool* array_summed = new bool[num_all_threads];
    bool* array_last = new bool[num_all_threads];
    for (int i = 0; i < num_all_threads; i++)
    {
        array_summed[i] = false;
        array_last[i] = false;
    }

    cout << endl << "Инициализированный массив" << endl;
    for (int i = 0; i < n; i++)
    {
        array[i] = i + 1;
        cout << array[i] << endl;
    }

    cout << endl << "Массив с частичными суммами" << endl;
#pragma omp parallel shared(array_last) num_threads(num_all_threads)
    {
        int thread_num = omp_get_thread_num(); //номер потока

        int N_for_thread = n / (num_all_threads - 1); //количество элементов на каждый поток
        for (int i = N_for_thread * thread_num + 1; i < N_for_thread * (thread_num + 1) and i < n; i++) //поток суммирует свою часть
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
                        start_i = n - 1;
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
    
    for (int i = 0; i < n; i++)
    {
        cout << array[i] << endl;
    }
    delete[] array;
}

void task_2_consistent(int n) //последовательная реализация
{
    int* array = new int[n];
    for (int i = 1; i < n; i++)
    {
        array[i] += array[i - 1];
    }
    delete[] array;
}

void task_2_OMP(int n, int num_all_threads)
{
    int* array = new int[n];
    bool* array_summed = new bool[num_all_threads];
    bool* array_last = new bool[num_all_threads];
    for (int i = 0; i < num_all_threads; i++)
    {
        array_summed[i] = false;
        array_last[i] = false;
    }
    
#pragma omp parallel shared(array_last) num_threads(num_all_threads)
    {
        int thread_num = omp_get_thread_num(); //номер потока

        int N_for_thread = n / (num_all_threads - 1); //количество элементов на каждый поток
        for (int i = N_for_thread * thread_num + 1; i < N_for_thread * (thread_num + 1) and i < n; i++) //поток суммирует свою часть
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
                        start_i = n - 1;
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
    
    delete[] array;
}

void task_2(int num_all_threads)
{
    int M = 10;
    unsigned int start_time_consistent;
    unsigned int end_time_consistent;
    unsigned int search_time_consistent;

    unsigned int start_time_OMP;
    unsigned int end_time_OMP;
    unsigned int search_time_OMP;
	while (true)
	{
        start_time_consistent = clock();
        task_2_consistent(M);
        end_time_consistent = clock();
        search_time_consistent = end_time_consistent - start_time_consistent;

        start_time_OMP = clock();
        task_2_OMP(M, num_all_threads);
        end_time_OMP = clock();
        search_time_OMP = end_time_OMP - start_time_OMP;
        if (search_time_OMP < search_time_consistent)
        {
            cout << "M = " << M << endl;
            ofstream fout("task_2_M.txt"); // создаём объект класса ofstream для записи и связываем его с файлом cppstudio.txt
            fout << M; // запись строки в файл
            fout.close(); // закрываем файл
            
            break;
        }
        M += 1;
	}
}

void task_3(int n, int num_all_threads)
{
    int M;
    ifstream file("task_2_M.txt");
    file >> M;
    
    if (n >= M)
    {
        cout << "Вычисляется параллельно" << endl;
        task_1_OMP(n, num_all_threads);
        cout << "Вычисляется параллельно" << endl;
    }
    else
    {
        cout << "Вычисляется последовательно" << endl;
        task_1_consistent(n);
        cout << "Вычисляется последовательно" << endl;
    }
}

void task_4(int num_all_threads)
{
    int M;
    ifstream file("task_2_M.txt");
    file >> M;
    
    int N = M * 2;
    cout << "N = " << N << endl;

    unsigned int start_time_OMP;
    unsigned int end_time_OMP;
    unsigned int search_time_OMP;
    for (int i = 2; i <= num_all_threads; i++)
    {
        start_time_OMP = clock();
        task_2_OMP(N, i);
        end_time_OMP = clock();
        search_time_OMP = end_time_OMP - start_time_OMP;
        cout << "Thread count = " << i << " time = " << search_time_OMP << endl;
    }
}

int main()
{
    setlocale(LC_CTYPE, "Russian");
    int num_all_threads = 8; //указать количество потоков

    int n = 1000;
    //task_1_consistent(n);
	//task_1_OMP(n, num_all_threads);

    //task_2(num_all_threads);
    
    //task_3(n, num_all_threads);

    task_4(num_all_threads);
}