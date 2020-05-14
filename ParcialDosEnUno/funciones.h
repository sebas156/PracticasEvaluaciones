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
void AgregarUsuarios( map<string,Usuario> &,map<string,string> & );
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
void LeerArticulosParaAgregarInventario(map<string,ProductoInvetario>&);
void EscribirRegistroDeComprasParaInventario(string );
bool SiExisteLaComidaSuficienteEnElInventario(map<string,ProductoInvetario>&, string,int);
void GastarRecursosInventario(map<string,ProductoInvetario>&,string , int);
void CrearNuevoCombo(map<string,ProductoInvetario>&,string,map<string,ComboCinema> &);
bool QueSeNecesitaParaCadaPreparacion(map<string,ProductoInvetario>&,map<string,int>,int);
void EjecutarGastoAutomatico(map<string,ProductoInvetario>&,map<string,int>,int);
map<string,int> DesglozarElementosDelContenidoDeUnCombo(string &);
void FormarCombos(map<string,ProductoInvetario>&, map<string,ComboCinema>& );
void AgregarAInventario(map<string,ProductoInvetario>&,int &);
void EscribirArchivoInventario(map<string,ProductoInvetario>&);
void EscribirEnArchiParaCombos(map<string,ComboCinema>&);
bool HayOtraPeticionConElMismoNombre(string);
void NotificarPeticionDeRegistro(string nombre, string);
void ExtraerSolicitudesDeRegistro(map<string,string> &);
void ObservarPeticiones(map<string,string> &);
void RealizarPeticion( map<string,Usuario> &);
void SeleccionarSaborGaseosa(map<string,ProductoInvetario>, int, string);
void precios_individuales( int &,int);
void EscribirLineaRegistroVentas(string);
void cambio_diniero(int *);
void PagarCuenta(int);
#endif // FUNCIONES_H
