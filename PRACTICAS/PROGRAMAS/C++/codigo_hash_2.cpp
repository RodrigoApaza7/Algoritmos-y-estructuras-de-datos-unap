#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <chrono>
#include <string>
#include <iomanip>
#include <unordered_map>

using namespace std;

// Estructura de datos base
struct Record {
    string key;
    string value;
};

// --- 1. TABLA HASH: ENCADENAMIENTO SEPARADO[cite: 1, 2] ---
class HashTableChaining {
private:
    int size;
    vector<list<Record>> table;
    int collisions;
public:
    HashTableChaining(int s) : size(s), collisions(0) { table.resize(size); }
    int hashFunction(string key) {
        unsigned long hash = 0;
        for (char c : key) hash = hash * 31 + c;
        return hash % size;
    }
    void insert(string key, string value) {
        int index = hashFunction(key);
        if (!table[index].empty()) collisions++;
        table[index].push_back({key, value});
    }
    string search(string key) {
        int index = hashFunction(key);
        for (auto& r : table[index]) if (r.key == key) return r.value;
        return "";
    }
    int getCollisions() { return collisions; }
};

// --- 2. TABLA HASH: SONDEO LINEAL[cite: 1, 2] ---
class HashTableLinear {
private:
    int size;
    vector<Record> table;
    vector<bool> occupied;
    int collisions;
public:
    HashTableLinear(int s) : size(s), collisions(0) {
        table.resize(size);
        occupied.resize(size, false);
    }
    int hashFunction(string key) {
        unsigned long hash = 0;
        for (char c : key) hash = hash * 31 + c;
        return hash % size;
    }
    void insert(string key, string value) {
        int index = hashFunction(key);
        while (occupied[index]) {
            collisions++;
            index = (index + 1) % size;
        }
        table[index] = {key, value};
        occupied[index] = true;
    }
    string search(string key) {
        int index = hashFunction(key);
        int start = index;
        while (occupied[index]) {
            if (table[index].key == key) return table[index].value;
            index = (index + 1) % size;
            if (index == start) break;
        }
        return "";
    }
    int getCollisions() { return collisions; }
};

// --- 3. TABLA HASH: SONDEO CUADRÁTICO ---
class HashTableQuadratic {
private:
    int size;
    vector<Record> table;
    vector<bool> occupied;
    int collisions;
public:
    HashTableQuadratic(int s) : size(s), collisions(0) {
        table.resize(size);
        occupied.resize(size, false);
    }
    int hashFunction(string key) {
        unsigned long hash = 0;
        for (char c : key) hash = hash * 31 + c;
        return hash % size;
    }
    void insert(string key, string value) {
        int index = hashFunction(key);
        int i = 0, newIndex = index;
        while (occupied[newIndex]) {
            collisions++;
            i++;
            newIndex = (index + (i * i)) % size;
        }
        table[newIndex] = {key, value};
        occupied[newIndex] = true;
    }
    string search(string key) {
        int index = hashFunction(key);
        int i = 0, newIndex = index;
        while (occupied[newIndex]) {
            if (table[newIndex].key == key) return table[newIndex].value;
            i++;
            newIndex = (index + (i * i)) % size;
            if (i >= size) break;
        }
        return "";
    }
    int getCollisions() { return collisions; }
};

// --- LECTURA DE CSV[cite: 1, 2] ---
vector<string> readKeys(string filename, int limit) {
    vector<string> keys;
    ifstream file(filename);
    string line, col;
    if (!file.is_open()) return keys;
    getline(file, line); // Saltar encabezado
    while (getline(file, line) && (int)keys.size() < limit) {
        stringstream ss(line);
        if (getline(ss, col, ',')) if (!col.empty()) keys.push_back(col);
    }
    return keys;
}

int main() {
    string filename = "D:\\U\\IV SEMESTRE\\ALGORITMOS Y ESTRUCTURA DE DATOS\\PRACTICAS\\PROGRAMAS\\C++\\DATOS\\kaggle\\kaggle_datos.csv ";
    int limit = 10000;
    int tableSize = 20011;

    vector<string> keys = readKeys(filename, limit);
    if (keys.empty()) { cout << "Error con el archivo." << endl; return 1; }

    HashTableChaining hChain(tableSize);
    HashTableLinear hLinear(tableSize);
    HashTableQuadratic hQuad(tableSize);
    unordered_map<string, string> hNative;

    // --- MEDICIONES DE INSERCIÓN ---
    auto s = chrono::high_resolution_clock::now();
    for (auto& k : keys) hChain.insert(k, "data");
    auto e = chrono::high_resolution_clock::now();
    chrono::duration<double> tInsChain = e - s;

    s = chrono::high_resolution_clock::now();
    for (auto& k : keys) hLinear.insert(k, "data");
    e = chrono::high_resolution_clock::now();
    chrono::duration<double> tInsLinear = e - s;

    s = chrono::high_resolution_clock::now();
    for (auto& k : keys) hQuad.insert(k, "data");
    e = chrono::high_resolution_clock::now();
    chrono::duration<double> tInsQuad = e - s;

    s = chrono::high_resolution_clock::now();
    for (auto& k : keys) hNative[k] = "data";
    e = chrono::high_resolution_clock::now();
    chrono::duration<double> tInsNative = e - s;

    // --- MEDICIONES DE BÚSQUEDA (1000 intentos) ---
    s = chrono::high_resolution_clock::now();
    for(int i=0; i<1000; i++) hChain.search(keys[i % keys.size()]);
    e = chrono::high_resolution_clock::now();
    chrono::duration<double> tSearchChain = e - s;

    s = chrono::high_resolution_clock::now();
    for(int i=0; i<1000; i++) hLinear.search(keys[i % keys.size()]);
    e = chrono::high_resolution_clock::now();
    chrono::duration<double> tSearchLinear = e - s;

    s = chrono::high_resolution_clock::now();
    for(int i=0; i<1000; i++) hQuad.search(keys[i % keys.size()]);
    e = chrono::high_resolution_clock::now();
    chrono::duration<double> tSearchQuad = e - s;

    // --- SALIDA DE RESULTADOS[cite: 1, 2] ---
    cout << fixed << setprecision(6);
    cout << "\n" << left << setw(18) << "Metodo" << "| " << setw(10) << "Ins (s)" << "| " << setw(12) << "Search (s)" << "| Colisiones" << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << left << setw(18) << "Encadenamiento" << "| " << setw(10) << tInsChain.count() << "| " << setw(12) << tSearchChain.count() << "| " << hChain.getCollisions() << endl;
    cout << left << setw(18) << "Sondeo Lineal" << "| " << setw(10) << tInsLinear.count() << "| " << setw(12) << tSearchLinear.count() << "| " << hLinear.getCollisions() << endl;
    cout << left << setw(18) << "Sondeo Cuad." << "| " << setw(10) << tInsQuad.count() << "| " << setw(12) << tSearchQuad.count() << "| " << hQuad.getCollisions() << endl;
    cout << left << setw(18) << "Nativo (Map)" << "| " << setw(10) << tInsNative.count() << "| " << setw(12) << "---" << "| N/A" << endl;

    return 0;
}