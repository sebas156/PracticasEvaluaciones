#include "funciones.h"
#include "objetospertinentes.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
using namespace std;

char MenuAdministrador(){
    char opcion;
    cout<<"A) Ver inventario. "<<endl;
    cout<<"B) Agregar productos al inventario. "<<endl;
    cout<<"C) Combos. "<<endl;
    cout<<"D) Agregar Usuarios. "<<endl;
    cout<<"E) Eliminar Usuarios. "<<endl;
    cout<<"F) Generar reporte de las ventas hasta el momento. "<<endl;
    cout<<"G) Mostrar ganancias y perdidas."<<endl;
    cout<<"H) Cerrar Sesion. "<<endl;
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
char SubMenuFormarCombo(){
    char opcion;
    cout<<"1) Observar combos existentes. "<<endl;
    cout<<"2) Recargar un combo. "<<endl;
    cout<<"3) Crear un nuevo combo. "<<endl;
    cout<<"4) Eliminar un combo. "<<endl;
    cout<<"5) Salir. "<<endl;
    cin>>opcion;
}

bool Administrador(){

    fstream archivo("sudo.txt");
    string clave;
    string LeerLinea;
    cout<<"Ingrese la clave de administrador: "<<endl;
    cin>>clave;
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo: "<<endl;
        return false;
    }
    else {
        getline(archivo,LeerLinea);
        if(LeerLinea==clave){
            cout<<"Sesion iniciada. Bienvenido."<<endl;
            return true;
        }
        else{
             cout<<"Contrasena incorrecta. "<<endl;
            return false;
        }
    }
}

void LeerUsuariosDelArchivo( map<string,Usuario>& UsuariosRegistrados){
    fstream archivo("usuarios.txt");
    string LeerLinea,nombre,clave,puntos;
    if(archivo.fail()){
        cout<<"Error al abrir el archivo."<<endl;
    }
    else {
        while (!archivo.eof()) {
            getline(archivo,LeerLinea);
            if(LeerLinea!=""){
                nombre=LeerLinea.substr(0,LeerLinea.find("+"));
                LeerLinea=LeerLinea.substr(LeerLinea.find("+")+1, LeerLinea.size()- LeerLinea.find("+"));
                clave=LeerLinea.substr(0,LeerLinea.find("+"));
                LeerLinea=LeerLinea.substr(LeerLinea.find("+")+1, LeerLinea.size()- LeerLinea.find("+"));
                puntos=LeerLinea.substr(0,LeerLinea.find("+"));
                Usuario auxiliar;
                auxiliar.inicializandoTodo(nombre,clave,puntos);
                UsuariosRegistrados.insert(pair<string,Usuario>(nombre,auxiliar));
            }
        }
    }
    archivo.close();
}

void EscribirUsuariosEnElArchivo(map<string,Usuario>& UsuariosRegistrados){
    ofstream archivo("usuarios.txt");
    string linea="";
    if(archivo.fail()){
        cout<<"Error al abrir el archivo. "<<endl;
    }
    else {
        for (auto i = UsuariosRegistrados.begin();i!=UsuariosRegistrados.end();i++) {
            linea=i->second.documento+ "+" + i->second.clave+ "+" + NumeroCadenaCaracterer(i->second.puntos)+ "+";
            archivo<<linea<<endl;
        }
    }
    archivo.close();
}

void EliminarUsuario(map<string,Usuario>& UsuariosRegistrados){
    string UsuarioEliminar;
    cout<<"Ingrese el nombre del usuario que desea eliminar: "<<endl;
    getline(cin,UsuarioEliminar);
    getline(cin,UsuarioEliminar);
    auto i = UsuariosRegistrados.find(UsuarioEliminar);
    if(i!=UsuariosRegistrados.end()){
        UsuariosRegistrados.erase(UsuarioEliminar);
        cout<<"Usuario eliminado exitosamente. "<<endl;
    }
    else {
        cout<<"No se puede eliminar un usuario que no esta registrado. "<<endl;
    }
}

