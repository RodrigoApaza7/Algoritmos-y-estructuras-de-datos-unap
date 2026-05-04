#include <iostream>
#include <fstream>   // Para abrir el archivo CSV
#include <sstream>   // Para separar las palabras por comas
#include <vector>
#include <list>
#include <chrono>
#include <string>

using namespace std;

// Estructura para guardar cada fila del dataset
struct Record {
    string key;
    string value;
};

// --- FUNCIÓN DE LECTURA DE DATOS (Lo que preguntabas) ---
// Esta función abre el archivo de Kaggle y saca las claves
vector<string> leerDatosDesdeKaggle(string nombreArchivo, int limite) {
    vector<string> claves;
    ifstream archivo(nombreArchivo); 
    string linea;

    if (!archivo.is_open()) {
        cout << "Error: No se encontro el archivo " << nombreArchivo << endl;
        return claves;
    }

    getline(archivo, linea); // Saltar la primera linea de titulos

    while (getline(archivo, linea) && claves.size() < limite) {
        stringstream ss(linea);
        string columna;
        vector<string> columnas;

        // Separar la linea por cada coma
        while (getline(ss, columna, ',')) {
            columnas.push_back(columna);
        }

        // Si la linea no esta vacia, guardamos la primera columna (ej: CustomerID)
        if (!columnas.empty()) {
            claves.push_back(columnas[0]); 
        }
    }
    archivo.close();
    return claves;
}

// --- CLASE DE TABLA HASH (Ejemplo con Encadenamiento) ---
class HashTableChaining {
    int size;
    vector<list<Record>> table;
    int collisions = 0;
public:
    HashTableChaining(int s) : size(s) { table.resize(size); }
    
    int hashFunction(string key) {
        unsigned long hash = 0;
        for (char c : key) hash = hash * 31 + c; // Funcion hash polinomial
        return hash % size;
    }

    void insert(string k, string v) {
        int idx = hashFunction(k);
        if (!table[idx].empty()) collisions++;
        table[idx].push_back({k, v});
    }

    int getCollisions() { return collisions; }
};

int main() {
    // 1. Nombre del archivo que bajaste de Kaggle
    string miArchivo = "D:\\U\\IV SEMESTRE\\ALGORITMOS Y ESTRUCTURA DE DATOS\\PRACTICAS\\PROGRAMAS\\C++\\kaggle_datos.csv"; 
    int cantidadDeDatos = 10000;
    int tamanoTabla = 20011;

    // 2. Llamar a la funcion que lee el archivo
    vector<string> datosDeKaggle = leerDatosDesdeKaggle(miArchivo, cantidadDeDatos);

    if (datosDeKaggle.empty()) return 1;

    HashTableChaining hashChain(tamanoTabla);

    // 3. Medir tiempo de insercion
    auto start = chrono::high_resolution_clock::now();
    for (string k : datosDeKaggle) {
        hashChain.insert(k, k);
    }
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> tiempo = end - start;
    cout << "Datos procesados: " << datosDeKaggle.size() << endl;
    cout << "Colisiones: " << hashChain.getCollisions() << endl;
    cout << "Tiempo total: " << tiempo.count() << " segundos" << endl;

    return 0;
}