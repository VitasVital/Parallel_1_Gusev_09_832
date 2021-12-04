#include <iostream>
#include <omp.h>
#include <random>
#include <ctime>

using namespace std;

void task_1_consistent(int N) //последовательна€ реализаци€
{
    int* array = new int[N];
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
    delete[] array;
}

void task_1_OMP(int N)
{
    int* array = new int[N];
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
    delete[] array;
}

void task_2_consistent(int N) //последовательна€ реализаци€
{
    int* array = new int[N];
    for (int i = 1; i < N; i++)
    {
        array[i] += array[i - 1];
    }
    delete[] array;
}

void task_2_OMP(int N)
{
    int* array = new int[N];
    const int num_all_threads = 4; //нужно указать количество потоков
    bool array_summed[num_all_threads] = { false }; //завершил ли каждый поток подсчЄт
    bool array_last[num_all_threads] = { false }; //заполнил ли последний элемент поток
    
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

void task_2()
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
        task_2_OMP(M);
        end_time_OMP = clock();
        search_time_OMP = end_time_OMP - start_time_OMP;
        if (search_time_OMP < search_time_consistent)
        {
            cout << M << endl;
            break;
        }
        M += 1;
	}
}

int main()
{
    setlocale(LC_CTYPE, "Russian");
    
    //task_1_consistent(100);
    //task_1_OMP(100);
    task_2();
}