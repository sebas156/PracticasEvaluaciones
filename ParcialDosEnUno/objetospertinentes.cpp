#include "objetospertinentes.h"

void Usuario::inicializandoTodo(string DocumentIngresado,string contrasena,string PuntosParaAcumular ){
    puntos=0;
    documento=DocumentIngresado;
    for (int i =0;i<PuntosParaAcumular.size();i++) {
        puntos+=PuntosParaAcumular[i]-48;
        puntos*=10;
    }
    puntos/=10;

    clave=contrasena;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ComboCinema::InicializarCombo(string NombreInventado, string Ingredientes, int valor,int cuantos){
    Nombre=NombreInventado;
    Componentes=Ingredientes;
    precio=valor;
    CuantosDeEsteHay=cuantos;
}
