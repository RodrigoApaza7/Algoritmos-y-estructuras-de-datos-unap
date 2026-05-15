#include <iostream>
using namespace std;

const int MAX = 5;

class Cola {
private:
    int datos[MAX];
    int frente;
    int final;

public:
    Cola() {
        frente = 0;
        final = -1;
    }

    void enqueue(int valor) {

        if (final == MAX - 1) {
            cout << "Error: Cola llena (overflow)" << endl;
            return;
        }

        final++;
        datos[final] = valor;

        cout << "Insertado: " << valor << endl;
    }

    void dequeue() {

        if (frente > final) {
            cout << "Error: Cola vacia (underflow)" << endl;
            return;
        }

        cout << "Eliminado: " << datos[frente] << endl;
        frente++;
    }

    int front() {

        if (frente > final) {
            cout << "Cola vacia" << endl;
            return -1;
        }

        return datos[frente];
    }

    void mostrar() {

        if (frente > final) {
            cout << "Cola vacia" << endl;
            return;
        }

        cout << "\nContenido de la cola:\n";

        for (int i = frente; i <= final; i++) {
            cout << datos[i] << " ";
        }

        cout << endl;
    }
};

int main() {

    Cola cola;

    cola.enqueue(3);
    cola.enqueue(6);
    cola.enqueue(9);
    cola.enqueue(12);

    cout << "Frente: " << cola.front() << endl;

    cola.dequeue();

    cout << "Nuevo frente: " << cola.front() << endl;

    cola.mostrar();

    return 0;
}