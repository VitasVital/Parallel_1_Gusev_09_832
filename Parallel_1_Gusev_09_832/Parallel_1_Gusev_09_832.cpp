#include <iostream>
#include <omp.h>
#include <random>
#include <ctime>
#include <fstream>

using namespace std;

void task_1_consistent(int N) //последовательная реализация
{
    int* array = new int[N];
    cout << endl << "Инициализированный массив" << endl;
    for (int i = 0; i < N; i++)
    {
        array[i] = i + 1;
        cout << array[i] << endl;
    }

    cout << endl << "Массив с частичными суммами" << endl;
    for (int i = 1; i < N; i++)
    {
        array[i] += array[i - 1];
    }
    for (int i = 0; i < N; i++)
    {
        cout << array[i] << endl;
    }
    delete[] array;
}

void task_1_OMP(int N, int num_all_threads)
{
    int* array = new int[N];
    bool* array_summed = new bool[num_all_threads];
    bool* array_last = new bool[num_all_threads];
    for (int i = 0; i < num_all_threads; i++)
    {
        array_summed[i] = false;
        array_last[i] = false;
    }

    cout << endl << "Инициализированный массив" << endl;
    for (int i = 0; i < N; i++)
    {
        array[i] = i + 1;
        cout << array[i] << endl;
    }

    cout << endl << "Массив с частичными суммами" << endl;
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
    delete[] array;
}

void task_2_consistent(int N) //последовательная реализация
{
    int* array = new int[N];
    for (int i = 1; i < N; i++)
    {
        array[i] += array[i - 1];
    }
    delete[] array;
}

void task_2_OMP(int N, int num_all_threads)
{
    int* array = new int[N];
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

void task_3(int N, int n, int num_all_threads)
{
    int M;
    ifstream file("task_2_M.txt");
    file >> M;
    
    if (n >= M)
    {
        cout << "Вычисляется параллельно" << endl;
        task_1_OMP(N, num_all_threads);
        cout << "Вычисляется параллельно" << endl;
    }
    else
    {
        cout << "Вычисляется последовательно" << endl;
        task_1_consistent(N);
        cout << "Вычисляется последовательно" << endl;
    }
}

int main()
{
    setlocale(LC_CTYPE, "Russian");
    int num_all_threads = 8; //указать количество потоков

    int N = 100;
    //task_1_consistent(N);
    //task_1_OMP(N, num_all_threads);

    //task_2(num_all_threads);

    int n_task_3 = 500;
    task_3(N, n_task_3, num_all_threads);
}