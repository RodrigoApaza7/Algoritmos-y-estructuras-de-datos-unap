#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <queue>
#include <iomanip>
#include <string>
#include <utility>

using namespace std;

// Estructura corregida: Todas las dimensiones encapsuladas dentro del tipo de dato
struct Estudiante {
    string codigo;
    string nombre;
    float nota1;
    float nota2;
    float nota3;
    float promedio;
};

// Estructuras de almacenamiento global
vector<Estudiante> hoja;
unordered_map<string, int> indiceHash;
stack<vector<Estudiante>> historial;
queue<string> colaAtencion;

// Función en línea para cálculo aritmético eficiente
inline float calcularPromedio(float n1, float n2, float n3) {
    return (n1 + n2 + n3) / 3.0f;
}

// Reconstrucción del índice asociativo O(n)
void actualizarHash() {
    indiceHash.clear();
    for (size_t i = 0; i < hoja.size(); ++i) {
        indiceHash[hoja[i].codigo] = static_cast<int>(i);
    }
}

// Resguardo del estado actual previo a mutaciones
void guardarHistorial() {
    historial.push(hoja);
}

// 1. REGISTRO DE ESTUDIANTES (Con validación estricta de rango 0-20 y Hash O(1))
void registrarEstudiante() {
    Estudiante e;
    cout << "\n=== REGISTRAR NUEVO ESTUDIANTE ===\n";
    cout << "Codigo unico: ";
    cin >> e.codigo;

    // Validación de clave primaria duplicada en tiempo O(1)
    if (indiceHash.find(e.codigo) != indiceHash.end()) {
        cout << "[ERROR] El codigo ingresado ya se encuentra registrado.\n";
        return;
    }

    cin.ignore(); // Limpiar el buffer de entrada para el getline
    cout << "Apellidos y Nombres: ";
    getline(cin, e.nombre);

    // Validación estricta de rangos para cumplir el esquema educativo (0-20)
    do {
        cout << "Nota 1 (0-20): "; cin >> e.nota1;
        if (e.nota1 < 0 || e.nota1 > 20) cout << "[ALERTA] Nota fuera de rango. Reintente.\n";
    } while (e.nota1 < 0 || e.nota1 > 20);

    do {
        cout << "Nota 2 (0-20): "; cin >> e.nota2;
        if (e.nota2 < 0 || e.nota2 > 20) cout << "[ALERTA] Nota fuera de rango. Reintente.\n";
    } while (e.nota2 < 0 || e.nota2 > 20);

    do {
        cout << "Nota 3 (0-20): "; cin >> e.nota3;
        if (e.nota3 < 0 || e.nota3 > 20) cout << "[ALERTA] Nota fuera de rango. Reintente.\n";
    } while (e.nota3 < 0 || e.nota3 > 20);

    // Cálculo e indexación inmediata
    e.promedio = calcularPromedio(e.nota1, e.nota2, e.nota3);

    guardarHistorial();
    hoja.push_back(e);
    actualizarHash();
    cout << "[EXITO] Registro insertado e indexado correctamente.\n";
}

// 2. MOSTRAR HOJA COMPLETA
void mostrarHoja() {
    if (hoja.empty()) {
        cout << "\n[AVISO] La hoja de calculo no contiene registros.\n";
        return;
    }

    cout << "\n========================================================================\n";
    cout << "                      MINI HOJA DE CALCULO ACADEMICA                    \n";
    cout << "========================================================================\n";
    cout << left << setw(12) << "Codigo"
         << setw(25) << "Nombre Completo"
         << setw(8) << "N1"
         << setw(8) << "N2"
         << setw(8) << "N3"
         << setw(10) << "Promedio" << endl;
    cout << "------------------------------------------------------------------------\n";

    for (const auto &e : hoja) {
        cout << left << setw(12) << e.codigo
             << setw(25) << e.nombre
             << setw(8) << setprecision(1) << fixed << e.nota1
             << setw(8) << e.nota2
             << setw(8) << e.nota3
             << setw(10) << setprecision(2) << e.promedio << endl;
    }
    cout << "========================================================================\n";
}

