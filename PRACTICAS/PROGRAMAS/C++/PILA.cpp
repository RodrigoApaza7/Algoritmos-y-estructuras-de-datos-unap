#include <iostream>
using namespace std;

const int MAX = 5;

class Pila {
private:
    int datos[MAX];
    int tope;

public:
    Pila() {
        tope = -1;
    }

    void push(int valor) {
        if (tope == MAX - 1) {
            cout << "Error: Pila llena (overflow)" << endl;
            return;
        }

        tope++;
        datos[tope] = valor;

        cout << "Insertado: " << valor << endl;
    }

    void pop() {
        if (tope == -1) {
            cout << "Error: Pila vacia (underflow)" << endl;
            return;
        }

        cout << "Eliminado: " << datos[tope] << endl;
        tope--;
    }

    int top() {
        if (tope == -1) {
            cout << "La pila esta vacia" << endl;
            return -1;
        }

        return datos[tope];
    }

    void mostrar() {
        if (tope == -1) {
            cout << "Pila vacia" << endl;
            return;
        }

        cout << "\nContenido de la pila:\n";

        for (int i = tope; i >= 0; i--) {
            cout << "| " << datos[i] << " |" << endl;
        }

        cout << "-----" << endl;
    }
};

int main() {

    Pila pila;

    pila.push(5);
    pila.push(10);
    pila.push(15);
    pila.push(20);
    pila.push(25);

    pila.pop();
    pila.pop();

    pila.mostrar();

    return 0;
}