bool verificando_cedula(string documentoING,map<string,Usuario> &UsuariosRegistrados){
    auto i=UsuariosRegistrados.find(documentoING);
    if(i!=UsuariosRegistrados.end()){
        cout<<"El nombre de usuario ya se encuentra en uso. "<<endl;
        return false;
    }
    else {
        return true;
    }
}


bool verificando_clave(string contrasena){
    int numeros=0,letras=0;
    if(contrasena.size()<8 or contrasena.size()>8){
        return false;
    } else{
        for (int i=0;i<contrasena.size();i++) {
            if(contrasena[i]>=48 and contrasena[i]<=57)
                numeros++;
            else if((contrasena[i]>=97 and contrasena[i]<=122)or(contrasena[i]>=65 and contrasena[i]<=90))
                letras++;
        }
        if(numeros != 0 and letras != 0)
            return true;
        else
            return false;
    }
}

void IncorporarUsuarioAlSistema( map<string,Usuario> &UsuariosRegistrados,string nombre, string contrasena,string puntos){
    Usuario auxilar;
    auxilar.inicializandoTodo(nombre,contrasena,puntos);
    UsuariosRegistrados.insert(pair<string,Usuario>(nombre,auxilar));
}

void AgregarUsuarios( map<string,Usuario> &UsuariosRegistrados){
    string documento,clave;
    getline(cin,documento);
    do{
        cout<<"Ingrese el nombre con el cual el usuario desea quedar registrado: "<<endl;
        getline(cin,documento);
    }while(!verificando_cedula(documento,UsuariosRegistrados));

    do{
        cout<<"La clave del usuario debe ser de exactamente 8 caracteres; ademas, debe contener numeros y letras. "<<endl;
        cout<<"Ingrese la clave del usuario: "<<endl;
        cin>>clave;
    }while(!verificando_clave(clave));

    IncorporarUsuarioAlSistema(UsuariosRegistrados,documento,clave,"0");
    cout<<"El usuario se ha agregado exitosamente. "<<endl;
}


void GastarRecursosInventario(map<string,ProductoInvetario>& AlmacenarInventario,string PosicionProductoUsado, int UnidadesUsadas){
    auto i = AlmacenarInventario.find( PosicionProductoUsado);
    i->second.NumeroDePaquetes= (((i->second.UnidadesPorPaquete)*(i->second.NumeroDePaquetes))-UnidadesUsadas)*(i->second.NumeroDePaquetes)/((i->second.UnidadesPorPaquete)*(i->second.NumeroDePaquetes));
}

