#include <iostream>
#include "funciones.h"
#include "objetospertinentes.h"
#include <map>
#include <vector>
using namespace std;

int main()
{
    //  No es recomensable cerrar el programa a mitad de la ejecucion.
    map<string,ComboCinema> TodosLosCombos;
    map<string,ProductoInvetario> AlmacenarInventario;
    map<string,Usuario> UsuariosRegistrados;
    char OpcionesGneral,OpcionAdmin,OpcionUser;
    LeerUsuariosDelArchivo(UsuariosRegistrados);
    LeerInventario(AlmacenarInventario);
    LeerCombos(TodosLosCombos);
    Usuario auxiliar;
    auxiliar.inicializandoTodo("Anonimo","","0");
    UsuariosRegistrados.insert(pair<string,Usuario>("Anonimo",auxiliar));
    do{
        OpcionesGneral=MenuGeneal();
        switch (OpcionesGneral) {
        case 'A':{
            if(Administrador()){
                map<string,string> PeticionesPendientes;
                ExtraerSolicitudesDeRegistro(PeticionesPendientes);
                do{
                    OpcionAdmin=MenuAdministrador();
                    switch (OpcionAdmin) {
                    case 'A':{
                        ObservarInventario(AlmacenarInventario);
                        break;
                    }
                    case 'B':{
                        AgregarAInventario(AlmacenarInventario);
                        break;
                    }
                    case 'C':{
                        FormarCombos(AlmacenarInventario,TodosLosCombos);
                        break;
                    }
                    case 'D':{
                        if(!PeticionesPendientes.empty()){
                            AgregarUsuarios(UsuariosRegistrados,PeticionesPendientes);
                        }
                        else
                            cout<<"Imposible agregar algun usuario si no hay peticiones. "<<endl;
                        break;
                    }
                    case 'E':{
                        EliminarUsuario(UsuariosRegistrados);
                        break;
                    }
                    case 'F':{
                        ObservarPeticiones(PeticionesPendientes);
                        break;
                    }
                    case 'G':{
                        if(contar_lineas("RegistroDeVentas.txt")==0)
                            cout<<"Aun no hay un registro disponible. "<<endl;
                        else {
                            ImprimirRegistroVentas();
                        }
                        break;
                    }
                    case 'H':{
                        for(auto i:PeticionesPendientes){
                            NotificarPeticionDeRegistro(i.first,i.second);
                        }
                        UsuariosRegistrados.erase("Anonimo");
                        EscribirArchivoInventario(AlmacenarInventario);
                        cout<<"Todos los cambios se han guardado correctamente."<<endl;
                        break;
                    }
                    }
                }while(OpcionAdmin != 'H');
            }
            break;
        }
        case 'B':{
                string NombreUsuario;
                string contrasena;
                cout<<"Ingrese su nombre de usuario: "<<endl;
                getline(cin,NombreUsuario);
                getline(cin,NombreUsuario);
                cout<<"Ingrese su contrasena: "<<endl;
                getline(cin,contrasena);
                if(UsuariosRegistrados.find(NombreUsuario)!=UsuariosRegistrados.end()){
                    if(UsuariosRegistrados[NombreUsuario].clave==contrasena){
                        do{
                        cout<<endl;
                        cout<<"Usario: "<<NombreUsuario<<endl;
                        OpcionUser=MenuUsuario();
                        switch (OpcionUser) {
                        case 'A':{
                            ObservarProductos(TodosLosCombos);
                            break;
                        }
                        case 'B':{
                            RealizarCompra(UsuariosRegistrados,TodosLosCombos,AlmacenarInventario,NombreUsuario);
                            break;
                        }

                        case 'C':{
                            cout<<"Gracias por comprar en nuestro cinema. "<<endl;
                            cout<<"Distruta tu pelicula. "<<endl;
                            break;
                        }
                        default:
                            cout<<"Opcion No especificada. "<<endl;
                            break;
                        }
                        }while(OpcionUser!='C');
                    }
                    else {
                        cout<<"Usuario o contrasena incorrectos. "<<endl;
                    }
                }
                else {
                    cout<<"Usuario o contrasena incorrectos. "<<endl;
                }
            break;
        }
        case 'C':{
            RealizarPeticion(UsuariosRegistrados);
            break;
        }
        case 'D':{
            string NombreUsuario="Anonimo";
            do{
            OpcionUser=MenuUsuario();
            switch (OpcionUser) {
            case 'A':{
                ObservarProductos(TodosLosCombos);
                break;
            }
            case 'B':{
                RealizarCompra(UsuariosRegistrados,TodosLosCombos,AlmacenarInventario,NombreUsuario);
                break;
            }

            case 'C':{
                cout<<"Gracias por comprar en nuestro cinema. "<<endl;
                cout<<"Distruta tu pelicula. "<<endl;
                break;
            }
            default:
                cout<<"Opcion No especificada. "<<endl;
                break;
            }
            }while(OpcionUser!='C');
            break;
        }
        case 'E':{
            EscribirEnArchiParaCombos(TodosLosCombos);
            EscribirUsuariosEnElArchivo(UsuariosRegistrados);
            cout<<"Todos los combios han sido guardados exitosamente. "<<endl;
            break;
        }
        default:{
            cout<<"La opcion ingresada no s reconoce. "<<endl;
            break;
        }

        }
    }while(OpcionesGneral != 'E');
    return 0;
}
