#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;
using namespace chrono;

// ---------- ALGORITMOS ----------
void bubbleSort(int arr[], int n) 
{
    for(int i = 0; i < n - 1; i++) 
    {
        for(int j = 0; j < n - i - 1; j++) 
        {
            if(arr[j] > arr[j + 1]) 
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void selectionSort(int arr[], int n) 
{
    for(int i = 0; i < n - 1; i++) 
    {
        int min = i;
        for(int j = i + 1; j < n; j++) 
        {
            if(arr[j] < arr[min]) 
            {
                min = j;
            }
        }
        swap(arr[i], arr[min]);
    }
}

void insertionSort(int arr[], int n) 
{
    for(int i = 1; i < n; i++) 
    {
        int key = arr[i];
        int j = i - 1;

        while(j >= 0 && arr[j] > key) 
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void shellSort(int arr[], int n) 
{
    for(int gap = n / 2; gap > 0; gap /= 2) 
    {
        for(int i = gap; i < n; i++) 
        {
            int temp = arr[i];
            int j;

            for(j = i; j >= gap && arr[j - gap] > temp; j -= gap) 
            {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

// ---------- GENERAR ----------
void generarArreglo(int arr[], int n) 
{
    for(int i = 0; i < n; i++) 
    {
        arr[i] = rand() % 1000;
    }
}

// ---------- IMPRIMIR ----------
void imprimir(int arr[], int n) 
{
    for(int i = 0; i < n; i++) 
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// ---------- MEDIR ----------
template <class F>
double medir(int arr[], int n, F f)
{
    const int REP = 1000;

    auto t0 = high_resolution_clock::now();

    for(int k = 0; k < REP; k++)
    {
        int* copia = new int[n];
        for(int i = 0; i < n; i++) 
        {
            copia[i] = arr[i];
        }

        f(copia, n);

        delete[] copia;
    }

    auto t1 = high_resolution_clock::now();

    duration<double, milli> ms = t1 - t0;
    return ms.count() / REP; // promedio
}

// ---------- MAIN ----------
int main() 
{
    srand(time(0));

    int tamanos[] = {10, 100, 1000};

    for(int t = 0; t < 3; t++)
    {
        int n = tamanos[t];

        int* arr = new int[n];
        generarArreglo(arr, n);

        cout << "\n============================\n";
        cout << "Tamanio: " << n << endl;

        cout << "Original:\n";
        imprimir(arr, (n <= 20 ? n : 20));

        cout << "\n--- RESULTADOS ---\n";

        // 🔹 BUBBLE
        {
            int* copia = new int[n];
            for(int i = 0; i < n; i++) copia[i] = arr[i];

            double tiempo = medir(arr, n, bubbleSort);
            bubbleSort(copia, n);

            cout << "\nBubble (" << tiempo << " ms):\n";
            imprimir(copia, (n <= 20 ? n : 20));

            delete[] copia;
        }

        // 🔹 SELECTION
        {
            int* copia = new int[n];
            for(int i = 0; i < n; i++) copia[i] = arr[i];

            double tiempo = medir(arr, n, selectionSort);
            selectionSort(copia, n);

            cout << "\nSelection (" << tiempo << " ms):\n";
            imprimir(copia, (n <= 20 ? n : 20));

            delete[] copia;
        }

        // 🔹 INSERTION
        {
            int* copia = new int[n];
            for(int i = 0; i < n; i++) copia[i] = arr[i];

            double tiempo = medir(arr, n, insertionSort);
            insertionSort(copia, n);

            cout << "\nInsertion (" << tiempo << " ms):\n";
            imprimir(copia, (n <= 20 ? n : 20));

            delete[] copia;
        }

        // 🔹 SHELL
        {
            int* copia = new int[n];
            for(int i = 0; i < n; i++) copia[i] = arr[i];

            double tiempo = medir(arr, n, shellSort);
            shellSort(copia, n);

            cout << "\nShell (" << tiempo << " ms):\n";
            imprimir(copia, (n <= 20 ? n : 20));

            delete[] copia;
        }

        delete[] arr;
    }

    return 0;
}