void CrearNuevoCombo(map<string,ProductoInvetario>& AlmacenarInventario,string NombreCombo,map<string,ComboCinema> &TodosLosCombos){
   vector<string> ParaFormarUnCombo{"Hamburguesa","Perro Caliente","Nachos","Doritos","Detodito","Caja de palomitas 170 Oz","Caja de palomitas 130 Oz","Caja de palomitas 85 Oz","Vaso Plastico 44 Oz","Vaso Plastico 32 Oz","Vaso Plastico 22 Oz"};
   map<string,ProductoInvetario> LeerProductosAgregarInventario;
   LeerArticulosParaAgregarInventario(LeerProductosAgregarInventario);
   string contenido="";
   int CantidadComida;
   int CostoCombo=0;
   int elemento;
   char opcion;
   bool bandera;
    for(int i=0;i<ParaFormarUnCombo.size();i++){
        cout<<i+1<<") "<<ParaFormarUnCombo[i]<<endl;
    }
    do{

        cout<<"Ingrese la posicion de la preparacion que va a incluir el combo: "<<endl;
        cin>>elemento;
        cout<<"Ingrese el numero de estas preparaciones que tendra el combo: "<<endl;
        cin>>CantidadComida;
        switch (elemento) {
           case 1:{
            for (int i =1;i<=4;i++) {
                bandera=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,NumeroCadenaCaracterer(i),1*CantidadComida);
                if(bandera==false){
                    cout<<"No hay suministros suficientes..."<<endl;
                    break;
                }
                GastarRecursosInventario(AlmacenarInventario,NumeroCadenaCaracterer(i),1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario[NumeroCadenaCaracterer(i)].precio/LeerProductosAgregarInventario[NumeroCadenaCaracterer(i)].NumeroDePaquetes)*CantidadComida;
            }
            if(bandera != false){
                if(opcion == '1')
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Hamburguesa";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Hamburguesas";
                }
            }
        break;
        }
        case 2:{
            for(int i=5;i<=8;i++){

                if(i != 7){
                    bandera=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,NumeroCadenaCaracterer(i),1*CantidadComida);
                    if(bandera==false){
                        cout<<"No hay suministros suficientes..."<<endl;
                        break;
                    }
                     GastarRecursosInventario(AlmacenarInventario,NumeroCadenaCaracterer(i),1*CantidadComida);
                     CostoCombo+=(LeerProductosAgregarInventario[NumeroCadenaCaracterer(i)].precio/LeerProductosAgregarInventario[NumeroCadenaCaracterer(i)].NumeroDePaquetes)*CantidadComida;
                }
                else{
                    bandera=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,NumeroCadenaCaracterer(i),14*CantidadComida);
                    if(bandera==false){
                        cout<<"No hay suministros suficientes..."<<endl;
                        break;
                    }
                    GastarRecursosInventario(AlmacenarInventario,NumeroCadenaCaracterer(i),14*CantidadComida);
                    CostoCombo+=(LeerProductosAgregarInventario[NumeroCadenaCaracterer(i)].precio/LeerProductosAgregarInventario[NumeroCadenaCaracterer(i)].NumeroDePaquetes)*CantidadComida;
                    }
            }
            if(bandera == true){
                bandera=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"3",1*CantidadComida);
                if(bandera==true){
                    GastarRecursosInventario(AlmacenarInventario,"3",1*CantidadComida);
                    CostoCombo+=(LeerProductosAgregarInventario["3"].precio/LeerProductosAgregarInventario["3"].NumeroDePaquetes)*CantidadComida;
                    if(opcion == '1')
                        contenido+=",";
                    if(CantidadComida==1){
                        contenido+= NumeroCadenaCaracterer(CantidadComida)+" Perro caliente";
                    }
                    else {
                        contenido+= NumeroCadenaCaracterer(CantidadComida)+" Perros calientes";
                    }
                }
                else {
                    cout<<"No hay suministros suficientes..."<<endl;
                }
            }
            else {
                cout<<"No hay suministros suficientes..."<<endl;
            }
            break;
        }
        case 3:{
            bandera=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"9",1*CantidadComida);
            if(bandera==true){
                GastarRecursosInventario(AlmacenarInventario,"9",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["9"].precio/LeerProductosAgregarInventario["9"].NumeroDePaquetes)*CantidadComida;
                if(opcion == '1')
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Paquete de nachos";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Paquetes de nachos";
                }
            }
            else {
                cout<<"No hay suministros..."<<endl;
                cout<<"Deficit de paquetes de nachos..."<<endl;
            }
            break;
        }
        case 4:{
            bandera=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"10",1*CantidadComida);
            if(bandera==true){
                GastarRecursosInventario(AlmacenarInventario,"10",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["10"].precio/LeerProductosAgregarInventario["10"].NumeroDePaquetes)*CantidadComida;
                if(opcion == '1')
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Paquete de doritos";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Paquetes de doritos";
                }
            }
            else {
                cout<<"No hay suministros suficientes..."<<endl;
                cout<<"Deficit de paquetes de doritos.."<<endl;
            }
            break;
        }
        case 5:{
            bandera=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"11",1*CantidadComida);
            if(bandera==true){
                GastarRecursosInventario(AlmacenarInventario,"11",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["11"].precio/LeerProductosAgregarInventario["11"].NumeroDePaquetes)*CantidadComida;
                if(opcion == '1')
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Paquete de detodito";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Paquetes de detodito";
                }
            }
            else {
                cout<<"No hay suministros suficientes..."<<endl;
                cout<<"Deficit de paquetes de detoditos..."<<endl;
            }
            break;
        }
        case 6:{
            if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"16",1*CantidadComida)==true and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"12",170*CantidadComida)==true){
                GastarRecursosInventario(AlmacenarInventario,"16",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["16"].precio/LeerProductosAgregarInventario["16"].NumeroDePaquetes)*CantidadComida;
                GastarRecursosInventario(AlmacenarInventario,"12",170*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["12"].precio/LeerProductosAgregarInventario["12"].NumeroDePaquetes)*CantidadComida*170;
                if(opcion == '1')
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Caja grande de palomitas";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Cajas grandes de palomitas";
                }
            }
            else {
                cout<<"No hay suficientes suministros..."<<endl;
                cout<<"Se recomienda revisar el abastecimiento de crispetas y cajas grandes para crispetas. "<<endl;
            }
            break;
        }
        case 7:{
            if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"17",1*CantidadComida)==true and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"12",150*CantidadComida)==true ){
                GastarRecursosInventario(AlmacenarInventario,"17",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["17"].precio/LeerProductosAgregarInventario["17"].NumeroDePaquetes)*CantidadComida;
                GastarRecursosInventario(AlmacenarInventario,"12",150*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["12"].precio/LeerProductosAgregarInventario["12"].NumeroDePaquetes)*CantidadComida*150;
                if(opcion == '1')
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Caja mediana de palomitas";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Cajas medianas de palomitas";
                }
            }
            else {
                cout<<"No hay recursos suficientes..."<<endl;
                 cout<<"Se recomienda revisar el abastecimiento de crispetas y cajas medianas para crispetas. "<<endl;
            }
            break;
        }
        case 8:{
            if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"18",1*CantidadComida) == true and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"12",130*CantidadComida)==true){
                GastarRecursosInventario(AlmacenarInventario,"18",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["18"].precio/LeerProductosAgregarInventario["18"].NumeroDePaquetes)*CantidadComida;
                GastarRecursosInventario(AlmacenarInventario,"12",130*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["12"].precio/LeerProductosAgregarInventario["12"].NumeroDePaquetes)*CantidadComida*130;
                if(opcion == '1')
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Caja pequena de palomitas";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Cajas pequenas de palomitas";
                }
            }
            else {
                cout<<"No hay suministros suficientes..."<<endl;
                cout<<"Se recomienda revisar el abastecimiento de crispetas y cajas pequenas para crispetas. "<<endl;
            }
            break;
        }
        case 9:{
            if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"13",1*CantidadComida)==false){
                cout<<"No hay recursos suficientes..."<<endl;
                cout<<"Se recomienda revisar los vasos plasticos de 44 Oz..."<<endl;
            }
            else if((SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"19",44*CantidadComida)==false and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"20",44*CantidadComida) and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"21",44*CantidadComida) and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"22",44*CantidadComida) and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"23",44*CantidadComida))){
                cout<<"No hay recursos suficientes..."<<endl;
                cout<<"Se recomienda revisar las gaseosas..."<<endl;
            }
            else {
                GastarRecursosInventario(AlmacenarInventario,"13",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["13"].precio/LeerProductosAgregarInventario["13"].NumeroDePaquetes)*CantidadComida;
                if(opcion == '1')
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Vaso de gaseosa grande";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Vasos de gaseosa grande";
                }
            }
            break;
        }
        case 10:{
            if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"14",1*CantidadComida)==false){
                cout<<"No hay recursos suficientes..."<<endl;
                cout<<"Se recomienda revisar los Vasos plasticos 32 Oz"<<endl;
            }
            else if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"19",32*CantidadComida)==false and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"20",32*CantidadComida) and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"21",32*CantidadComida) and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"22",32*CantidadComida) and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"23",32*CantidadComida)) {
                cout<<"No hay recursos suficientes..."<<endl;
                cout<<"Se recomienda revisar las gaseosas "<<endl;
            }
            else {
                GastarRecursosInventario(AlmacenarInventario,"14",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["14"].precio/LeerProductosAgregarInventario["14"].NumeroDePaquetes)*CantidadComida;
                if(opcion == '1')
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Vaso de gaseosa mediana";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Vasos de gaseosa mediana";
                }
            }
            break;
        }
        case 11:{
            if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"15",1*CantidadComida)==false){
                cout<<"No hay recursos suficientes..."<<endl;
                cout<<"Se recomienda revisar los Vasos plasticos 22 Oz"<<endl;
            }
            else if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"19",32*CantidadComida)==false and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"20",32*CantidadComida) and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"21",32*CantidadComida) and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"22",32*CantidadComida) and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"23",32*CantidadComida)) {
                cout<<"No hay recursos suficientes..."<<endl;
                cout<<"Se recomienda revisar las gaseosas "<<endl;
            }
            else {
                GastarRecursosInventario(AlmacenarInventario,"15",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["15"].precio/LeerProductosAgregarInventario["15"].NumeroDePaquetes)*CantidadComida;
                if(opcion == '1')
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Vaso de gaseosa pequena";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Vasos de gaseosa pequena";
                }
            }
            break;
        }
        default:
            cout<<"Opcion ingresada deconocida. "<<endl;
            break;
        }
        cout<<"Desea agregar otro elemento al combo "<<NombreCombo<<"? "<<endl;
        cout<<"Si -> 1.\nNo -> Calquier otra tecla. "<<endl;
        cin>>opcion;
    }while(opcion == '1');

    for(int i=contenido.size();i>=0;--i){
        if(contenido[i]==','){
            contenido=contenido.substr(0,i)+" y"+contenido.substr(i+1,contenido.size()-i)+".";
            break;
        }
    }
    char SegOpcion;
    ComboCinema auxiliar;
    auxiliar.InicializarCombo(NombreCombo,contenido,CostoCombo,1);
    cout<<"El combo resultante es: "<<endl;
    cout<<"Nombre: "<<auxiliar.Nombre<<endl;
    cout<<"Contenido: "<<auxiliar.Componentes<<endl;
    cout<<"Precio: "<<auxiliar.precio<<endl;
    cout<<endl;
    cout<<"Confirma el combo? "<<endl;
    cout<<"Si -> 1.\nNo -> Cualuqier otra tecla. "<<endl;
    cin>>SegOpcion;
    if(SegOpcion=='1')
        TodosLosCombos.insert(pair<string,ComboCinema>(NombreCombo,auxiliar));
    LeerProductosAgregarInventario.clear();
}

