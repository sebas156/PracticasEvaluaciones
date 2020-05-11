#include <iostream>
#include "funciones.h"
#include "objetospertinentes.h"
#include <map>
#include <vector>
using namespace std;
struct ProductoInvetario{
    string nombre;
    int UnidadesPorPaquete;
    float NumeroDePaquetes;
    int PrecioTotal;
};

int main()
{
    map<string,ComboCinema> TodosLosCombos;
    map<string,ProductoInvetario> AlmacenarInventario;
    float prueba;
    string numero = "12.13";
    prueba=StringAFloat(numero);
    vector<string> sujeto{"D","C","B","A"};
    cout<<"Holaminfo"<<endl;
    cout<<1<<endl;
    return 0;
}
