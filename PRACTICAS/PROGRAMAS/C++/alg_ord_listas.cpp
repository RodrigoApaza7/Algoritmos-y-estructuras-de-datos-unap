#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;

struct Nodo {
    int dato;
    Nodo* siguiente;
};

class ListaEnlazada 
{
private:
    Nodo* cabeza;
    long comparaciones = 0;
    long intercambios = 0;
public:
    ListaEnlazada()
    {
        cabeza = nullptr;
    }

    // Insertar al inicio

    void insertarInicio(int valor) 
    {
        Nodo* nuevo = new Nodo();

        nuevo->dato = valor;
        nuevo->siguiente = cabeza;

        cabeza = nuevo;

        cout << "Insertado al inicio: " << valor << endl;
    }

    // Insertar al final
    void insertarFinal(int valor) 
    {

        Nodo* nuevo = new Nodo();

        nuevo->dato = valor;
        nuevo->siguiente = nullptr;

        // Si la lista esta vacia
        if (cabeza == nullptr) 
        {
            cabeza = nuevo;
        }
        else 
        {

            Nodo* auxiliar = cabeza;

            // Recorrer hasta el ultimo nodo
            while (auxiliar->siguiente != nullptr) 
            {
                auxiliar = auxiliar->siguiente;
            }

            auxiliar->siguiente = nuevo;
        }
    }

    void mostrar() 
    {

        if (cabeza == nullptr) 
        {
            cout << "La lista esta vacia" << endl;
            return;
        }

        Nodo* auxiliar = cabeza;

        cout << "\nContenido de la lista:\n";

        while (auxiliar != nullptr) 
        {

            cout << "[" << auxiliar->dato << "]";

            if (auxiliar->siguiente != nullptr) 
            {
                cout << " -> ";
            }

            auxiliar = auxiliar->siguiente;
        }

        cout << " -> NULL" << endl;
    }

    ListaEnlazada& operator=(const ListaEnlazada& otra) 
    {
        if (this == &otra) 
        {
            return *this; // Evitar autoasignación
        }

        // Liberar memoria de la lista actual
        Nodo* actual = cabeza;

        while (actual != nullptr) 
        {
            Nodo* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }

        cabeza = nullptr;

        // Copiar nodos de la otra lista
        Nodo* otroActual = otra.cabeza;

        while (otroActual != nullptr) 
        {
            insertarFinal(otroActual->dato);
            otroActual = otroActual->siguiente;
        }

        return *this;
    }

    void bubbleSort()
    {
        if(cabeza == nullptr)
        {
            return;
        }

        bool cambiado;
        long comparaciones = 0;
        long intercambios = 0;

        do
        {
            cambiado = false;

            Nodo* actual = cabeza;

            while(actual->siguiente != nullptr)
            {
                comparaciones++;
                if(actual->dato > actual->siguiente->dato)
                {
                    // Intercambio

                    int temp = actual->dato;

                    actual->dato = actual->siguiente->dato;

                    actual->siguiente->dato = temp;

                    cambiado = true;
                    intercambios++;
                }

                actual = actual->siguiente;

                
            }

        } while(cambiado);
        cout << "Ordenamiento completado. Comparaciones: " << comparaciones << ", Intercambios: " << intercambios << endl;
    }

    void selectionSort()
    {
        if(cabeza == nullptr)
        {
            return;
        }

        long comparaciones = 0;
        long intercambios = 0;

        Nodo* actual = cabeza;

        while(actual != nullptr)
        {
            Nodo* minimo = actual;

            Nodo* recorrido = actual->siguiente;

            // Buscar el menor elemento
            while(recorrido != nullptr)
            {
                comparaciones++;

                if(recorrido->dato < minimo->dato)
                {
                    minimo = recorrido;
                }

                recorrido = recorrido->siguiente;
            }

            // Intercambiar datos
            if(minimo != actual)
            {
                int temp = actual->dato;

                actual->dato = minimo->dato;

                minimo->dato = temp;

                intercambios++;
            }

            actual = actual->siguiente;
        }

        cout << "Selection Sort completado." << endl;

        cout << "Comparaciones: " << comparaciones << endl;

        cout << "Intercambios: " << intercambios << endl;
    }