// 3. BÚSQUEDA INDEXADA O(1)
void buscarEstudiante() {
    string codigo;
    cout << "\nIngrese el codigo a buscar en Tabla Hash: ";
    cin >> codigo;

    auto it = indiceHash.find(codigo);
    if (it != indiceHash.end()) {
        int pos = it->second;
        const auto &e = hoja[pos];
        cout << "\n[REGISTRO ENCONTRADO - BUSQUEDA O(1)]\n";
        cout << "Estudiante: " << e.nombre << " | Promedio Actual: " << fixed << setprecision(2) << e.promedio << endl;
    } else {
        cout << "[AVISO] Codigo no registrado en la base indexada.\n";
    }
}

// ============================================================================
// ALGORITMOS DE ORDENAMIENTO (Optimizados por paso de referencia modular)
// ============================================================================

// ALGORITMO 1: Selection Sort - Descendente por Promedio
void selectionSort(vector<Estudiante> &arr) {
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {
        size_t indice_max = i;
        for (size_t j = i + 1; j < n; ++j) {
            if (arr[j].promedio > arr[indice_max].promedio) {
                indice_max = j;
            }
        }
        if (indice_max != i) {
            swap(arr[i], arr[indice_max]);
        }
    }
}

void ordenarSelectionSort() {
    if (hoja.empty()) return;
    guardarHistorial();
    selectionSort(hoja);
    actualizarHash();
    cout << "[OK] Celdas ordenadas por Promedio (Descendente) via Selection Sort.\n";
}

// ALGORITMO 2: Insertion Sort - Ascendente por Código
void insertionSort(vector<Estudiante> &arr) {
    size_t n = arr.size();
    for (size_t i = 1; i < n; ++i) {
        Estudiante clave = arr[i];
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && arr[j].codigo > clave.codigo) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = clave;
    }
}

void ordenarInsertionSort() {
    if (hoja.empty()) return;
    guardarHistorial();
    insertionSort(hoja);
    actualizarHash();
    cout << "[OK] Celdas ordenadas por Codigo (Ascendente) via Insertion Sort.\n";
}

// ALGORITMO 3: QuickSort - Descendente por Promedio
int particion(vector<Estudiante> &arr, int bajo, int alto) {
    float pivote = arr[alto].promedio;
    int i = bajo - 1;
    for (int j = bajo; j < alto; j++) {
        if (arr[j].promedio >= pivote) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[alto]);
    return i + 1;
}

void quickSort(vector<Estudiante> &arr, int bajo, int alto) {
    if (bajo < alto) {
        int pi = particion(arr, bajo, alto);
        quickSort(arr, bajo, pi - 1);
        quickSort(arr, pi + 1, alto);
    }
}

void ordenarPorPromedioQuick() {
    if (hoja.empty()) return;
    guardarHistorial();
    quickSort(hoja, 0, static_cast<int>(hoja.size() - 1));
    actualizarHash();
    cout << "[OK] Filas ordenadas por Promedio (Descendente) via QuickSort.\n";
}

// ALGORITMO 4: MergeSort - Ascendente por Código
void merge(vector<Estudiante> &arr, int izq, int medio, int der) {
    int n1 = medio - izq + 1;
    int n2 = der - medio;

    vector<Estudiante> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[izq + i];
    for (int j = 0; j < n2; j++) R[j] = arr[medio + 1 + j];

    int i = 0, j = 0, k = izq;
    while (i < n1 && j < n2) {
        if (L[i].codigo <= R[j].codigo) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }
}

void mergeSort(vector<Estudiante> &arr, int izq, int der) {
    if (izq < der) {
        int medio = izq + (der - izq) / 2;
        mergeSort(arr, izq, medio);
        mergeSort(arr, medio + 1, der);
        merge(arr, izq, medio, der);
    }
}

void ordenarPorCodigoMerge() {
    if (hoja.empty()) return;
    guardarHistorial();
    mergeSort(hoja, 0, static_cast<int>(hoja.size() - 1));
    actualizarHash();
    cout << "[OK] Filas ordenadas por Codigo (Ascendente) via MergeSort.\n";
}

// ============================================================================

// 6. SUBSISTEMA DESHACER (Stack LIFO)
void deshacer() {
    if (!historial.empty()) {
        hoja = historial.top();
        historial.pop();
        actualizarHash();
        cout << "[UNDO] Estado de celdas restaurado al punto de control anterior.\n";
    } else {
        cout << "[INFO] No existen acciones registradas para deshacer.\n";
    }
}

