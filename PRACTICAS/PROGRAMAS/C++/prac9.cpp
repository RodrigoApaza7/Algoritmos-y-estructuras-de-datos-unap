#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <queue>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <chrono>
#include <random>

// =====================================================================
// ACTIVIDAD 8: Estructuras de Datos Base en C++17
// =====================================================================

enum class EstadoAcademico {
    REGULAR,
    OBSERVADO,
    RESERVADO,
    EGRESADO
};

// Función auxiliar para convertir el Enum en texto legible
std::string estadoToString(EstadoAcademico estado) {
    switch (estado) {
        case EstadoAcademico::REGULAR:   return "Regular";
        case EstadoAcademico::OBSERVADO: return "Observado";
        case EstadoAcademico::RESERVADO: return "Reservado";
        case EstadoAcademico::EGRESADO:  return "Egresado";
        default:                         return "Desconocido";
    }
}

struct Estudiante {
    int codigo;         // Código de 8 dígitos de la UNAP
    std::string nombre;
    std::string escuela;
    double ppa;         // Promedio Ponderado Acumulado (0.0 - 20.0)
    EstadoAcademico estado;

    // Constructor con las validaciones de robustez exigidas (Actividad 13)
    Estudiante(int cod, std::string nom, std::string esc, double p, EstadoAcademico est) 
        : codigo(cod), nombre(nom), escuela(esc), ppa(p), estado(est) {
        
        if (codigo < 10000000 || codigo > 99999999) {
            throw std::invalid_argument("El codigo UNAP debe tener exactamente 8 digitos.");
        }
        if (ppa < 0.0 || ppa > 20.0) {
            throw std::invalid_argument("El PPA debe estar en el rango de 0.0 a 20.0.");
        }
    }
};

struct NodoBST {
    Estudiante estudiante;
    std::unique_ptr<NodoBST> izquierdo; // RAII - Gestión inteligente de memoria
    std::unique_ptr<NodoBST> derecho;

    NodoBST(Estudiante est) : estudiante(est), izquierdo(nullptr), derecho(nullptr) {}
};

// =====================================================================
// ACTIVIDADES 9, 10, 11 y 13: Clase ArbolAcademico Expandida
// =====================================================================

class ArbolAcademico {
private:
    std::unique_ptr<NodoBST> raiz;

    // -----------------------------------------------------------------
    // Métodos Privados Auxiliares (Recursivos)
    // -----------------------------------------------------------------

    // Actividad 9: Inserción recursiva usando referencias a unique_ptr
    void _insertar(std::unique_ptr<NodoBST>& nodo_actual, const Estudiante& estudiante) {
        if (nodo_actual == nullptr) {
            nodo_actual = std::make_unique<NodoBST>(estudiante);
            return;
        }

        if (estudiante.codigo < nodo_actual->estudiante.codigo) {
            _insertar(nodo_actual->izquierdo, estudiante);
        } else if (estudiante.codigo > nodo_actual->estudiante.codigo) {
            _insertar(nodo_actual->derecho, estudiante);
        } else {
            // Actividad 13: Excepción por llave duplicada
            throw std::runtime_error("El codigo " + std::to_string(estudiante.codigo) + " ya esta registrado.");
        }
    }

    // Actividad 9: Recorrido In-Order
    void _in_order(const std::unique_ptr<NodoBST>& nodo, std::vector<Estudiante>& resultados) const {
        if (nodo != nullptr) {
            _in_order(nodo->izquierdo, resultados);
            resultados.push_back(nodo->estudiante);
            _in_order(nodo->derecho, resultados);
        }
    }

    // Actividad 10: Búsqueda recursiva
    NodoBST* _buscar(NodoBST* nodo_actual, int codigo) const {
        if (nodo_actual == nullptr || nodo_actual->estudiante.codigo == codigo) {
            return nodo_actual;
        }
        if (codigo < nodo_actual->estudiante.codigo) {
            return _buscar(nodo_actual->izquierdo.get(), codigo);
        }
        return _buscar(nodo_actual->derecho.get(), codigo);
    }

    // Actividad 10: Encontrar el nodo mínimo (Sucesor In-Order)
    NodoBST* _minimo(NodoBST* nodo) const {
        NodoBST* actual = nodo;
        while (actual && actual->izquierdo != nullptr) {
            actual = actual->izquierdo.get();
        }
        return actual;
    }

