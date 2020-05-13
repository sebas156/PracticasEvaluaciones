#ifndef OBJETOSPERTINENTES_H
#define OBJETOSPERTINENTES_H
#include <iostream>
#include <string>
using namespace std;

struct ProductoInvetario{
    string nombre;
    int UnidadesPorPaquete;
    float NumeroDePaquetes;
    int precio;
};

class Usuario
{
public:
    string documento;
    string clave;
    int puntos;
    void inicializandoTodo(string,string,string);
};

class ComboCinema
{
public:
    string Nombre;
    string Componentes;
    int precio;
    int CuantosDeEsteHay;
    void InicializarCombo(string,string,int,int);
};

#endif // OBJETOSPERTINENTES_H
