#include "funciones.h"
#include "objetospertinentes.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>


using namespace std;

char MenuAdministrador(){
    char opcion;
    cout<<"A) Ver inventario. "<<endl;
    cout<<"B) Agregar productos al inventario. "<<endl;
    cout<<"C) Formar combos. "<<endl;
    cout<<"D) Agregar Usuarios. "<<endl;
    cout<<"E) Generar reporte de las ventas hasta el momento. "<<endl;
    cout<<"F) Mostrar ganancias y perdidas."<<endl;
    cout<<"G) Cerrar Sesion. "<<endl;
    cin>>opcion;
    return opcion;
}

char MenuGeneal(){
    char opcion;
    cout<<"A) Iniciar sesion como administrador. "<<endl;
    cout<<"B) Iniciar sesion como usuario. "<<endl;
    cout<<"C) Salir. "<<endl;
    cin>>opcion;
    return opcion;
}

char MenuUsuario(){
    char opcion;
    cout<<"A) Ver combos disponibles. "<<endl;
    cout<<"B) Comprar combo. "<<endl;
    cout<<"C) Cerrar sesion. "<<endl;
    cin>>opcion;
    return opcion;
}

void LeerCombos( map<string,ComboCinema>&CombosPorLeer){ // Los combos tienen que estar almacenados en un archivo, esta funcion lee ese archivo y los almacena en un map.
    fstream archivo("combos.txt");
    string LeerLinea,nombre,contenido,precio,cuantoshay;
    if(archivo.fail()){
        cout<<"Hubo un error al abrir el archivo. "<<endl;
    }
    else {
        while (!archivo.eof()) {
            getline(archivo,LeerLinea);
            if(LeerLinea!=""){
                ComboCinema auxiliar;
                nombre=LeerLinea.substr(0,LeerLinea.find("+"));
                LeerLinea=LeerLinea.substr(LeerLinea.find("+")+1,LeerLinea.size()-LeerLinea.find("+"));
                contenido=LeerLinea.substr(0,LeerLinea.find("+"));
                LeerLinea=LeerLinea.substr(LeerLinea.find("+")+1,LeerLinea.size()-LeerLinea.find("+"));
                precio=LeerLinea.substr(0,LeerLinea.find("+"));
                LeerLinea=LeerLinea.substr(LeerLinea.find("+")+1,LeerLinea.size()-LeerLinea.find("+"));
                cuantoshay=LeerLinea.substr(0,LeerLinea.find("+"));
                auxiliar.InicializarCombo(nombre,contenido,StringAnumero(precio),StringAnumero(cuantoshay));
                CombosPorLeer.insert(pair<string,ComboCinema>(nombre,auxiliar));
            }
        }
    }
    archivo.close();
}

int StringAnumero(string numero){ // Esta funcion convierte un string a numero
    int resultado=0;
    for (int i =0;i<numero.size();i++) {
       resultado+=numero[i]-48;
        resultado*=10;
    }
    resultado/=10;
    return resultado;
}

void ObservarInventario(){ //Esta funcion imprime en pantalla el inventario.
    int indicador=contar_lineas("inventario.txt");
    if(indicador!=0){
        fstream archivo("inventario.txt");
        string LeerLinea;
        if(archivo.fail())
            cout<<"No se pudo abrir el archivo. "<<endl;
        else {
            while (!archivo.eof()) {
                getline(archivo,LeerLinea);
                if(LeerLinea!=""){
                    for (int i=0 ;i<LeerLinea.size();i++) {
                        if(LeerLinea[i]!='+')
                            cout<<LeerLinea[i];
                        else
                            cout<<"  ";
                    }
                    cout<<endl;
                }
            }
        }

        archivo.close();
    }
    else {
        cout<<"Aun no hay ningun articulo en el inventario. "<<endl;
    }

}