    // Actividad 10: Eliminación recursiva por casos
    std::unique_ptr<NodoBST> _eliminar(std::unique_ptr<NodoBST> nodo_actual, int codigo) {
        if (nodo_actual == nullptr) return nullptr;

        if (codigo < nodo_actual->estudiante.codigo) {
            nodo_actual->izquierdo = _eliminar(std::move(nodo_actual->izquierdo), codigo);
        } else if (codigo > nodo_actual->estudiante.codigo) {
            nodo_actual->derecho = _eliminar(std::move(nodo_actual->derecho), codigo);
        } else {
            // ¡Nodo encontrado!

            // Caso 1 y 2: Cero o un hijo
            if (nodo_actual->izquierdo == nullptr) {
                return std::move(nodo_actual->derecho);
            } else if (nodo_actual->derecho == nullptr) {
                return std::move(nodo_actual->izquierdo);
            }

            // Caso 3: Dos hijos
            NodoBST* sucesor = _minimo(nodo_actual->derecho.get());
            
            // Reemplazamos los datos (C++ sobreescribe la estructura)
            nodo_actual->estudiante = sucesor->estudiante;
            
            // Eliminamos el sucesor en el subárbol derecho
            nodo_actual->derecho = _eliminar(std::move(nodo_actual->derecho), sucesor->estudiante.codigo);
        }
        return nodo_actual;
    }

    // Actividad 11: Visualización ASCII recursiva
    void _imprimir_arbol(const std::unique_ptr<NodoBST>& nodo, int espacio) const {
        const int DISTANCIA = 8;
        if (nodo == nullptr) return;

        espacio += DISTANCIA;
        _imprimir_arbol(nodo->derecho, espacio);

        std::cout << std::endl;
        for (int i = DISTANCIA; i < espacio; ++i) {
            std::cout << " ";
        }
        std::cout << "[" << nodo->estudiante.codigo << "]" << std::endl;

        _imprimir_arbol(nodo->izquierdo, espacio);
    }

    // Método privado recursivo para calcular la altura del BST
    int _calcular_altura(const std::unique_ptr<NodoBST>& nodo) const {
        if (nodo == nullptr) return 0;
        return 1 + std::max(_calcular_altura(nodo->izquierdo), _calcular_altura(nodo->derecho));
    }

public:
    ArbolAcademico() : raiz(nullptr) {}

    // -----------------------------------------------------------------
    // Interfaz Pública de la Clase
    // -----------------------------------------------------------------

    void insertar(const Estudiante& estudiante) {
        _insertar(raiz, estudiante);
    }

    std::vector<Estudiante> in_order() const {
        std::vector<Estudiante> resultados;
        _in_order(raiz, resultados);
        return resultados;
    }

    // Actividad 9: Recorrido por niveles (BFS) usando std::queue
    std::vector<Estudiante> bfs() const {
        std::vector<Estudiante> resultados;
        if (raiz == nullptr) return resultados;

        std::queue<NodoBST*> cola;
        cola.push(raiz.get());

        while (!cola.empty()) {
            NodoBST* actual = cola.front();
            cola.pop();

            resultados.push_back(actual->estudiante);

            if (actual->izquierdo != nullptr) cola.push(actual->izquierdo.get());
            if (actual->derecho != nullptr) cola.push(actual->derecho.get());
        }
        return resultados;
    }

    bool buscar(int codigo, Estudiante& estudiante_encontrado) const {
        NodoBST* resultado = _buscar(raiz.get(), codigo);
        if (resultado != nullptr) {
            estudiante_encontrado = resultado->estudiante;
            return true;
        }
        return false;
    }

    void eliminar(int codigo) {
        Estudiante aux(10000000, "", "", 0.0, EstadoAcademico::REGULAR); // Auxiliar para buscar
        if (!_buscar(raiz.get(), codigo)) {
            throw std::runtime_error("El estudiante con codigo " + std::to_string(codigo) + " no existe.");
        }
        raiz = _eliminar(std::move(raiz), codigo);
    }

    // Actividad 11: Consultas y Filtros
    std::vector<Estudiante> por_rango_ppa(double min_ppa, double max_ppa) const {
        std::vector<Estudiante> todos = in_order();
        std::vector<Estudiante> filtrados;
        for (const auto& e : todos) {
            if (e.ppa >= min_ppa && e.ppa <= max_ppa) filtrados.push_back(e);
        }
        return filtrados;
    }

    std::vector<Estudiante> por_estado(EstadoAcademico estado) const {
        std::vector<Estudiante> todos = in_order();
        std::vector<Estudiante> filtrados;
        for (const auto& e : todos) {
            if (e.estado == estado) filtrados.push_back(e);
        }
        return filtrados;
    }