bool SiExisteLaComidaSuficienteEnElInventario(map<string,ProductoInvetario>& AlmacenarInventario, string IndiceProducto,int cantidad){
    if(AlmacenarInventario[IndiceProducto].NumeroDePaquetes*AlmacenarInventario[IndiceProducto].UnidadesPorPaquete >= cantidad)
        return true;
    else
        return false;
}

bool QueSeNecesitaParaCadaPreparacion(map<string,ProductoInvetario>& AlmacenarInventario,map<string,int> PreparacionCanidad,int Repetir){
    map<string,map<vector<int>,vector<string>>> ParaFormarUnCombo {{"Hamburguesa",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir},{"1","2","3","4"}}}},{"Perro Caliente",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*14,PreparacionCanidad.begin()->second*Repetir},{"5","6","7","8","3"}}}},{"Nachos",{{{PreparacionCanidad.begin()->second*Repetir},{"9"}}}},{"Doritos",{{{PreparacionCanidad.begin()->second*Repetir},{"10"}}}},{"Detodito",{{{PreparacionCanidad.begin()->second*Repetir},{"11"}}}},{"Caja para palomitas 170 Oz",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*170},{"16","12"}}}},{"Caja para palomitas 130 Oz",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*150},{"17","12"}}}},{"Caja para palomitas 85 Oz",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*130},{"16","12"}}}},{"Vaso Plastico 44 Oz",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*44,PreparacionCanidad.begin()->second*Repetir*44,PreparacionCanidad.begin()->second*Repetir*44,PreparacionCanidad.begin()->second*Repetir*44,PreparacionCanidad.begin()->second*Repetir*44},{"13","19","20","21","22","23"}}}},{"Vaso Plastico 32 Oz",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*32,PreparacionCanidad.begin()->second*Repetir*32,PreparacionCanidad.begin()->second*Repetir*32,PreparacionCanidad.begin()->second*Repetir*32,PreparacionCanidad.begin()->second*Repetir*32},{"14","19","20","21","22","23"}}}},{"Vaso Plastico 22 Oz",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*22,PreparacionCanidad.begin()->second*Repetir*22,PreparacionCanidad.begin()->second*Repetir*22,PreparacionCanidad.begin()->second*Repetir*22,PreparacionCanidad.begin()->second*Repetir*22},{"15","19","20","21","22","23"}}}}};
    bool indicador=true;
    auto i= ParaFormarUnCombo.find(PreparacionCanidad.begin()->first);
    for (int verificador=0;verificador<i->second.begin()->first.size();verificador++) {
        indicador=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,i->second.begin()->second[verificador],i->second.begin()->first[verificador]);
        if(indicador==false)
            return false;
    }
    return true;

}
map<string,int> DesglozarElementosDelContenidoDeUnCombo(string & contenido){
    map<string,int> AuxiliarProductoCantidad;
    string ayuda;
    int numero;
    int i=contenido.find(',');
    if(i=-1)
        i=contenido.find('.');
    ayuda=contenido.substr(0,i);
    contenido=contenido.substr(i+1,contenido.size()-i);
    i=ayuda.find(' ');
    numero=StringAnumero(ayuda.substr(0,i));
    ayuda=contenido.substr(i+1,ayuda.size()-i);
    if(ayuda=="Hamburguesa" or ayuda == "Hamburguesas")
        AuxiliarProductoCantidad.insert(pair<string,int>("Hamburguesa",numero));
    else if(ayuda=="Perro caliente" or ayuda == "Perros caliente")
        AuxiliarProductoCantidad.insert(pair<string,int>("Perro caliente",numero));
    else if(ayuda=="Paquete de nachos" or ayuda == "Paquetes de nachos")
        AuxiliarProductoCantidad.insert(pair<string,int>("Nachos",numero));
    else if(ayuda=="Paquete de doritos" or ayuda == "Paquetes de doritos")
        AuxiliarProductoCantidad.insert(pair<string,int>("Doritos",numero));
    else if(ayuda=="Paquete de detodito" or ayuda == "Paquete de detodito")
        AuxiliarProductoCantidad.insert(pair<string,int>("Detodito",numero));
    else if(ayuda=="Caja grande de palomitas" or ayuda == "Cajas grandes de palomitas")
        AuxiliarProductoCantidad.insert(pair<string,int>("Caja de palomitas 170 Oz",numero));
    else if(ayuda=="Caja mediana de palomitas" or ayuda == "Cajas medianas de palomitas")
        AuxiliarProductoCantidad.insert(pair<string,int>("Caja de palomitas 130 Oz",numero));
    else if(ayuda=="Caja pequena de palomitas" or ayuda == "Cajas pequenas de palomitas")
        AuxiliarProductoCantidad.insert(pair<string,int>("Caja de palomitas 85 Oz",numero));
    else if(ayuda=="Vaso de gaseosa grande" or ayuda == "Vasos de gaseosa grande")
        AuxiliarProductoCantidad.insert(pair<string,int>("Vasos plasticos 44 Oz",numero));
    else if(ayuda=="Vaso de gaseosa mediana" or ayuda == "Vasos de gaseosa mediana")
        AuxiliarProductoCantidad.insert(pair<string,int>("Vasos plasticos 32 Oz",numero));
    else if(ayuda=="Vaso de gaseosa pequena" or ayuda == "Vasos de gaseosa pequena")
        AuxiliarProductoCantidad.insert(pair<string,int>("Vasos plasticos 22 Oz",numero));
    return AuxiliarProductoCantidad;

}
void FormarCombos(map<string,ProductoInvetario>& AlmacenarInventario, map<string,ComboCinema>& TodosLosCombos){
    char opcion;
    string NombreComboRecargar;
    getline(cin,NombreComboRecargar);
    do{
        opcion=SubMenuFormarCombo();
        switch (opcion) {
        case '1':{
            MostrarCombos(TodosLosCombos);
        }
            break;
        case '2':{
            cout<<"Ingrese el nombre del combo que desea recargar: "<<endl;
            getline(cin,NombreComboRecargar);
            if(TodosLosCombos.find(NombreComboRecargar)==TodosLosCombos.end())
                cout<<"Imposible recargar un combo que no existe. "<<endl;
            else {
                int numero;
                cout<<"Cuantos combos nuevos desea crear? "<<endl;
                cin>>numero;
            }
           break;
        }
        case '3':
            cout<<"Hola"<<endl;
            break;
        case '4':
            cout<<"Hola<"<<endl;
            break;
        case '5':
            cout<<"Los cambios han sido guardados correctamente. "<<endl;
            break;
        }
    }while(opcion != '5');
}


