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
bool Administrador();
void LeerUsuariosDelArchivo( map<string,Usuario>&);
void EscribirUsuariosEnElArchivo(map<string,Usuario>&);
void EliminarUsuario(map<string,Usuario>&);
void AgregarUsuarios( map<string,Usuario> &);
int contar_lineas(string );
string NumeroDocumetnoCadenaCaracteres(long long);
string NumeroCadenaCaracterer(int);
void MostrarCombos( map<string,ComboCinema>&);
int StringAnumero(string);
float StringAFloat(string);
bool verificando_cedula(string,map<string,Usuario> &);
bool verificando_clave(string);
void IncorporarUsuarioAlSistema( map<string,Usuario> &UsuariosRegistrados,string, string,string);
void LeerInventario( map<string,ProductoInvetario>&);
void ObservarInventario(map<string,ProductoInvetario>&);
void LeerCombos( map<string,ComboCinema>&);
#endif // FUNCIONES_H
