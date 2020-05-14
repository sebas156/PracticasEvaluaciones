#include <iostream>
#include "funciones.h"
#include "objetospertinentes.h"
#include <map>
#include <vector>
using namespace std;

int main()
{
    int inversion=0;
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
                do{
                    OpcionAdmin=MenuAdministrador();
                    map<string,string> PeticionesPendientes;
                    ExtraerSolicitudesDeRegistro(PeticionesPendientes);
                    switch (OpcionAdmin) {
                    case 'A':{
                        ObservarInventario(AlmacenarInventario);
                        break;
                    }
                    case 'B':{
                        AgregarAInventario(AlmacenarInventario,inversion);
                        break;
                    }
                    case 'C':{
                        FormarCombos(AlmacenarInventario,TodosLosCombos);
                        break;
                    }
                    case 'D':{
                        if(!PeticionesPendientes.empty())
                            AgregarUsuarios(UsuariosRegistrados,PeticionesPendientes);
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
                        break;
                    }
                    case 'I':{
                        for(auto i:PeticionesPendientes){
                            NotificarPeticionDeRegistro(i.first,i.second);
                        }
                        UsuariosRegistrados.erase("Anonimo");
                        EscribirArchivoInventario(AlmacenarInventario);
                        cout<<"Todos los cambios se han guardado correctamente."<<endl;
                        break;
                    }
                    }
                }while(OpcionAdmin != 'I');
            }
            break;
        }
        case 'B':{
                do{
                OpcionUser=MenuUsuario();

                }while(OpcionUser!='C');
            break;
        }
        case 'C':{
            RealizarPeticion(UsuariosRegistrados);
            break;
        }
        case 'D':{
            break;
        }
        case 'E':{
            EscribirUsuariosEnElArchivo(UsuariosRegistrados);
            cout<<"Todos los combios han sido guardados exitosamente. "<<endl;
            break;
        }
        default:{
            cout<<"La opcion ingresada no s reconoce. "<<endl;
            break;
        }

        }
    }while(OpcionesGneral != 'C');
    return 0;
}
