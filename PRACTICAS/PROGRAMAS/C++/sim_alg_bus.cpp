#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>

using namespace std;

// --- Algoritmos ---
int busquedaLineal(const vector<int>& a, int x){
    for (size_t i = 0; i < a.size(); ++i)
        if (a[i] == x) return (int)i;
    return -1;
}

int busquedaBinaria(const vector<int>& a, int x){
    int l = 0, r = (int)a.size() - 1;
    while (l <= r){
        int m = l + (r - l) / 2;
        if (a[m] == x) return m;
        else if (a[m] < x) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

int busquedaExponencial(const vector<int>& a, int x){
    if (a.empty()) return -1;
    if (a[0] == x) return 0;
    int i = 1;
    while (i < (int)a.size() && a[i] < x) i <<= 1;

    int l = i / 2, r = min(i, (int)a.size() - 1);

    while (l <= r){
        int m = l + (r - l) / 2;
        if (a[m] == x) return m;
        else if (a[m] < x) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

int busquedaInterpolacion(const vector<int>& a, int x){
    int low = 0, high = (int)a.size() - 1;

    while (low <= high && x >= a[low] && x <= a[high]){
        if (a[low] == a[high])
            return (a[low] == x ? low : -1);

        long long pos = low + (long long)((double)(high - low) *
                        (x - a[low]) / (a[high] - a[low]));

        if (pos < low || pos > high) break;

        if (a[pos] == x) return (int)pos;
        if (a[pos] < x) low = (int)pos + 1;
        else high = (int)pos - 1;
    }
    return -1;
}

// --- Medición ---
template <class F>
double medir(const vector<int>& a, int x, F f){
    auto t0 = chrono::high_resolution_clock::now();
    f(a, x);
    auto t1 = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> ms = t1 - t0;
    return ms.count();
}

int main(){
    int opcionN, tipo, algoritmo;
    int n;

    // --- Selección de tamaño ---
    cout << "Selecciona tamaño:\n1) 10k\n2) 100k\n3) 500k\n> ";
    cin >> opcionN;

    if (opcionN == 1) n = 10000;
    else if (opcionN == 2) n = 100000;
    else n = 500000;

    // --- Tipo de dataset ---
    cout << "\nTipo de datos:\n1) Uniforme ordenado\n2) Sesgado ordenado\n3) Desordenado\n> ";
    cin >> tipo;

    vector<int> a(n);
    mt19937 rng(123);

    if (tipo == 1){
        iota(a.begin(), a.end(), 0);
    }
    else if (tipo == 2){
        for (int i = 0; i < n; i++){
            double u = uniform_real_distribution<double>(0,1)(rng);
            if (u < 0.8)
                a[i] = uniform_int_distribution<int>(0, (int)(0.1*n))(rng);
            else
                a[i] = uniform_int_distribution<int>((int)(0.1*n), 5*n)(rng);
        }
        sort(a.begin(), a.end());
    }
    else{
        iota(a.begin(), a.end(), 0);
        shuffle(a.begin(), a.end(), rng);
    }

    // --- Valor a buscar ---
    int x;
    cout << "\nValor a buscar: ";
    cin >> x;

    // --- Selección de algoritmo ---
    cout << "\nAlgoritmo:\n1) Lineal\n2) Binaria\n3) Exponencial\n4) Interpolacion\n> ";
    cin >> algoritmo;

    int resultado = -1;
    double tiempo = 0;

    switch(algoritmo){
        case 1:
            tiempo = medir(a, x, busquedaLineal);
            resultado = busquedaLineal(a, x);
            break;

        case 2:
            if (!is_sorted(a.begin(), a.end())){
                cout << "Array no ordenado, no se puede usar binaria\n";
                return 0;
            }
            tiempo = medir(a, x, busquedaBinaria);
            resultado = busquedaBinaria(a, x);
            break;

        case 3:
            if (!is_sorted(a.begin(), a.end())){
                cout << "Array no ordenado\n";
                return 0;
            }
            tiempo = medir(a, x, busquedaExponencial);
            resultado = busquedaExponencial(a, x);
            break;

        case 4:
            if (!is_sorted(a.begin(), a.end())){
                cout << "Array no ordenado\n";
                return 0;
            }
            tiempo = medir(a, x, busquedaInterpolacion);
            resultado = busquedaInterpolacion(a, x);
            break;
    }

    cout << "\nResultado: " << resultado << "\n";
    cout << "Tiempo: " << tiempo << " ms\n";

    return 0;
}