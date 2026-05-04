#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

// 1. COUNTING SORT
void countingSort(vector<int>& arr) 
{
    if (arr.empty()) return;
    int maxVal = *max_element(arr.begin(), arr.end());
    vector<int> count(maxVal + 1, 0);
    vector<int> output(arr.size());

    for (int num : arr) count[num]++;
    for (int i = 1; i <= maxVal; i++) count[i] += count[i - 1];
    for (int i = arr.size() - 1; i >= 0; i--) 
    {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }
    arr = output;
}

// 2. RADIX SORT (Subrutina para dígitos)
void countingSortForRadix(vector<int>& arr, int exp) 
{
    int n = arr.size();
    vector<int> output(n);
    int count[10] = {0};

    for (int i = 0; i < n; i++) count[(arr[i] / exp) % 10]++;
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];
    for (int i = n - 1; i >= 0; i--) 
    {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }
    for (int i = 0; i < n; i++) arr[i] = output[i];
}

void radixSort(vector<int>& arr) 
{
    int m = *max_element(arr.begin(), arr.end());
    for (int exp = 1; m / exp > 0; exp *= 10)
        countingSortForRadix(arr, exp);
}

// 3. BUCKET SORT
void bucketSort(vector<int>& arr) 
{
    int n = arr.size();
    if (n <= 0) return;

    int maxVal = *max_element(arr.begin(), arr.end());

    int numBuckets = sqrt(n);  // 👈 clave
    vector<vector<int>> buckets(numBuckets);

    // Distribuir elementos
    for (int i = 0; i < n; i++) 
    {
        int bi = (long long)arr[i] * numBuckets / (maxVal + 1);
        buckets[bi].push_back(arr[i]);
    }

    // Ordenar cada bucket
    for (int i = 0; i < numBuckets; i++)
        sort(buckets[i].begin(), buckets[i].end());

    // Concatenar
    int index = 0;
    for (int i = 0; i < numBuckets; i++)
        for (int num : buckets[i])
            arr[index++] = num;
}

// 4. TIMSORT (Implementación simplificada)
const int RUN = 32;
void insertionSort(vector<int>& arr, int left, int right) 
{
    for (int i = left + 1; i <= right; i++) 
    {
        int temp = arr[i], j = i - 1;
        while (j >= left && arr[j] > temp) 
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

void merge(vector<int>& arr, int l, int m, int r) 
{
    int len1 = m - l + 1, len2 = r - m;
    vector<int> left(len1), right(len2);
    for (int i = 0; i < len1; i++) left[i] = arr[l + i];
    for (int i = 0; i < len2; i++) right[i] = arr[m + 1 + i];
    int i = 0, j = 0, k = l;
    while (i < len1 && j < len2) arr[k++] = (left[i] <= right[j]) ? left[i++] : right[j++];
    while (i < len1) arr[k++] = left[i++];
    while (j < len2) arr[k++] = right[j++];
}

void timSort(vector<int>& arr) 
{
    int n = arr.size();
    for (int i = 0; i < n; i += RUN)
        insertionSort(arr, i, min((i + RUN - 1), (n - 1)));
    for (int size = RUN; size < n; size = 2 * size) 
    {
        for (int left = 0; left < n; left += 2 * size) 
        {
            int mid = left + size - 1;
            int right = min((left + 2 * size - 1), (n - 1));
            if (mid < right) merge(arr, left, mid, right);
        }
    }
}

// Función para medir tiempo
void measureTime(string name, vector<int> arr, void (*sortFunc)(vector<int>&)) 
{
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << name << ": " << duration.count() << " ms" << endl;
}

int main() 
{
    const int N = 100000; // 10^5 datos
    vector<int> data(N);
    mt19937 rng(time(0));
    uniform_int_distribution<int> dist(0, 10000); // Rango limitado para Counting/Radix

    for (int i = 0; i < N; i++) data[i] = dist(rng);

    cout << "--- Comparacion de Algoritmos (N = " << N << ") ---" << endl;
    measureTime("Counting Sort", data, countingSort);
    measureTime("Radix Sort   ", data, radixSort);
    measureTime("Bucket Sort  ", data, bucketSort);
    measureTime("Timsort      ", data, timSort);

    return 0;
}