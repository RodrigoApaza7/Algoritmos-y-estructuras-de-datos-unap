#include <iostream>
#include <string>
using namespace std;

string limpiarCadena(const string& cadena) 
{
    string resultado;
    for (char c : cadena) {
        if (isalnum(c)) { // Solo considerar caracteres alfanuméricos
            resultado += tolower(c); // Convertir a minúscula
        }
    }
    return resultado;
}

bool esPalindromo(const string& cadena) 
{
    string cadenaLimpia = limpiarCadena(cadena);
    int inicio = 0;
    int fin = cadenaLimpia.length() - 1;

    while (inicio < fin) {
        if (cadenaLimpia[inicio] != cadenaLimpia[fin]) {
            return false; // No es palíndromo
        }
        inicio++;
        fin--;
    }
    return true; // Es palíndromo
}

int main() {
    string cadena;
    cout << "Ingrese una cadena: ";
    getline(cin, cadena);

    if (esPalindromo(cadena))
        cout << "Es palindromo";
    else
        cout << "No es palindromo";

    return 0;
}