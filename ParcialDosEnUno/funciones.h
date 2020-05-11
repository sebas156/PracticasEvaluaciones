#ifndef FUNCIONES_H
#define FUNCIONES_H
#include <string>
#include <iostream>
#include <map>
#include "objetospertinentes.h"

using namespace std;
char MenuAdministrador();
char MenuGeneal();
char MenuUsuario();
char SubMenuFormarCombo();
int contar_lineas(string );
string NumeroDocumetnoCadenaCaracteres(long long);
string NumeroCadenaCaracterer(int);
void MostrarCombos( map<string,ComboCinema>&);
int StringAnumero(string);
float StringAFloat(string);
bool verificando_cedula(string );
bool verificando_clave(string);
#endif // FUNCIONES_H