void LeerInventario( map<string,ProductoInvetario>& AlmacenarInventario){
string LeerLinea,nombre,UnidadesPorPaquete,NumeroDePaquetes,PrecioTotal,indice;
    fstream archivo("inventario.txt");
    if(archivo.fail())
        cout<<"Error al abrir el archivo, formar combos..."<<endl;
    else {
        while (!archivo.eof()) {
            getline(archivo,LeerLinea);
            if(LeerLinea!=""){
                ProductoInvetario auxiliar;
                indice=LeerLinea.substr(0,LeerLinea.find("+"));
                LeerLinea=LeerLinea.substr(LeerLinea.find("+")+1,LeerLinea.size()-LeerLinea.find("+"));
                auxiliar.nombre=LeerLinea.substr(0,LeerLinea.find("+"));
                LeerLinea=LeerLinea.substr(LeerLinea.find("+")+1,LeerLinea.size()-LeerLinea.find("+"));
                UnidadesPorPaquete=LeerLinea.substr(0,LeerLinea.find("+"));
                auxiliar.UnidadesPorPaquete=StringAnumero(UnidadesPorPaquete);
                LeerLinea=LeerLinea.substr(LeerLinea.find("+")+1,LeerLinea.size()-LeerLinea.find("+"));
               NumeroDePaquetes=LeerLinea.substr(0,LeerLinea.find("+"));
               auxiliar.NumeroDePaquetes=StringAFloat(NumeroDePaquetes);
               LeerLinea=LeerLinea.substr(LeerLinea.find("+")+1,LeerLinea.size()-LeerLinea.find("+"));
               PrecioTotal=LeerLinea.substr(0,LeerLinea.find("+"));
               AlmacenarInventario.insert(pair<string,ProductoInvetario>(indice,auxiliar));
            }
        }
    }
    archivo.close();
}