// 7. INSERTAR A COLA DE ATENCIÓN (FIFO)
void agregarColaAtencion() {
    string codigo;
    cout << "\nIngrese el codigo del estudiante que entra a la cola: ";
    cin >> codigo;

    if (indiceHash.find(codigo) != indiceHash.end()) {
        colaAtencion.push(codigo);
        cout << "[COLA] Estudiante ingresado en la cola FIFO de secretaria.\n";
    } else {
        cout << "[ERROR] El codigo no figura registrado en la hoja actual.\n";
    }
}

// 8. ATENDER SIGUIENTE ESTUDIANTE (FIFO)
void atenderEstudiante() {
    if (!colaAtencion.empty()) {
        string codigo = colaAtencion.front();
        colaAtencion.pop();
        int pos = indiceHash[codigo];
        cout << "\n[ATENCION] Despachando requerimiento de: " << hoja[pos].nombre << endl;
    } else {
        cout << "[INFO] Canal de atencion despejado. Cola vacia.\n";
    }
}

// 9. ESTADÍSTICAS AVANZADAS (Función Recursiva Pura O(n))
float sumaPromediosRecursiva(size_t i) {
    if (i == hoja.size()) return 0.0f;
    return hoja[i].promedio + sumaPromediosRecursiva(i + 1);
}

void estadisticas() {
    if (hoja.empty()) {
        cout << "\n[AVISO] Hoja sin datos para el calculo estadistico.\n";
        return;
    }
    float suma = sumaPromediosRecursiva(0);
    float promedioGeneral = suma / static_cast<float>(hoja.size());

    cout << "\n==== METRICAS ANALITICAS GENERALES ====\n";
    cout << " Estudiantes Procesados  : " << hoja.size() << "\n";
    cout << " Promedio General Cohorte: " << fixed << setprecision(2) << promedioGeneral << "\n";
    cout << "=======================================\n";
}

// INTERFAZ DE USUARIO CON LIMPIEZA DE FLUJO SEGURA
void menu() {
    int opcion;
    do {
        cout << "\n      MENU PRINCIPAL SPREADSHEET\n";
        cout << "=========================================\n";
        cout << "1. Registrar Estudiante (Fila)\n";
        cout << "2. Mostrar Hoja de Calculo\n";
        cout << "3. Buscar Estudiante por Codigo (Hash)\n";
        cout << "4. Ordenar por Promedio [Selection Sort - Desc]\n";
        cout << "5. Ordenar por Codigo [Insertion Sort - Asc]\n";
        cout << "6. Ordenar por Promedio [QuickSort - Desc]\n";
        cout << "7. Ordenar por Codigo [MergeSort - Asc]\n";
        cout << "8. Deshacer Ultima Modificacion [Stack]\n";
        cout << "9. Insertar Estudiante a Cola de Atencion\n";
        cout << "10. Atender Siguiente Estudiante [FIFO]\n";
        cout << "11. Mostrar Estadisticas Consolidadas\n";
        cout << "12. Salir de la Aplicacion\n";
        cout << "=========================================\n";
        cout << "Seleccione una opcion (1-12): ";
        
        if (!(cin >> opcion)) {
            cout << "[ALERTA] Entrada invalida. Digite un numero entero.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = 0; // Resetear para evitar basura en el switch
            continue;
        }

        switch (opcion) {
            case 1: registrarEstudiante(); break;
            case 2: mostrarHoja(); break;
            case 3: buscarEstudiante(); break;
            case 4: ordenarSelectionSort(); break;
            case 5: ordenarInsertionSort(); break;
            case 6: ordenarPorPromedioQuick(); break;
            case 7: ordenarPorCodigoMerge(); break;
            case 8: deshacer(); break;
            case 9: agregarColaAtencion(); break;
            case 10: atenderEstudiante(); break;
            case 11: estadisticas(); break;
            case 12: cout << "Liberando memoria interna... Programa cerrado.\n"; break;
            default: if (opcion != 0) cout << "[ERROR] Opcion fuera de rango.\n";
        }
    } while (opcion != 12);
}

int main() {
    // Sincronización rápida de flujos de E/S
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    menu();
    return 0;
}