    // Actividad 11: Estadísticas descriptivas del PPA
    void calcular_estadisticas(int& total, double& media, double& max_val, double& min_val, double& stdev) const {
        std::vector<Estudiante> todos = in_order();
        total = todos.size();
        if (total == 0) {
            media = max_val = min_val = stdev = 0.0;
            return;
        }

        double suma = 0.0;
        max_val = todos[0].ppa;
        min_val = todos[0].ppa;

        for (const auto& e : todos) {
            suma += e.ppa;
            if (e.ppa > max_val) max_val = e.ppa;
            if (e.ppa < min_val) min_val = e.ppa;
        }
        media = suma / total;

        if (total > 1) {
            double varianza_suma = 0.0;
            for (const auto& e : todos) {
                varianza_suma += std::pow(e.ppa - media, 2);
            }
            stdev = std::sqrt(varianza_suma / (total - 1)); // Desviación estándar muestral
        } else {
            stdev = 0.0;
        }
    }

    void imprimir_arbol() const {
        if (raiz == nullptr) {
            std::cout << "\n[!] El arbol esta vacio actualmente.\n";
            return;
        }
        std::cout << "\n--- ESTRUCTURA VISUAL DEL ARBOL BINARIO (Rotado 90 CCW) ---";
        _imprimir_arbol(raiz, 0);
        std::cout << "----------------------------------------------------------\n";
    }

    // Actividad 12: Benchmark de rendimiento para inserción y búsqueda

    void ejecutar_benchmark_cpp() {
        std::vector<int> tamanos = {100, 1000, 10000, 100000};
        
        // Configuración de generadores aleatorios modernos de C++
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(10000000, 99999999); // Códigos UNAP válidos

        std::cout << "\n==========================================================\n";
        std::cout << "      INICIANDO BENCHMARK EXPERIMENTAL (ACTIVIDAD 12)     \n";
        std::cout << "==========================================================\n";
        
        for (int n : tamanos) {
            ArbolAcademico arbol_prueba;
            std::vector<int> codigos_insertados;
            codigos_insertados.reserve(n);
            
            // --- 1. MEDICIÓN DE TIEMPO DE INSERCIÓN ---
            auto start_ins = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < n; ++i) {
                int cod = distr(gen);
                try {
                    arbol_prueba.insertar(Estudiante(cod, "Est", "Sistemas", 14.0, EstadoAcademico::REGULAR));
                    codigos_insertados.push_back(cod);
                } catch(...) {
                    // Si por casualidad sale un código duplicado, reintentamos la iteración
                    i--; 
                }
            }
            auto end_ins = std::chrono::high_resolution_clock::now();
            double t_ins = std::chrono::duration<double, std::milli>(end_ins - start_ins).count();

            // --- 2. MEDICIÓN DE TIEMPO DE BÚSQUEDA ---
            // Buscamos el último código insertado (garantiza evaluar la profundidad del árbol)
            int cod_buscar = codigos_insertados.back();
            Estudiante aux(10000000, "", "", 0.0, EstadoAcademico::REGULAR);
            
            auto start_busq = std::chrono::high_resolution_clock::now();
            arbol_prueba.buscar(cod_buscar, aux);
            auto end_busq = std::chrono::high_resolution_clock::now();
            double t_busq = std::chrono::duration<double, std::milli>(end_busq - start_busq).count();

            // --- 3. CÁLCULO DE LA ALTURA ---
            int altura = arbol_prueba._calcular_altura(arbol_prueba.raiz);

            // Imprimir resultados con formato tabular ordenado
            std::cout << std::fixed << std::setprecision(4);
            std::cout << "N = " << std::setw(6) << n 
                    << " | BST ins: " << std::setw(8) << t_ins << " ms"
                    << " | BST busq: " << std::setw(8) << t_busq << " ms"
                    << " | Altura: " << altura << "\n";
        }
        std::cout << "==========================================================\n";
    }
};

