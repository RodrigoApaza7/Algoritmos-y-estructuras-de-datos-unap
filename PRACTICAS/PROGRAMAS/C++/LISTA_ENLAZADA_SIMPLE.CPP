#include <iostream>

using namespace std;

struct Nodo {
    int dato;
    Nodo* siguiente;
};

class ListaEnlazada 
{
private:
    Nodo* cabeza;

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

        cout << "Insertado al final: " << valor << endl;
    }

    // Eliminar al inicio
    void eliminarInicio() 
    {

        if (cabeza == nullptr) 
        {
            cout << "La lista esta vacia" << endl;
            return;
        }

        Nodo* temporal = cabeza;

        cout << "Eliminado: " << temporal->dato << endl;

        cabeza = cabeza->siguiente;

        delete temporal;
    }

    // Mostrar lista
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

int main() 
{

    ListaEnlazada lista;

    // Insertar al inicio
    lista.insertarInicio(8);
    lista.insertarInicio(4);

    // Insertar al final
    lista.insertarFinal(11);

    // Eliminar primer nodo
    lista.eliminarInicio();

    // Mostrar lista final
    lista.mostrar();

    return 0;
}