float StringAFloat(string numero){
    float retornar=0;
    for (int i=numero.size()-1;i>=0;--i) {
        if(numero[i]=='.'){
            while (retornar>1) {
                retornar/=10;
            }
        }
        else {
            if(retornar>=1)
                retornar+=10*(numero[i]-48);
            else
               retornar+=numero[i]-48;
        }
    }

    return retornar;
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

void MostrarCombos( map<string,ComboCinema>& CombosEnEsteMomento){ //Imprime en pantalla los combos que hay dispobiles.
    if(!CombosEnEsteMomento.empty()){
        cout<<"Los combos disponibles en el momento son: "<<endl<<endl;
        for(auto i:CombosEnEsteMomento){
            cout<<"Nombre: "<<i.first<<endl;
            cout<<"Contenido: "<<i.second.Componentes<<endl;
            cout<<"Precio: "<<i.second.precio<<endl;
            cout<<"Disponibilidad: "<<i.second.CuantosDeEsteHay<<endl;
            cout<<endl<<endl;
        }
    }
    else {
        cout<<"En el momento no hay combos disponibles. "<<endl;
    }

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

void ObservarInventario(map<string,ProductoInvetario>& AlmacenarInventario){ //Esta funcion imprime en pantalla el inventario.
    if(!AlmacenarInventario.empty()){
        for(auto i:AlmacenarInventario){
            cout<<"Nombre: "<<i.first<<endl;
            cout<<"Unidades por paquete: "<<i.second.UnidadesPorPaquete<<endl;
            cout<<"Numero de paquetes "<<i.second.NumeroDePaquetes<<endl;;
            cout<<endl<<endl;
        }
    }
    else {
        cout<<"Por el momento el inventario se encuentra vacio. "<<endl;
    }
}

void LeerArticulosParaAgregarInventario(map<string,ProductoInvetario>& LeerProductosAgregarInventario){
   //Esta funcion lee los productos que yo puedo agregar en el inventario.
    fstream archivo("ProductosParaAgregarInventario.txt");
   string LeerLinea,indice,nombre,unidadesPorPaquete,precioProducto;
   ProductoInvetario auxilar;
   if(archivo.fail()){
       cout<<"Error al abrir el archivo: "<<endl;
   }
   else {
       getline(archivo,LeerLinea);
       while (!archivo.eof()) {
           if(LeerLinea != ""){
               indice=LeerLinea.substr(0,LeerLinea.find('+'));
               LeerLinea=LeerLinea.substr(LeerLinea.find('+')+1,LeerLinea.size()-LeerLinea.find('+'));
                auxilar.nombre=LeerLinea.substr(0,LeerLinea.find('+'));
               LeerLinea=LeerLinea.substr(LeerLinea.find('+')+1,LeerLinea.size()-LeerLinea.find('+'));
               auxilar.UnidadesPorPaquete=StringAnumero(LeerLinea.substr(0,LeerLinea.find('+')));
               LeerLinea=LeerLinea.substr(LeerLinea.find('+')+1,LeerLinea.size()-LeerLinea.find('+'));
               precioProducto=StringAnumero(LeerLinea.substr(0,LeerLinea.find('+')));
               auxilar.NumeroDePaquetes=1.0;
               LeerProductosAgregarInventario.insert(pair<string,ProductoInvetario>(indice,auxilar));
           }
       }
   }
    archivo.close();
}

void AgregarAInventario(map<string,ProductoInvetario>& AlmacenarInventario,int &inversion){
    map<string,ProductoInvetario> LeerProductosAgregarInventario;
    LeerArticulosParaAgregarInventario(LeerProductosAgregarInventario);
    cout<<"Los productos que la administracion puede comprar son los siguientes: "<<endl;
    cout<<endl<<endl;
    cout<<"   Nombre                       U/P  Precio"<<endl; // Muesta el nombre del producto, las unidades por paquete y el precio de cada paquete.
    for (auto i:LeerProductosAgregarInventario) {
        cout<<i.first<<") "<<i.second.nombre<<" "<<i.second.UnidadesPorPaquete<<" "<<i.second.precio<<endl;
    }
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%d-%m-%Y %H-%M-%S");
    auto str = oss.str();
    EscribirRegistroDeComprasParaInventario(str);
    char opcion;
    do{
        cout<<"Desea agregar un producto al inventario? "<<endl;
        cout<<"Si -> 1\nNo -> Cualquier otra tecla. "<<endl;
        if(opcion=='1'){
            string reporte;
            string indice;
            int cantidad;
            cout<<"Ingrese el indice del producto que la administracion desea comprar: "<<endl;
            cin>>indice;
            cout<<"Ingrese cuantos de estos paquetes desea comprar: "<<endl;
            cin>>cantidad;
            AlmacenarInventario[indice].NumeroDePaquetes+=LeerProductosAgregarInventario[indice].NumeroDePaquetes*cantidad;
            inversion+=LeerProductosAgregarInventario[indice].precio*cantidad;
            reporte=LeerProductosAgregarInventario[indice].nombre+" "+NumeroCadenaCaracterer(LeerProductosAgregarInventario[indice].UnidadesPorPaquete)+NumeroCadenaCaracterer(LeerProductosAgregarInventario[indice].NumeroDePaquetes*cantidad);
            EscribirRegistroDeComprasParaInventario(reporte);
        }
    }while(opcion == '1');
    LeerProductosAgregarInventario.clear();
}

void EscribirRegistroDeComprasParaInventario(string reporte){
    ofstream archivo("RegistroDeComprasInventario.txt",ios::app);
    if(archivo.fail()){
        cout<<"Error al abrir el archivo.EscribirRegistroDeComprasParaInventario "<<endl;
    }
    else {
        archivo<<reporte<<endl;
    }
    archivo.close();
}


string NumeroCadenaCaracterer(int canDinero){// Transforma un entero en una cadena de caracteres.
    string RetornarConvertido="";
    char letra;
    if(canDinero==0){
        letra=(canDinero%10+48);
        RetornarConvertido= letra + RetornarConvertido;
    }
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
