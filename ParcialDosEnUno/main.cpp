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
    char OpcionesGneral,OpcionAdmin;
    LeerUsuariosDelArchivo(UsuariosRegistrados);
    LeerInventario(AlmacenarInventario);
    LeerCombos(TodosLosCombos);
    do{
        OpcionesGneral=MenuGeneal();
        switch (OpcionesGneral) {
        case 'A':{
            if(Administrador()){
                do{
                    OpcionAdmin=MenuAdministrador();
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
                        AgregarUsuarios(UsuariosRegistrados);
                        break;
                    }
                    case 'E':{
                        EliminarUsuario(UsuariosRegistrados);
                        break;
                    }
                    case 'F':{
                        break;
                    }
                    case 'G':{
                        break;
                    }
                    case 'I':{
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
            AgregarUsuarios(UsuariosRegistrados);
            break;
        }
        case 'C':{
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
