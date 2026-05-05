#include <iostream>
#include <string>

using namespace std;

float calcularTarifa(float distancia, int tipoCliente) 
{
    const float costoPorKm = 1.0;  // Costo por kilómetro

    if(tipoCliente == 1) { // Cliente regular
        return costoPorKm * distancia;
    } else if (tipoCliente == 2) { // Cliente premium
        return costoPorKm * distancia * 0.5; // Descuento del 20%
    } else {
        cout << "Tipo de cliente no válido." << endl;
        return -1; // Indicar error
    }
}

int main() 
{
    float distancia;
    int tipoCliente;

    cout << "Ingrese la distancia del viaje (km): ";
    cin >> distancia;
    cout << "Ingrese el tipo de cliente (1 para regular, 2 para premium): ";
    cin >> tipoCliente;

    float tarifa = calcularTarifa(distancia, tipoCliente);
    if (tarifa >= 0) {
        cout << "La tarifa calculada es: $" << tarifa << endl;
    }

    return 0;
}