void AgregarArticulosInventario(){   // Esta funcion permite agregar nuevos productos al inventario.
    string NombreProducto;
    char indicador='1';
    int NumeroDeUnidadesPorPaquete;
    int NumeroPaquetes;
    int CostoCadaPaquete;
    cout<<"Recuerda que la informacion se debe agregar de la siguiente forma: "<<endl;
    cout<<"Primero -> Escibes el nombre del producto. Ejemplo: Pan para perro. "<<endl;
    cout<<"Segundo -> Ingresas el numero de unidades que tiene ese paquete. Ejemplo: Si el paquete de pan para perro tiene 6 unidades ingresas: 6"<<endl;
    cout<<"Tercero -> Ingresas el numero de paquetes que compraste. Ejemplo: Si compraste 5 paquetes ingresas: 5"<<endl;
    do{
        char indicador2;
        cout<<"Ingrese le nombre del producto: "<<endl;
        getline(cin,NombreProducto);
        cout<<"Ingrese el numero de unidades por paquetes: "<<endl;
        cin>>NumeroDeUnidadesPorPaquete;
        cout<<"Ingrese el numero de paquetes: "<<endl;
        cin>>NumeroPaquetes;
        cout<<"Ingrese el costo de un paquete: "<<endl;
        cin>>CostoCadaPaquete;
        NombreProducto+="+"+NumeroCadenaCaracterer(NumeroDeUnidadesPorPaquete)+"+"+NumeroCadenaCaracterer(NumeroPaquetes)+"+"+NumeroCadenaCaracterer(NumeroPaquetes*CostoCadaPaquete);
        cout<<endl;
        cout<<"La informacion se va a guardar en la base de datos de la siguiente manera: "<<endl<<endl;
        cout<<NombreProducto;
        do{
            cout<<"Estas de acuerdo? "<<endl;
            cout<<"Si -> 1."<<endl<<"No -> 2."<<endl;
            cin>>indicador2;
            if(indicador2 != '2' and indicador2 != '1')
                cout<<"Opcion invalida. "<<endl;
        }while(indicador2 != '2' and indicador2 != '1');

        if(indicador2=='1'){
            ofstream archivo("inventario.txt",ios::app);
            if(archivo.fail()){
                cout<<"Error al abrir el archivo. "<<endl;
            }
            else {
                archivo<<NombreProducto<<endl;
            }

            do{
                cout<<"Agregar otro producto: "<<endl;
                cout<<"Si -> 1."<<endl<<"No -> 2."<<endl;
                cin>>indicador;
                if(indicador != '2' and indicador != '1')
                    cout<<"Opcion invalida. "<<endl;
            }while(indicador != '2' and indicador != '1');
            archivo.close();
        }

    }while(indicador != '2');
}

string NumeroDocumetnoCadenaCaracteres(long long NumDocumento){ // Los usuarios van a estar registrados con sus cedulas.
    string RetornarConvertido="";                               // Una cedula tiene 10 digitos, por lo que sería muy bueno representar estas con un long long.
    char letra;                                                 //Se representa con un long long para brindar la posibilidad de organizar a los usuarios en el archivo de acuerdo a sus cedulas.
    while (NumDocumento>0) {
        letra=(NumDocumento%10+48);
        RetornarConvertido= letra + RetornarConvertido;
        NumDocumento/=10;
    }
    return RetornarConvertido;
}

string NumeroCadenaCaracterer(int canDinero){// Transforma un entero en una cadena de caracteres.
    string RetornarConvertido="";
    char letra;
    while (canDinero>0) {
        letra=(canDinero%10+48);
        RetornarConvertido= letra + RetornarConvertido;
        canDinero/=10;
    }
    return RetornarConvertido;
}

int contar_lineas(string NombreDelArchivoParaContar){  // Cuentas las lineas que hay en un archivo.
    int contLineas=0;
    ifstream archivo(NombreDelArchivoParaContar);
    if(archivo.fail()){
        cout<<"No se ha podido abrir el archivo para contar las lineas. "<<endl;
    }else {
        while(!archivo.eof()){
            string linea;
            getline(archivo,linea);
            if(linea != "")
                contLineas++;
        }
    }
    archivo.close();
    return contLineas;
}