    void insertionSort()
    {
        if(cabeza == nullptr || cabeza->siguiente == nullptr)
        {
            return;
        }

        long comparaciones = 0;
        long intercambios = 0;

        Nodo* ordenada = nullptr;
        Nodo* actual = cabeza;

        while(actual != nullptr)
        {
            Nodo* siguiente = actual->siguiente;

            if(ordenada == nullptr || actual->dato < ordenada->dato)
            {
                comparaciones++;

                actual->siguiente = ordenada;
                ordenada = actual;

                intercambios++;
            }
            else
            {
                Nodo* temp = ordenada;

                while(temp->siguiente != nullptr &&
                    temp->siguiente->dato < actual->dato)
                {
                    comparaciones++;
                    temp = temp->siguiente;
                }

                actual->siguiente = temp->siguiente;
                temp->siguiente = actual;

                intercambios++;
            }

            actual = siguiente;
        }

        cabeza = ordenada;

        cout << "Insertion Sort completado. Comparaciones: "
            << comparaciones
            << ", Intercambios: "
            << intercambios << endl;
    }

    Nodo* fusionar(Nodo* izquierda, Nodo* derecha)
    {
        if(izquierda == nullptr)
        {
            return derecha;
        }

        if(derecha == nullptr)
        {
            return izquierda;
        }

        Nodo* resultado = nullptr;

        comparaciones++;

        if(izquierda->dato <= derecha->dato)
        {
            resultado = izquierda;

            resultado->siguiente =
                fusionar(izquierda->siguiente, derecha);
        }
        else
        {
            resultado = derecha;

            resultado->siguiente =
                fusionar(izquierda, derecha->siguiente);
        }

        intercambios++;

        return resultado;
    }

    void dividirLista(Nodo* fuente,
                    Nodo** izquierda,
                    Nodo** derecha)
    {
        Nodo* rapido;
        Nodo* lento;

        if(fuente == nullptr ||
        fuente->siguiente == nullptr)
        {
            *izquierda = fuente;
            *derecha = nullptr;
        }
        else
        {
            lento = fuente;
            rapido = fuente->siguiente;

            while(rapido != nullptr)
            {
                rapido = rapido->siguiente;

                if(rapido != nullptr)
                {
                    lento = lento->siguiente;

                    rapido = rapido->siguiente;
                }
            }

            *izquierda = fuente;

            *derecha = lento->siguiente;

            lento->siguiente = nullptr;
        }
    }

    void mergeSortRecursivo(Nodo** cabezaRef)
    {
        Nodo* cabezaActual = *cabezaRef;

        if(cabezaActual == nullptr ||
        cabezaActual->siguiente == nullptr)
        {
            return;
        }

        Nodo* izquierda;
        Nodo* derecha;

        dividirLista(cabezaActual,
                    &izquierda,
                    &derecha);

        mergeSortRecursivo(&izquierda);

        mergeSortRecursivo(&derecha);

        *cabezaRef = fusionar(izquierda, derecha);
    }

    void mergeSort()
    {
        comparaciones = 0;
        intercambios = 0;

        mergeSortRecursivo(&cabeza);

        cout << "Merge Sort completado." << endl;

        cout << "Comparaciones: "
            << comparaciones << endl;

        cout << "Intercambios: "
            << intercambios << endl;
    }

    // Liberar memoria
    ~ListaEnlazada()
    {
        Nodo* actual = cabeza;

        while (actual != nullptr) 
        {
            Nodo* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
    }
};

void inicio()
{
    ListaEnlazada lista;
    ListaEnlazada lista2, lista3, lista4;
    srand(time(0));
    for(int i = 0; i < 10000; i++)
    {
        // Genera un número aleatorio entre 1 y 100
        int numero = rand() % 100 + 1;
        lista.insertarFinal(numero);
    }
    lista2=lista; // Copia la lista para usarla en otro ordenamiento
    lista3=lista; // Copia la lista para usarla en otro ordenamiento
    lista4=lista; // Copia la lista para usarla en otro ordenamiento
    auto inicio = chrono::high_resolution_clock::now();
    lista.bubbleSort();
    auto fin = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio);
    cout << "Tiempo de ordenamiento: " << duracion.count() << " ms" << endl;

    inicio = chrono::high_resolution_clock::now();
    lista2.selectionSort();
    fin = chrono::high_resolution_clock::now();
    duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio);
    cout << "Tiempo de ordenamiento: " << duracion.count() << " ms" << endl;

    inicio = chrono::high_resolution_clock::now();
    lista3.insertionSort();
    fin = chrono::high_resolution_clock::now();
    duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio);
    cout << "Tiempo de ordenamiento: " << duracion.count() << " ms" << endl;

    inicio = chrono::high_resolution_clock::now();
    lista4.mergeSort();
    fin = chrono::high_resolution_clock::now();
    duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio);
    cout << "Tiempo de ordenamiento: " << duracion.count() << " ms" << endl;
}

int main() 
{
    inicio();

    return 0;
}