#include <iostream>
#include "funciones.h"
#include "objetospertinentes.h"
#include <map>
#include <vector>
using namespace std;

int main()
{
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
                    case 'D':{
                        AgregarUsuarios(UsuariosRegistrados);
                        break;
                    }
                    case 'E':{
                        EliminarUsuario(UsuariosRegistrados);
                        break;
                    }

                    }
                }while(OpcionAdmin != 'H');
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