// =====================================================================
// INTERFAZ DE CONSOLA INTERACTIVA (Menú Principal)
// =====================================================================
int main() {
    ArbolAcademico arbol;

    // Precarga automática del dataset del laboratorio (7 estudiantes obligatorios)
    try {
        arbol.insertar(Estudiante(20210300, "Apaza, Carlos", "Ingenieria de Sistemas", 14.5, EstadoAcademico::REGULAR));
        arbol.insertar(Estudiante(20200150, "Blanco, Ana", "Ingenieria de Sistemas", 11.2, EstadoAcademico::EGRESADO));
        arbol.insertar(Estudiante(20220450, "Condori, Luis", "Ingenieria de Sistemas", 16.8, EstadoAcademico::REGULAR));
        arbol.insertar(Estudiante(20190050, "Mamani, Eva", "Ingenieria de Sistemas", 13.0, EstadoAcademico::RESERVADO));
        arbol.insertar(Estudiante(20210200, "Flores, Rene", "Ingenieria de Sistemas", 15.2, EstadoAcademico::REGULAR));
        arbol.insertar(Estudiante(20210400, "Quispe, Diana", "Ingenieria de Sistemas", 17.5, EstadoAcademico::REGULAR));
        arbol.insertar(Estudiante(20230600, "Zela, Hector", "Ingenieria de Sistemas", 12.1, EstadoAcademico::REGULAR));
    } catch (const std::exception& e) {
        std::cerr << "Error cargando datos iniciales: " << e.what() << std::endl;
    }

    int opcion = 0;
    while (true) {
        std::cout << "\n=======================================================\n";
        std::cout << "    SISTEMA DE GESTION DE EXPEDIENTES ACADEMICOS UNAP  \n";
        std::cout << "=======================================================\n";
        std::cout << " 1. Insertar nuevo estudiante (Actividad 9)\n";
        std::cout << " 2. Buscar estudiante por codigo (Actividad 10)\n";
        std::cout << " 3. Eliminar estudiante por codigo (Actividad 10)\n";
        std::cout << " 4. Ver consultas y filtros academicos (Actividad 11)\n";
        std::cout << " 5. Ver reporte estadistico global (Actividad 11)\n";
        std::cout << " 6. Mostrar estructura visual del BST (Actividad 11)\n";
        std::cout << " 7. Listar todos los estudiantes (In-Order / BFS)\n";
        std::cout << " 8. Salir de la aplicacion\n";
        std::cout << " 9. Ejecutar prueba de estres masiva (Actividad 12)\n"; // Nueva opción
        std::cout << "=======================================================\n";
        std::cout << "Seleccione una opcion (1-9): ";
        
        if (!(std::cin >> opcion)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[!] Entrada invalida. Intente de nuevo.\n";
            continue;
        }

        if (opcion == 8) {
            std::cout << "\nCerrando el sistema academico en C++. ¡Hasta luego!\n";
            break;
        }

        try {
            if (opcion == 1) {
                int codigo;
                std::string nombre, escuela;
                double ppa;
                int est_opc;

                std::cout << "\n--- INSERTAR ESTUDIANTE ---\n";
                std::cout << "Codigo UNAP (8 digitos): "; std::cin >> codigo;
                std::cin.ignore(); // Limpiar salto de línea
                std::cout << "Nombre y Apellidos: "; std::getline(std::cin, nombre);
                std::cout << "Escuela Profesional: "; std::getline(std::cin, escuela);
                std::cout << "Promedio Ponderado Acumulado (0.0 - 20.0): "; std::cin >> ppa;
                std::cout << "Estados: 1.REGULAR, 2.OBSERVADO, 3.RESERVADO, 4.EGRESADO\nSeleccione numero: "; 
                std::cin >> est_opc;

                EstadoAcademico estado = EstadoAcademico::REGULAR;
                if (est_opc == 2) estado = EstadoAcademico::OBSERVADO;
                else if (est_opc == 3) estado = EstadoAcademico::RESERVADO;
                else if (est_opc == 4) estado = EstadoAcademico::EGRESADO;

                arbol.insertar(Estudiante(codigo, nombre, escuela, ppa, estado));
                std::cout << "\x1B[32m" << "-> Estudiante registrado exitosamente en C++." << "\x1B[0m" << std::endl;

            } else if (opcion == 2) {
                int codigo;
                std::cout << "\n--- BUSCAR ESTUDIANTE ---\n";
                std::cout << "Ingrese codigo a buscar: "; std::cin >> codigo;
                
                Estudiante encontrado(10000000, "", "", 0.0, EstadoAcademico::REGULAR);
                if (arbol.buscar(codigo, encontrado)) {
                    std::cout << "\n[Registro Encontrado]\n";
                    std::cout << " Nombre   : " << encontrado.nombre << "\n";
                    std::cout << " Escuela  : " << encontrado.escuela << "\n";
                    std::cout << " PPA      : " << encontrado.ppa << "\n";
                    std::cout << " Estado   : " << estadoToString(encontrado.estado) << "\n";
                } else {
                    std::cout << "-> No se encontro ningun estudiante con el codigo: " << codigo << std::endl;
                }

            } else if (opcion == 3) {
                int codigo;
                std::cout << "\n--- ELIMINAR ESTUDIANTE ---\n";
                std::cout << "Ingrese codigo a eliminar: "; std::cin >> codigo;
                arbol.eliminar(codigo);
                std::cout << "\x1B[32m" << "-> Registro eliminado correctamente del BST." << "\x1B[0m" << std::endl;

            } else if (opcion == 4) {
                int sub_opc;
                std::cout << "\n--- CONSULTAS Y FILTROS ---\n";
                std::cout << "1. Filtrar por rango de PPA\n2. Filtrar por Estado Academico\nSeleccione filtro: ";
                std::cin >> sub_opc;

                if (sub_opc == 1) {
                    double min_p, max_p;
                    std::cout << "PPA Minimo: "; std::cin >> min_p;
                    std::cout << "PPA Maximo: "; std::cin >> max_p;
                    auto res = arbol.por_rango_ppa(min_p, max_p);
                    std::cout << "\nResultados encontrados (" << res.size() << "):\n";
                    for (const auto& e : res) std::cout << "  - " << e.nombre << " | PPA: " << e.ppa << "\n";
                } else if (sub_opc == 2) {
                    std::cout << "1.REGULAR, 2.OBSERVADO, 3.RESERVADO, 4.EGRESADO\nSeleccione numero: ";
                    int est_opc; std::cin >> est_opc;
                    EstadoAcademico target = EstadoAcademico::REGULAR;
                    if (est_opc == 2) target = EstadoAcademico::OBSERVADO;
                    else if (est_opc == 3) target = EstadoAcademico::RESERVADO;
                    else if (est_opc == 4) target = EstadoAcademico::EGRESADO;

                    auto res = arbol.por_estado(target);
                    std::cout << "\nResultados encontrados (" << res.size() << "):\n";
                    for (const auto& e : res) std::cout << "  - " << e.nombre << " | Codigo: " << e.codigo << "\n";
                }

            } else if (opcion == 5) {
                int total;
                double media, max_val, min_val, stdev;
                arbol.calcular_estadisticas(total, media, max_val, min_val, stdev);

                std::cout << "\n--- REPORTE ESTADISTICO GLOBAL (C++) ---\n";
                std::cout << " Total de estudiantes matriculados : " << total << "\n";
                std::cout << std::fixed << std::setprecision(2);
                std::cout << " Media (Promedio general del PPA)  : " << media << "\n";
                std::cout << " PPA Maximo registrado             : " << max_val << "\n";
                std::cout << " PPA Minimo registrado             : " << min_val << "\n";
                std::cout << " Desviacion Estandar de la muestra : " << stdev << "\n";

            } else if (opcion == 6) {
                arbol.imprimir_arbol();

            } else if (opcion == 7) {
                int sub_opc;
                std::cout << "\n--- LISTADO DE EXPEDIENTES ---\n";
                std::cout << "1. Ordenado ascendentemente (In-Order)\n2. Por niveles jerarquicos (BFS)\nSeleccione: ";
                std::cin >> sub_opc;

                if (sub_opc == 1) {
                    std::cout << "\n[Listado In-Order C++]\n";
                    std::cout << std::left << std::setw(10) << "Codigo" << " | " << std::setw(15) << "Nombre" << " | " << std::setw(6) << "PPA" << " | Estado\n";
                    for (const auto& e : arbol.in_order()) {
                        std::cout << std::left << std::setw(10) << e.codigo << " | " << std::setw(15) << e.nombre << " | " << std::setw(6) << e.ppa << " | " << estadoToString(e.estado) << "\n";
                    }
                } else if (sub_opc == 2) {
                    std::cout << "\n[Listado BFS por Niveles C++]\n";
                    for (const auto& e : arbol.bfs()) {
                        std::cout << "  Codigo: " << e.codigo << " | Nombre: " << e.nombre << "\n";
                    }
                }
            } else if (opcion == 9) {
                arbol.ejecutar_benchmark_cpp();
            }else {
                std::cout << "[!] Opcion no valida. Seleccione del 1 al 9.\n";
            }
        } catch (const std::exception& e) {
            // Actividad 13: Captura limpia de errores en consola
            std::cout << "\x1B[31m" << "EXCEPCION CAPTURADA: " << e.what() << "\x1B[0m" << std::endl;
        }
    }

    return 0;
}