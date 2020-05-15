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
    cout<<"F) Solicitudes de registro. "<<endl;
    cout<<"G) Generar reporte de las ventas hasta el momento. "<<endl;
    cout<<"H) Cerrar Sesion. "<<endl;
    cin>>opcion;
    return opcion;
}

char MenuGeneal(){
    char opcion;
    cout<<"A) Iniciar sesion como administrador. "<<endl;
    cout<<"B) Iniciar sesion como usuario. "<<endl;
    cout<<"C) Enviar solucitud de registro. "<<endl;
    cout<<"D) Realizar compra anonima. "<<endl;
    cout<<"E) Salir. "<<endl;
    cin>>opcion;
    return opcion;
}

char MenuUsuario(){
    char opcion;
    cout<<"A) Ver productos disponibles. "<<endl;
    cout<<"B) Comprar producto. "<<endl;
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
    return opcion;
}

void ObservarProductos(map<string,ComboCinema> &TodosLosCombos){
    //Esta funcion imprime los diferentes articulos que hay disponibles.
    vector<string> ParaFormarUnCombo{"Hamburguesa","Perro Caliente","Nachos","Doritos","Detodito","Caja grande de palomitas.","Caja mediana de palomitas.","Caja pequena de palomitas","Vaso grande de gaseosa.","Vaso mediano de gaseosa.","Vaso pequeno de gaseosa."};
    cout<<"Combos: "<<endl;
    if(TodosLosCombos.empty())
        cout<<"No hay combos disponibles."<<endl;
    else {
        MostrarCombos(TodosLosCombos);
    }
    cout<<"Articulos independientes: "<<endl;
    for (int i=0;i<ParaFormarUnCombo.size();i++) {
        cout<<i+1<<") "<<ParaFormarUnCombo[i]<<endl;
    }
    cout<<endl;
}

void RealizarCompra(map<string,Usuario>& UsuariosRegistrados,map<string,ComboCinema> &TodosLosCombos,map<string,ProductoInvetario> & AlmacenarInventario,string usuarioOriginal){
    //Permite que el usuario compre articulos.
    string usuario=usuarioOriginal;
    char ComboOIndependiente;
    int cantidad;
    char CanastaDeCompras;
    int DineroAPagar=0;
    string NombrePedido;
    auto t = std::time(nullptr); // Obtiene la fecha y la hora.
    auto tm = *std::localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%d-%m-%Y %H-%M-%S");
    auto str = oss.str();
    str= "Fecha: "+ str.substr(0,str.find(" "))+" Hora: "+ str.substr(str.find(" ")+1,str.size()-str.find(" "));
    EscribirLineaRegistroVentas(str);
    cout<<"Por favor sea cuidadoso al ingresar los siguientes datos. "<<endl; // Pide al usuario que ingrese el puesto en el cine.
    cout<<"Ingrese la fila y el asiento a la que se debe llevar el pedido: "<<endl;
    do{
        cout<<"Fila (A - Z): "<<endl;
        cin>>NombrePedido;
        if(NombrePedido[0]<65 or NombrePedido[0]>90)
            cout<<"Fila fuera de rango. "<<endl;  //Verifica que la fila y asienta enten dentro del rango establecido.
    }while(NombrePedido[0]<65 or NombrePedido[0]>90);
    NombrePedido=NombrePedido[0];
    usuario+=" .Fila: "+NombrePedido;
    int auxiliar;
    do{
        cout<<"Asiento (1-15): "<<endl;
        cin>>NombrePedido;
        auxiliar=StringAnumero(NombrePedido);
        if(auxiliar<1 or auxiliar > 15)
            cout<<"Asiento fuera de rango. "<<endl;
    }while(auxiliar<1 or auxiliar > 15);
    usuario+=" .Asiento: "+NombrePedido;
    EscribirLineaRegistroVentas("Usuario: "+usuario);
    do{
        ObservarProductos(TodosLosCombos);
        do{
            cout<<"Que producto desea comprar? "<<endl;
            cout<<"Combo -> 1.\nIndependiente->2"<<endl;
            cin>>ComboOIndependiente;
            if(ComboOIndependiente!= '1' and ComboOIndependiente != '2')
                cout<<"Opcion desconocida. "<<endl;
        }while(ComboOIndependiente!= '1' and ComboOIndependiente != '2');

        if(ComboOIndependiente=='1'){
            cout<<"Ingrese el nombre del combo: "<<endl;
            getline(cin,NombrePedido);
            getline(cin,NombrePedido);
            cout<<"Ingrese cuantos de este combo desea: "<<endl;
            cin>>cantidad;
            if(TodosLosCombos.find(NombrePedido)==TodosLosCombos.end())
                cout<<"El combo ingresado NO se encuentra en nuestro repertorio.\nIntentelo nuevamente."<<endl;
            else {
                if(TodosLosCombos[NombrePedido].CuantosDeEsteHay>=cantidad){
                    for (int i=1;i<=cantidad;i++) {
                        SeleccionarSaborGaseosa(AlmacenarInventario,DineroAPagar,TodosLosCombos[NombrePedido].Componentes); // Permite realizar la seleccion de gaseosa.
                    }
                    TodosLosCombos[NombrePedido].CuantosDeEsteHay-=cantidad;
                    EscribirLineaRegistroVentas(NombrePedido+" "+NumeroCadenaCaracterer(cantidad)); //Registra el producto vendido en el archivo de registro de ventas.
                    DineroAPagar+=TodosLosCombos[NombrePedido].precio*cantidad; //Aumenta el costo total.
                    UsuariosRegistrados[usuarioOriginal].puntos+=TodosLosCombos[NombrePedido].precio*0.05; // Sistema de puntos.
                }   // Los puntos que va a ganar un usuario por compra de combo es una francion del precion del mismo.
                else {
                    cout<<"No hay la cantidad de combos suficientes para satisfacer el pedido."<<endl;
                }
            }
        }
        else {
            //Los diferentes platos disponibles en el cine.
            vector<string> ParaFormarUnCombo{"Hamburguesa","Perro caliente","Paquete de nachos","Paquete de doritos","Paquete de detodito","Caja grande de palomitas","Caja mediana de palomitas","Caja pequena de palomitas","Vaso de gaseosa grande","Vaso de gaseosa mediana","Vaso de gaseosa pequena"};
            map <string,int> ProductoCantidad;
            bool bandera;
            string opcion;
            do{
                cout<<"Ingrese el producto que desea comprar: "<<endl;
                cin>>opcion;
                if(opcion != "1" and opcion != "2" and opcion != "3" and opcion != "4" and opcion != "5" and opcion != "6" and opcion != "7" and opcion != "8" and opcion != "9" and opcion != "10" and opcion != "11" )
                    cout<<"Opcion ingresada invalida. "<<endl;
            }while(opcion != "1" and opcion != "2" and opcion != "3" and opcion != "4" and opcion != "5" and opcion != "6" and opcion != "7" and opcion != "8" and opcion != "9" and opcion != "10" and opcion != "11" );
            int opcionNumero=StringAnumero(opcion);
            int numero;
            cout<<"Cuantas unidades desea agregar? "<<endl;
            cin>>numero;
            //Analiza si hay los suministros suficientes para formar el plato ingresado.
            string AnalizarContenidoCombo=NumeroCadenaCaracterer(numero)+" "+ParaFormarUnCombo[opcionNumero-1]+",";
            while(AnalizarContenidoCombo != ""){
                ProductoCantidad=DesglozarElementosDelContenidoDeUnCombo(AnalizarContenidoCombo);
                bandera=QueSeNecesitaParaCadaPreparacion(AlmacenarInventario,ProductoCantidad,numero);
                if(bandera == false){
                    cout<<"Senor usuario lamentamos las molestias, pero en el momento no hay inventario suficiente para satisfacer este pedido. Pruebe con otro articulo."<<endl;
                    break;
                }
            }
            if(bandera==true){
                //De confirmarce que si hay la cantidad de inventario suficiente para preparar e platillo estas las siguientes lineas de codigo retan
                //esa cantidad.
                AnalizarContenidoCombo=NumeroCadenaCaracterer(numero)+" "+ParaFormarUnCombo[opcionNumero-1]+",";
                while (AnalizarContenidoCombo!= "") {
                    ProductoCantidad=DesglozarElementosDelContenidoDeUnCombo(AnalizarContenidoCombo);
                    EjecutarGastoAutomatico(AlmacenarInventario,ProductoCantidad,numero);
                }
                precios_individuales(DineroAPagar,opcionNumero);
                EscribirLineaRegistroVentas(ParaFormarUnCombo[opcionNumero-1]+" "+NumeroCadenaCaracterer(numero));
            }
        }
        cout<<"Desea agregar otro articulo al carrito? "<<endl;
        cout<<"Si -> 1\nNo -> Cualquier otra tecla. "<<endl;
        cin>>CanastaDeCompras;
    }while(CanastaDeCompras == '1');
    if(UsuariosRegistrados[usuarioOriginal].puntos>=5000){
        DineroAPagar=DineroAPagar*0.6;
        UsuariosRegistrados[usuarioOriginal].puntos-=5000;
    }
    EscribirLineaRegistroVentas("Precio Total: "+NumeroCadenaCaracterer(DineroAPagar));
    EscribirLineaRegistroVentas("        ");
    PagarCuenta(DineroAPagar);
    EscribirArchivoInventario(AlmacenarInventario);
}

void PagarCuenta(int PrecioTotal){
    int dineroIngresado;
    bool bandera;
    do{
        cout<<"Su compra tiene un precio de "<<PrecioTotal<<" pesos."<<endl;
        cout<<"Ingrese la cantidad con la que va a pagar: "<<endl;
        cin>>dineroIngresado;
        if(dineroIngresado<PrecioTotal){
            cout<<"La cantidad de dinero ingresada es insuficiente. "<<endl;
            bandera=false;
        }
        else {
            dineroIngresado-=PrecioTotal;
            cout<<"Su cambio es: "<<endl;
            cambio_diniero(&dineroIngresado);
            cout<<"Con un resto de "<<dineroIngresado<<endl;
            bandera=true;
        }
    }while(bandera==false);
}

void cambio_diniero(int *dinero){
    cout<<"Billetes de 50 000: "<<*dinero/50000<<endl;
    *dinero=*dinero%50000;
    cout<<"Billetes de 20 000: "<<*dinero/20000<<endl;
    *dinero=*dinero%20000;
    cout<<"Billetes de 10 000: "<<*dinero/10000<<endl;
    *dinero=*dinero%10000;
    cout<<"Billetes de 5000: "<<*dinero/5000<<endl;
    *dinero=*dinero%5000;
    cout<<"Billetes de 2000: "<<*dinero/2000<<endl;
    *dinero=*dinero%2000;
    cout<<"Billetes de 1000: "<<*dinero/1000<<endl;
    *dinero=*dinero%1000;
    cout<<"Monedas de 500: "<<*dinero/500<<endl;
    *dinero=*dinero%500;
    cout<<"Monedas de 200: "<<*dinero/500<<endl;
    *dinero=*dinero%200;
    cout<<"Monedas de 100: "<<*dinero/500<<endl;
    *dinero=*dinero%100;
    cout<<"Monedas de 50: "<<*dinero/500<<endl;
    *dinero=*dinero%50;
}
void EscribirLineaRegistroVentas(string LineaEscribir){
    ofstream archivo("RegistroDeVentas.txt",ios::app);
    if(archivo.fail())
        cout<<"Error al abrir el archivo. EscribirLineaRegistroVentas. "<<endl;
    else {
        archivo<<LineaEscribir<<endl;
    }
    archivo.close();
}

void ImprimirRegistroVentas(){
    ifstream archivo("RegistroDeVentas.txt");
    string LeerLinea;
    if (archivo.fail()) {
        cout<<"Error al abrir el archivo. "<<endl;
    }
    else {
        while (!archivo.eof()) {
            getline(archivo,LeerLinea);
            if(LeerLinea!="")
                cout<<LeerLinea<<endl;
        }
    }
    archivo.close();

}
void precios_individuales( int &PrecioApagar,int opcion){
    //Esta funcion contiene los precios de las los productos individuales.
    switch (opcion) {
    case 1:{
        PrecioApagar+=7000;
        break;
    }
    case 2:{
        PrecioApagar+=6500;
        break;
    }
    case 3:{
        PrecioApagar+=3000;
        break;
    }
    case 4:{
        PrecioApagar+=4000;
        break;
    }
    case 5:{
        PrecioApagar+=4500;
        break;
    }
    case 6:{
        PrecioApagar+=5000;
        break;
    }
    case 7:{
        PrecioApagar+=4500;
        break;
    }
    case 8:{
        PrecioApagar+=4000;
        break;
    }
    case 9:{
        PrecioApagar+=7000;
        break;
    }
    case 10:{
        PrecioApagar+=6200;
        break;
    }
    case 11:{
        PrecioApagar+=5400;
        break;
    }
    }
}
void SeleccionarSaborGaseosa(map<string,ProductoInvetario> & AlmacenarInventario, int & PrecioAPagar, string AnalizarContenidoCombo){
    map<string,int> ProductoCantidad;
    vector <string> Gaseosas{"Gaseosa Pepsi","Cocacola","Gaseosa Postobon Manzana","Gaseosa Postobon Uva","Dry Ginger"};
    map<string,ProductoInvetario> LeerProductosAgregarInventario;
    LeerArticulosParaAgregarInventario(LeerProductosAgregarInventario);
    bool bandera=true;
    int numeroOnzas;
    string indice;
    while (AnalizarContenidoCombo != "") {
        ProductoCantidad=DesglozarElementosDelContenidoDeUnCombo(AnalizarContenidoCombo);
        if(ProductoCantidad.begin()->first=="Vasos Plasticos 44 Oz" or ProductoCantidad.begin()->first=="Vasos Plasticos 32 Oz" or ProductoCantidad.begin()->first=="Vasos Plasticos 22 Oz"){
            if(ProductoCantidad.begin()->first=="Vasos Plasticos 44 Oz")
                numeroOnzas=44;
            else if (ProductoCantidad.begin()->first=="Vasos Plasticos 32 Oz")
                numeroOnzas=32;
            else
                numeroOnzas=22;
            for (int iterador=0;iterador<Gaseosas.size();iterador++) {
                cout<<iterador+1<<") "<<Gaseosas[iterador]<<endl;
            }
            for (int auxiliar=1;auxiliar<=ProductoCantidad.begin()->second;auxiliar++) {
                do{
                    char sabor;
                    do{
                        cout<<"De que sabor desea la gaseosa "<<auxiliar<<": "<<endl;
                        cin>>sabor;
                        if(sabor != '1' and sabor != '2' and sabor != '3' and sabor !='4')
                            cout<<"Opcion desconocida. "<<endl;
                    }while(sabor != '1' and sabor != '2' and sabor != '3' and sabor !='4');
                    switch (sabor) {
                    case'1':{
                        indice="19";
                        break;
                    }
                    case'2':{
                        indice="20";
                        break;
                    }
                    case'3':{
                        indice="21";
                        break;
                    }
                    case'4':{
                        indice="22";
                        break;
                    }
                    case'5':{
                        indice="23";
                        break;
                    }
                    }
                    if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,indice,numeroOnzas)){
                        PrecioAPagar+=LeerProductosAgregarInventario[indice].precio/(LeerProductosAgregarInventario[indice].NumeroDePaquetes*LeerProductosAgregarInventario[indice].UnidadesPorPaquete)*numeroOnzas;
                        GastarRecursosInventario(AlmacenarInventario,indice,numeroOnzas);
                    }
                    else {
                        int ads=sabor-49;
                        cout<<"No tenemos suficiente "<<Gaseosas[ads]<<" para satisfacer tu pedido. "<<endl;
                        cout<<"Prueba otro sabor. "<<endl;
                    }
                }while(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,indice,numeroOnzas)==false);
            }
        }

    }
    LeerProductosAgregarInventario.clear();
}

bool Administrador(){
    //Verifica si la clave ingresada el administrador es la correcta o no.
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
    //Esta funcion lee la informacion del archivo usuarios.txt y la lleva a la variable UsuariosRegistrados
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
    //La informacion de la variable UsuariosRegistrados la lleva al archivo.
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
    //Elimina un usuario del sistema.
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

void ExtraerSolicitudesDeRegistro(map<string,string> & PeticionesPendientes){
    //Lee las solicitudes que esten en el archivo SolicitudesDeRegistro.txt
    // Y la  lleva a la variable Peticiones pendientes.
    fstream archivo("SolicitudesDeRegistro.txt");
    string linea;
    if(archivo.fail())
        cout<<"Error al abrir el archivo. ExtraerSolicitudesDeRegistro"<<endl;
    else {
        while (!archivo.eof()) {
            getline(archivo,linea);
            if(linea!="")
                PeticionesPendientes.insert(pair<string,string>(linea.substr(0,linea.find('+')),linea.substr(linea.find('+')+1,linea.size()-linea.find('+'))));
        }
    }
    archivo.close();
}

void ObservarPeticiones(map<string,string> & PeticionesPendientes){
    //Imprime en pantallas las diferentes peticiones pendientes.
    for (auto i: PeticionesPendientes) {
        cout<<"Usuario: "<<i.first<<" Contrasena: "<<i.second<<endl;
    }
}
bool verificando_cedula(string documentoING,map<string,Usuario> &UsuariosRegistrados){
    //Verifica si el nombre de usuario registrado se encuentra o no en uso.
    auto i=UsuariosRegistrados.find(documentoING);
    if(i!=UsuariosRegistrados.end()){
        return false;
    }
    else {
        return true;
    }
}


bool verificando_clave(string contrasena){
    //Vaerifica si la clave ingresada cumple con las condiciones pecificadas.
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
    //Crea un objeto usuario y lo agrega a la variable Usuarios registrados.
    //Que es la variable que contiene todos los usuarios del sistema.
    Usuario auxilar;
    auxilar.inicializandoTodo(nombre,contrasena,puntos);
    UsuariosRegistrados.insert(pair<string,Usuario>(nombre,auxilar));
}

void AgregarUsuarios( map<string,Usuario> &UsuariosRegistrados,map<string,string> & PeticionesPendientes){
    //Funcion en la que el administrador va a aceptar las peticiones de registro.
    ObservarPeticiones(PeticionesPendientes);
    string documento,clave;
    getline(cin,documento);
    do{
        cout<<"Ingrese el nombre con el cual el usuario desea quedar registrado: "<<endl;
        getline(cin,documento);
        //Verifica si el nombre ingresado está dentro de las peticiones.
        if(PeticionesPendientes.find(documento)==PeticionesPendientes.end())
            cout<<"Lo setimos pero este no es un nombre que este dentro de las peticiones. "<<endl;
    }while(PeticionesPendientes.find(documento)==PeticionesPendientes.end());
    auto i= PeticionesPendientes.find(documento);
    do{
        cout<<"La clave del usuario debe ser de exactamente 8 caracteres; ademas, debe contener numeros y letras. "<<endl;
        cout<<"Ingrese la clave del usuario: "<<endl;
        cin>>clave;
        if(i->second!=clave)
            cout<<"Este clave no corresponde con la peticion. "<<endl;
    }while(i->second!=clave); //Verifica si la clave coincide con la la peticion.

    IncorporarUsuarioAlSistema(UsuariosRegistrados,documento,clave,"0"); //Agrega al nuevo usuario a la variable usuarios registrados.
    PeticionesPendientes.erase(documento); //Elimina la peticion.
    cout<<"El usuario se ha agregado exitosamente. "<<endl;
}

void RealizarPeticion( map<string,Usuario> &UsuariosRegistrados){
    //Permite a un usuario anonimo realizar una peticion.
    string documento,clave;
    getline(cin,documento);
    do{
        cout<<"Ingrese el nombre con el cual el desea quedar registrado: "<<endl;
        getline(cin,documento);
        //Verifica si el nombre ingresado está entre las peticiones realizadas.
        if(verificando_cedula(documento,UsuariosRegistrados)==false or HayOtraPeticionConElMismoNombre(documento)==false)
            cout<<"Lo sentimos pero este nombre ya se encuentra en uso. "<<endl;
    }while(verificando_cedula(documento,UsuariosRegistrados)==false or HayOtraPeticionConElMismoNombre(documento)==false);

    do{
        cout<<"La clave del usuario debe ser de exactamente 8 caracteres; ademas, debe contener numeros y letras. "<<endl;
        cout<<"Ingrese la clave del usuario: "<<endl;
        cin>>clave;
    }while(!verificando_clave(clave));

    NotificarPeticionDeRegistro(documento,clave); //Escribe en el archivo de peticiones la peticion relalizada.
    cout<<"Peticion enviada exitosamente. "<<endl;
}

bool HayOtraPeticionConElMismoNombre(string CandidatoNombre){
    //Esta funcion se encarga de verificar que dos peticiones NO tengan el mismo nombre.
    int indicador=contar_lineas("SolicitudesDeRegistro.txt");
    if(indicador==0)
        return true;
    else {
        fstream archivo("SolicitudesDeRegistro.txt");
        string linea;
        if(archivo.fail())
            cout<<"Error al abrir el archivo.  HayOtraPeticionConElMismoNombre"<<endl;
        else {
            while (!archivo.eof()) {
                getline(archivo,linea);
                if(linea.find(CandidatoNombre)!=-1){
                    return false;
                }
            }
        }
        return true;
        archivo.close();
    }
}
void NotificarPeticionDeRegistro(string nombre, string contrasena){
    //Actualiza el archivo de que contiene las peticiones de registro.
    ofstream archivo("SolicitudesDeRegistro.txt",ios::app);
    if(archivo.fail()){
        cout<<"Error al abrir el archivo. NotificarPeticionDeRegistro. "<<endl;
    }else {
        archivo<<nombre+"+"+contrasena<<endl;
    }
    archivo.close();
}
void GastarRecursosInventario(map<string,ProductoInvetario>& AlmacenarInventario,string PosicionProductoUsado, int UnidadesUsadas){
    auto i = AlmacenarInventario.find( PosicionProductoUsado);
    i->second.NumeroDePaquetes= (((i->second.UnidadesPorPaquete)*(i->second.NumeroDePaquetes))-UnidadesUsadas)*(i->second.NumeroDePaquetes)/((i->second.UnidadesPorPaquete)*(i->second.NumeroDePaquetes));
}
void CrearNuevoCombo(map<string,ProductoInvetario>& AlmacenarInventario,string NombreCombo,map<string,ComboCinema> &TodosLosCombos){
   //Esta funcion permite la creacion de un nuevo combo.
    //Los siguientes son las diferentes comidas que pueden hay disponibles en el cine.
    vector<string> ParaFormarUnCombo{"Hamburguesa","Perro Caliente","Nachos","Doritos","Detodito","Caja de palomitas 170 Oz","Caja de palomitas 130 Oz","Caja de palomitas 85 Oz","Vaso Plastico 44 Oz","Vaso Plastico 32 Oz","Vaso Plastico 22 Oz"};
   map<string,ProductoInvetario> LeerProductosAgregarInventario;
   LeerArticulosParaAgregarInventario(LeerProductosAgregarInventario);
   string contenido="";
   int CantidadComida;
   int CostoCombo=0;
   int elemento;
   char opcion;
   bool bandera,perfeccionista=true;
    do{
       for(int i=0;i<ParaFormarUnCombo.size();i++){
           cout<<i+1<<") "<<ParaFormarUnCombo[i]<<endl;
       }

        cout<<"Ingrese la posicion de la preparacion que va a incluir el combo: "<<endl;
        cin>>elemento;
        cout<<"Ingrese el numero de estas preparaciones que tendra el combo: "<<endl;
        cin>>CantidadComida;
        switch (elemento) {
           case 1:{
            for (int i =1;i<=4;i++) {
                bandera=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,NumeroCadenaCaracterer(i),1*CantidadComida);
                if(bandera==false){
                    perfeccionista=false;
                    break;
                }
            }
            if(bandera != false){
                for (int i= 1;i<=4;i++) {
                    GastarRecursosInventario(AlmacenarInventario,NumeroCadenaCaracterer(i),1*CantidadComida);
                    CostoCombo+=(LeerProductosAgregarInventario[NumeroCadenaCaracterer(i)].precio/(LeerProductosAgregarInventario[NumeroCadenaCaracterer(i)].NumeroDePaquetes*LeerProductosAgregarInventario[NumeroCadenaCaracterer(i)].UnidadesPorPaquete))*CantidadComida;
                }
                if(opcion == '1' and perfeccionista==true)
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Hamburguesa";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Hamburguesas";
                }
                perfeccionista=true;
            }else {
              cout<<"No hay suministros suficientes. Por favor, recargar el inventario. "<<endl;
              perfeccionista=false;
            }
        break;
        }
        case 2:{
            for(int i=5;i<=8;i++){
                if(i != 7){
                    bandera=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,NumeroCadenaCaracterer(i),1*CantidadComida);
                    if(bandera==false){
                        cout<<"No hay suministros suficientes..."<<endl;
                        perfeccionista=false;
                        break;
                    }
                }
                else{
                    bandera=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,NumeroCadenaCaracterer(i),14*CantidadComida);
                    if(bandera==false){
                        cout<<"No hay suministros suficientes..."<<endl;
                        perfeccionista=false;
                        break;
                    }
               }
            }
            if(bandera == true){
                bandera=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"3",1*CantidadComida);
                if(bandera==true){
                    for(int i=5;i<=8;i++){
                        if(i != 7){
                             GastarRecursosInventario(AlmacenarInventario,NumeroCadenaCaracterer(i),1*CantidadComida);
                             CostoCombo+=(LeerProductosAgregarInventario[NumeroCadenaCaracterer(i)].precio/(LeerProductosAgregarInventario[NumeroCadenaCaracterer(i)].NumeroDePaquetes*LeerProductosAgregarInventario[NumeroCadenaCaracterer(i)].UnidadesPorPaquete))*CantidadComida;
                        }
                        else{
                            GastarRecursosInventario(AlmacenarInventario,NumeroCadenaCaracterer(i),14*CantidadComida);
                            CostoCombo+=(LeerProductosAgregarInventario[NumeroCadenaCaracterer(i)].precio/(LeerProductosAgregarInventario[NumeroCadenaCaracterer(i)].NumeroDePaquetes*LeerProductosAgregarInventario[NumeroCadenaCaracterer(i)].UnidadesPorPaquete))*CantidadComida;
                            }
                    }
                    GastarRecursosInventario(AlmacenarInventario,"3",1*CantidadComida);
                    CostoCombo+=(LeerProductosAgregarInventario["3"].precio/(LeerProductosAgregarInventario["3"].NumeroDePaquetes*LeerProductosAgregarInventario["3"].UnidadesPorPaquete))*CantidadComida;
                    if(opcion == '1' and perfeccionista==true)
                        contenido+=",";
                    if(CantidadComida==1){
                        contenido+= NumeroCadenaCaracterer(CantidadComida)+" Perro caliente";
                    }
                    else {
                        contenido+= NumeroCadenaCaracterer(CantidadComida)+" Perros calientes";
                    }
                    perfeccionista=true;
                }
                else {
                    cout<<"No hay suministros suficientes..."<<endl;
                    perfeccionista=false;
                }
            }
            else {
                cout<<"No hay suministros suficientes..."<<endl;
                perfeccionista=false;
            }
            break;
        }
        case 3:{
            bandera=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"9",1*CantidadComida);
            if(bandera==true){
                GastarRecursosInventario(AlmacenarInventario,"9",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["9"].precio/(LeerProductosAgregarInventario["9"].NumeroDePaquetes*LeerProductosAgregarInventario["9"].UnidadesPorPaquete))*CantidadComida;
                if(opcion == '1' and perfeccionista==true)
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Paquete de nachos";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Paquetes de nachos";
                }
                perfeccionista=true;
            }
            else {
                cout<<"No hay suministros..."<<endl;
                cout<<"Deficit de paquetes de nachos..."<<endl;
                perfeccionista=false;
            }
            break;
        }
        case 4:{
            bandera=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"10",1*CantidadComida);
            if(bandera==true){
                GastarRecursosInventario(AlmacenarInventario,"10",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["10"].precio/(LeerProductosAgregarInventario["10"].NumeroDePaquetes*LeerProductosAgregarInventario["10"].UnidadesPorPaquete))*CantidadComida;
                if(opcion == '1' and perfeccionista == true)
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Paquete de doritos";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Paquetes de doritos";
                }
                perfeccionista=true;
            }
            else {
                cout<<"No hay suministros suficientes..."<<endl;
                cout<<"Deficit de paquetes de doritos.."<<endl;
                perfeccionista=false;
            }
            break;
        }
        case 5:{
            bandera=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"11",1*CantidadComida);
            if(bandera==true){
                GastarRecursosInventario(AlmacenarInventario,"11",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["11"].precio/(LeerProductosAgregarInventario["11"].NumeroDePaquetes*LeerProductosAgregarInventario["11"].UnidadesPorPaquete))*CantidadComida;
                if(opcion == '1' and perfeccionista==true)
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Paquete de detodito";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Paquetes de detodito";
                }
                perfeccionista=true;
            }
            else {
                cout<<"No hay suministros suficientes..."<<endl;
                cout<<"Deficit de paquetes de detoditos..."<<endl;
                perfeccionista=false;
            }
            break;
        }
        case 6:{
            if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"16",1*CantidadComida)==true and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"12",170*CantidadComida)==true){
                GastarRecursosInventario(AlmacenarInventario,"16",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["16"].precio/(LeerProductosAgregarInventario["16"].NumeroDePaquetes*LeerProductosAgregarInventario["16"].UnidadesPorPaquete))*CantidadComida;
                GastarRecursosInventario(AlmacenarInventario,"12",170*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["12"].precio/(LeerProductosAgregarInventario["12"].NumeroDePaquetes*LeerProductosAgregarInventario["12"].UnidadesPorPaquete))*CantidadComida*170;
                if(opcion == '1' and perfeccionista==true)
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Caja grande de palomitas";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Cajas grandes de palomitas";
                }
                perfeccionista=true;
            }
            else {
                cout<<"No hay suficientes suministros..."<<endl;
                cout<<"Se recomienda revisar el abastecimiento de crispetas y cajas grandes para crispetas. "<<endl;
                perfeccionista=false;
            }
            break;
        }
        case 7:{
            if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"17",1*CantidadComida)==true and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"12",150*CantidadComida)==true ){
                GastarRecursosInventario(AlmacenarInventario,"17",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["17"].precio/(LeerProductosAgregarInventario["17"].NumeroDePaquetes*LeerProductosAgregarInventario["17"].UnidadesPorPaquete))*CantidadComida;
                GastarRecursosInventario(AlmacenarInventario,"12",150*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["12"].precio/(LeerProductosAgregarInventario["12"].NumeroDePaquetes*LeerProductosAgregarInventario["12"].UnidadesPorPaquete))*CantidadComida*150;
                if(opcion == '1' and perfeccionista==true)
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Caja mediana de palomitas";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Cajas medianas de palomitas";
                }
                perfeccionista=true;
            }
            else {
                cout<<"No hay recursos suficientes..."<<endl;
                 cout<<"Se recomienda revisar el abastecimiento de crispetas y cajas medianas para crispetas. "<<endl;
                 perfeccionista=false;
            }
            break;
        }
        case 8:{
            if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"18",1*CantidadComida) == true and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"12",130*CantidadComida)==true){
                GastarRecursosInventario(AlmacenarInventario,"18",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["18"].precio/(LeerProductosAgregarInventario["18"].NumeroDePaquetes*LeerProductosAgregarInventario["18"].UnidadesPorPaquete))*CantidadComida;
                GastarRecursosInventario(AlmacenarInventario,"12",130*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["12"].precio/(LeerProductosAgregarInventario["12"].NumeroDePaquetes*LeerProductosAgregarInventario["12"].UnidadesPorPaquete))*CantidadComida*130;
                if(opcion == '1' and perfeccionista==true)
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Caja pequena de palomitas";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Cajas pequenas de palomitas";
                }
                perfeccionista=true;
            }
            else {
                cout<<"No hay suministros suficientes..."<<endl;
                cout<<"Se recomienda revisar el abastecimiento de crispetas y cajas pequenas para crispetas. "<<endl;
                perfeccionista=false;
            }
            break;
        }
        case 9:{
            if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"13",1*CantidadComida)==false){
                cout<<"No hay recursos suficientes..."<<endl;
                cout<<"Se recomienda revisar los vasos plasticos de 44 Oz..."<<endl;
                perfeccionista=false;
            }
            else if((SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"19",44*CantidadComida)==false and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"20",44*CantidadComida) and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"21",44*CantidadComida) and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"22",44*CantidadComida) and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"23",44*CantidadComida))){
                cout<<"No hay recursos suficientes..."<<endl;
                cout<<"Se recomienda revisar las gaseosas..."<<endl;
                perfeccionista=false;
            }
            else {
                GastarRecursosInventario(AlmacenarInventario,"13",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["13"].precio/(LeerProductosAgregarInventario["13"].NumeroDePaquetes*LeerProductosAgregarInventario["13"].UnidadesPorPaquete))*CantidadComida;
                if(opcion == '1' and perfeccionista==true)
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Vaso de gaseosa grande";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Vasos de gaseosa grande";
                }
                perfeccionista=true;
            }
            break;
        }
        case 10:{
            if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"14",1*CantidadComida)==false){
                cout<<"No hay recursos suficientes..."<<endl;
                cout<<"Se recomienda revisar los Vasos plasticos 32 Oz"<<endl;
                perfeccionista=false;
            }
            else if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"19",32*CantidadComida)==false and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"20",32*CantidadComida) and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"21",32*CantidadComida) and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"22",32*CantidadComida) and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,"23",32*CantidadComida)) {
                cout<<"No hay recursos suficientes..."<<endl;
                cout<<"Se recomienda revisar las gaseosas "<<endl;
                perfeccionista=false;
            }
            else {
                GastarRecursosInventario(AlmacenarInventario,"14",1*CantidadComida);
                CostoCombo+=(LeerProductosAgregarInventario["14"].precio/(LeerProductosAgregarInventario["14"].NumeroDePaquetes*LeerProductosAgregarInventario["14"].UnidadesPorPaquete))*CantidadComida;
                if(opcion == '1' and perfeccionista==true)
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Vaso de gaseosa mediana";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Vasos de gaseosa mediana";
                }
                perfeccionista=true;
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
                CostoCombo+=(LeerProductosAgregarInventario["15"].precio/(LeerProductosAgregarInventario["15"].NumeroDePaquetes*LeerProductosAgregarInventario["15"].UnidadesPorPaquete))*CantidadComida;
                if(opcion == '1' and perfeccionista==true)
                    contenido+=",";
                if(CantidadComida==1){
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Vaso de gaseosa pequena";
                }
                else {
                    contenido+= NumeroCadenaCaracterer(CantidadComida)+" Vasos de gaseosa pequena";
                }
                perfeccionista=true;
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
        if(contenido.size()>1 and contenido[contenido.size()-1]!=',')
            perfeccionista=true;
    }while(opcion == '1');

    for(int i=contenido.size();i>=0;--i){
        if(contenido[i]==','){
            contenido=contenido.substr(0,i)+" y "+contenido.substr(i+1,contenido.size()-i)+".";
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
    if(SegOpcion=='1'){
        TodosLosCombos.insert(pair<string,ComboCinema>(NombreCombo,auxiliar));
    }
    LeerProductosAgregarInventario.clear();
}

bool SiExisteLaComidaSuficienteEnElInventario(map<string,ProductoInvetario>& AlmacenarInventario, string IndiceProducto,int cantidad){
    if(AlmacenarInventario[IndiceProducto].NumeroDePaquetes*AlmacenarInventario[IndiceProducto].UnidadesPorPaquete >= cantidad)
        return true;
    else
        return false;
}

bool QueSeNecesitaParaCadaPreparacion(map<string,ProductoInvetario>& AlmacenarInventario,map<string,int> PreparacionCanidad,int Repetir){
    //Esta funcion analiza si sí tengo los materiales suficientes para hacer una preparacion.
    map<string,map<vector<int>,vector<string>>> ParaFormarUnCombo {{"Hamburguesa",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir},{"1","2","3","4"}}}},{"Perro caliente",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*14,PreparacionCanidad.begin()->second*Repetir},{"5","6","7","8","3"}}}},{"Nachos",{{{PreparacionCanidad.begin()->second*Repetir},{"9"}}}},{"Doritos",{{{PreparacionCanidad.begin()->second*Repetir},{"10"}}}},{"Detodito",{{{PreparacionCanidad.begin()->second*Repetir},{"11"}}}},{"Caja para palomitas 170 Oz",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*170},{"16","12"}}}},{"Caja para palomitas 130 Oz",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*150},{"17","12"}}}},{"Caja para palomitas 85 Oz",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*130},{"16","12"}}}},{"Vasos Plasticos 44 Oz",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*44,PreparacionCanidad.begin()->second*Repetir*44,PreparacionCanidad.begin()->second*Repetir*44,PreparacionCanidad.begin()->second*Repetir*44,PreparacionCanidad.begin()->second*Repetir*44},{"13","19","20","21","22","23"}}}},{"Vasos Plasticos 32 Oz",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*32,PreparacionCanidad.begin()->second*Repetir*32,PreparacionCanidad.begin()->second*Repetir*32,PreparacionCanidad.begin()->second*Repetir*32,PreparacionCanidad.begin()->second*Repetir*32},{"14","19","20","21","22","23"}}}},{"Vasos Plasticos 22 Oz",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*22,PreparacionCanidad.begin()->second*Repetir*22,PreparacionCanidad.begin()->second*Repetir*22,PreparacionCanidad.begin()->second*Repetir*22,PreparacionCanidad.begin()->second*Repetir*22},{"15","19","20","21","22","23"}}}}};
    bool indicador=true;
    auto i= ParaFormarUnCombo.find(PreparacionCanidad.begin()->first);
    for (int verificador=0;verificador<i->second.begin()->first.size();verificador++) {
        if(i->second.begin()->second[verificador] != "19" and i->second.begin()->second[verificador] != "20" and i->second.begin()->second[verificador] != "21" and i->second.begin()->second[verificador] != "22" and i->second.begin()->second[verificador] != "23"){
            indicador=SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,i->second.begin()->second[verificador],i->second.begin()->first[verificador]);
            if(indicador==false)
                return false;
        }
        else {
            if(SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,i->second.begin()->second[1],i->second.begin()->first[1])==false and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,i->second.begin()->second[2],i->second.begin()->first[2])==false and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,i->second.begin()->second[3],i->second.begin()->first[3])==false and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,i->second.begin()->second[4],i->second.begin()->first[4])==false and SiExisteLaComidaSuficienteEnElInventario(AlmacenarInventario,i->second.begin()->second[5],i->second.begin()->first[5])==false){
                return false;
            }
            else {
                return true;
            }
        }
    }
    return true;

}

void EjecutarGastoAutomatico(map<string,ProductoInvetario>& AlmacenarInventario,map<string,int> PreparacionCanidad,int Repetir){
    /*Esta funcion recibe el inventario (AlmacenarInventario), recibe el plato y el numero de estos que voy a incluir en el combo
     *  PreparacionCanidad y recibe la cantidad de combos que van a haber (repetir).
     * Si un combo tiene 1 hamburguesa ... y yo voy a crear 3 combos mas de ese mismo... voy a tener 3 hamburguesas en total.
     * repetir*1.
     * La variable ParaFormarUnCombo es un mapa que tiene como clave un string que es el plato (preparacion o comida) que va
     * a llevar el combo y tiene como valor otro mapa que a su vez tiene dos vectores uno como valor y otro como clave.
     * El vector valor de este ultimo mapa contiene todos los ingredientes que se necesitan para preparar el plato, mientras que
     * que el vector clave contiene las cantidades de estos ingredientes que se necesitan para preparar la comida.
     *
     */
    map<string,map<vector<int>,vector<string>>> ParaFormarUnCombo {{"Hamburguesa",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir},{"1","2","3","4"}}}},{"Perro caliente",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*14,PreparacionCanidad.begin()->second*Repetir},{"5","6","7","8","3"}}}},{"Nachos",{{{PreparacionCanidad.begin()->second*Repetir},{"9"}}}},{"Doritos",{{{PreparacionCanidad.begin()->second*Repetir},{"10"}}}},{"Detodito",{{{PreparacionCanidad.begin()->second*Repetir},{"11"}}}},{"Caja para palomitas 170 Oz",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*170},{"16","12"}}}},{"Caja para palomitas 130 Oz",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*150},{"17","12"}}}},{"Caja para palomitas 85 Oz",{{{PreparacionCanidad.begin()->second*Repetir,PreparacionCanidad.begin()->second*Repetir*130},{"16","12"}}}},{"Vasos Plasticos 44 Oz",{{{PreparacionCanidad.begin()->second*Repetir},{"13"}}}},{"Vasos Plasticos 32 Oz",{{{PreparacionCanidad.begin()->second*Repetir},{"14"}}}},{"Vasos Plasticos 22 Oz",{{{PreparacionCanidad.begin()->second*Repetir},{"15"}}}}};
    auto i= ParaFormarUnCombo.find(PreparacionCanidad.begin()->first);
    for (int iterador=0;iterador<i->second.begin()->first.size();++iterador) {
        GastarRecursosInventario(AlmacenarInventario,i->second.begin()->second[iterador],i->second.begin()->first[iterador]);
    }
}
map<string,int> DesglozarElementosDelContenidoDeUnCombo(string & contenido){
    /*Esta funcion toma el contenido de un determinado combo y lo desgloza de tal manera que el programa pueda trabajar con esa inforacion.
     * ejemplo:
     * La funcion recibe un strin con la informacion "1 hamburguesa, 1 paquete de doritos y 1 vaso de gaseosa grande"
     * Inicialmente la funcion retornará 1 hamburguesa. Esto mediante un mapa cuya clave es hambuguesa y cuyo valor es 1.
     * Pero como exterior a esta funcion va a ver un bucle que se va a ejecutar hasta que contenido sea igual a vacio
     * habrá retornado tambien <Doritos,1> y <Vasos Plasticos 44 Oz,1>
     */
    map<string,int> AuxiliarProductoCantidad;
    string ayuda;
    int numero;
    int i=contenido.find(',');
    if(i==-1)
        i=contenido.find(" y ");
    if(i==-1)
        i=contenido.find('.');
    ayuda=contenido.substr(0,i);
    contenido=contenido.substr(i+1,contenido.size()-i);
    if(contenido[0]=='y'){
        contenido.erase(0,2);
    }
    i=ayuda.find(' ');
    numero=StringAnumero(ayuda.substr(0,i));
    ayuda=ayuda.substr(i+1,ayuda.size()-i);
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
        AuxiliarProductoCantidad.insert(pair<string,int>("Caja para palomitas 170 Oz",numero));
    else if(ayuda=="Caja mediana de palomitas" or ayuda == "Cajas medianas de palomitas")
        AuxiliarProductoCantidad.insert(pair<string,int>("Caja para palomitas 130 Oz",numero));
    else if(ayuda=="Caja pequena de palomitas" or ayuda == "Cajas pequenas de palomitas")
        AuxiliarProductoCantidad.insert(pair<string,int>("Caja para palomitas 85 Oz",numero));
    else if(ayuda=="Vaso de gaseosa grande" or ayuda == "Vasos de gaseosa grande")
        AuxiliarProductoCantidad.insert(pair<string,int>("Vasos Plasticos 44 Oz",numero));
    else if(ayuda=="Vaso de gaseosa mediana" or ayuda == "Vasos de gaseosa mediana")
        AuxiliarProductoCantidad.insert(pair<string,int>("Vasos Plasticos 32 Oz",numero));
    else if(ayuda=="Vaso de gaseosa pequena" or ayuda == "Vasos de gaseosa pequena")
        AuxiliarProductoCantidad.insert(pair<string,int>("Vasos Plasticos 22 Oz",numero));
    return AuxiliarProductoCantidad;

}
void FormarCombos(map<string,ProductoInvetario>& AlmacenarInventario, map<string,ComboCinema>& TodosLosCombos){
    //Esta funcion permite crear, recargar y eliminar combos.
    char opcion;
    bool bandera;
    map<string,int> ProductoCantidad;
    string NombreComboRecargar;
    string AnalizarContenidoCombo;
    do{
        opcion=SubMenuFormarCombo();
        switch (opcion) {
        case '1':{
            MostrarCombos(TodosLosCombos); //Se muestran los combos.
            break;
        }
        case '2':{
            cout<<"Ingrese el nombre del combo que desea recargar: "<<endl;
            getline(cin,NombreComboRecargar);
            getline(cin,NombreComboRecargar);
            if(TodosLosCombos.find(NombreComboRecargar)==TodosLosCombos.end()) //Aqui se recagan los combos existentes.
                cout<<"Imposible recargar un combo que no existe. "<<endl;
            else {
                int numero;
                cout<<"Cuantos combos nuevos desea crear? "<<endl;
                cin>>numero;
                AnalizarContenidoCombo=TodosLosCombos[NombreComboRecargar].Componentes;
                while(AnalizarContenidoCombo != ""){
                    ProductoCantidad=DesglozarElementosDelContenidoDeUnCombo(AnalizarContenidoCombo);
                    bandera=QueSeNecesitaParaCadaPreparacion(AlmacenarInventario,ProductoCantidad,numero);
                    if(bandera == false){
                        cout<<"No hay comida suficientes para elvorar el pedido realizado. Por favor, surta el inventario. "<<endl;
                        break;
                    }
                }
                if(bandera==true){
                    AnalizarContenidoCombo=TodosLosCombos[NombreComboRecargar].Componentes;
                    while (AnalizarContenidoCombo!= "") {
                        ProductoCantidad=DesglozarElementosDelContenidoDeUnCombo(AnalizarContenidoCombo);
                        EjecutarGastoAutomatico(AlmacenarInventario,ProductoCantidad,numero);
                    }
                    TodosLosCombos[NombreComboRecargar].CuantosDeEsteHay+=numero;
                }
            }
           break;
        }
        case '3':{
            string NombreCombo;
            cout<<"Ingrese el nombre del combo  que desea crear: "<<endl;
            getline(cin,NombreCombo);
            getline(cin,NombreCombo);
            if(TodosLosCombos.find(NombreCombo)!=TodosLosCombos.end())
                cout<<"No pueden haber dos combos con el mismo nombre. "<<endl;
            else {
                CrearNuevoCombo(AlmacenarInventario,NombreCombo,TodosLosCombos);
                cout<<"Creacion exitosa."<<endl;
            }
            break;
        }
        case '4':{
            string NombreCombo;
            cout<<"Ingrese el nombre del combo  que desea eliminar: "<<endl;
            getline(cin,NombreCombo);
            getline(cin,NombreCombo);
            if(TodosLosCombos.find(NombreCombo)==TodosLosCombos.end())
                cout<<"Imposible eliminar un combo que no existe. "<<endl;
            else {
                if(TodosLosCombos[NombreCombo].CuantosDeEsteHay!=0)
                    cout<<"Para eliminar a un combo del catalogo este debe tener una disponibilidad igual a cero. "<<endl;
                else {
                    TodosLosCombos.erase(NombreCombo);
                    cout<<"Combo eliminado del catalogo. "<<endl;
                }
            }
            break;
        }
        case '5':
            EscribirEnArchiParaCombos(TodosLosCombos);
            cout<<"Los cambios han sido guardados correctamente. "<<endl;
            break;
        }
    }while(opcion != '5');
}


void LeerInventario( map<string,ProductoInvetario>& AlmacenarInventario){
    //Esta funcion lee el inventario y lo almacena en la variable Almacenar Inventario.
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
            while (retornar>=1) {
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
    for(auto i:AlmacenarInventario){
        cout<<"Nombre: "<<i.second.nombre<<endl;
        if(i.second.nombre=="Gaseosa Pepsi" or i.second.nombre=="Cocacola" or i.second.nombre=="Gaseosa Postobon Manzana" or i.second.nombre=="Gaseosa Postobon Uva" or i.second.nombre=="Dry Ginger" ){
            cout<<"Unidades por paquete: "<<i.second.UnidadesPorPaquete<<" Onzas" <<endl;
        }
        else if(i.second.nombre=="Palomitas"){
            cout<<"Unidades por paquete: "<<i.second.UnidadesPorPaquete<<" Gramos"<<endl;
        }
        else {
            cout<<"Unidades por paquete: "<<i.second.UnidadesPorPaquete<<" unidades"<<endl;
        }
        cout<<"Numero de paquetes "<<i.second.NumeroDePaquetes<<endl;;
        cout<<endl<<endl;
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
       while (!archivo.eof()) {
           getline(archivo,LeerLinea);
           if(LeerLinea != ""){
               indice=LeerLinea.substr(0,LeerLinea.find('+'));
               LeerLinea=LeerLinea.substr(LeerLinea.find('+')+1,LeerLinea.size()-LeerLinea.find('+'));
                auxilar.nombre=LeerLinea.substr(0,LeerLinea.find('+'));
               LeerLinea=LeerLinea.substr(LeerLinea.find('+')+1,LeerLinea.size()-LeerLinea.find('+'));
               auxilar.UnidadesPorPaquete=StringAnumero(LeerLinea.substr(0,LeerLinea.find('+')));
               LeerLinea=LeerLinea.substr(LeerLinea.find('+')+1,LeerLinea.size()-LeerLinea.find('+'));
               auxilar.precio=StringAnumero(LeerLinea.substr(0,LeerLinea.find('+')));
               auxilar.NumeroDePaquetes=1.0;
               LeerProductosAgregarInventario.insert(pair<string,ProductoInvetario>(indice,auxilar));
           }
       }
   }
    archivo.close();
}

void AgregarAInventario(map<string,ProductoInvetario>& AlmacenarInventario){
    //Esta funcion me permite agregar productos al inventario.
    map<string,ProductoInvetario> LeerProductosAgregarInventario;
    LeerArticulosParaAgregarInventario(LeerProductosAgregarInventario);
    cout<<"Los productos que la administracion puede comprar son los siguientes: "<<endl;
    cout<<endl<<endl;
    cout<<"    Nombre                       U/P  Precio"<<endl; // Muesta el nombre del producto, las unidades por paquete y el precio de cada paquete.
    for (auto i:LeerProductosAgregarInventario) {
        if(i.first=="1" or i.first=="2" or i.first=="3" or i.first=="4" or i.first=="5" or i.first=="6" or i.first=="7" or i.first=="8" or i.first=="9")
            cout<<" ";
        cout<<i.first<<") "<<i.second.nombre;
        for(int a=i.second.nombre.size();a<=29;a++)
            cout<<" ";
        cout<<i.second.UnidadesPorPaquete<<"    "<<i.second.precio<<endl;
    }
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t); // Obtiene el la fecha y hora actual.
    ostringstream oss;
    oss << put_time(&tm, "%d-%m-%Y %H-%M-%S");
    auto str = oss.str();  // Guarda en la variable str la fecha y hora en la que se hizo la compra.
    str= "Fecha: "+ str.substr(0,str.find(" "))+" Hora: "+ str.substr(str.find(" ")+1,str.size()-str.find(" "));
    EscribirRegistroDeComprasParaInventario(str);
    char opcion;
    do{
        cout<<"Desea agregar un producto al inventario? "<<endl;
        cout<<"Si -> 1\nNo -> Cualquier otra tecla. "<<endl;
        cin>>opcion;
        if(opcion=='1'){
            string reporte;
            string indice;
            int cantidad;
            cout<<"Ingrese el indice del producto que la administracion desea comprar: "<<endl;
            cin>>indice;
            cout<<"Ingrese cuantos de estos paquetes desea comprar: "<<endl;
            cin>>cantidad;
            AlmacenarInventario[indice].NumeroDePaquetes+=LeerProductosAgregarInventario[indice].NumeroDePaquetes*cantidad; //Aumenta la cantidad de producto en el inventario.
            // Esta linea almacena que y cuanto producto se compro.
            reporte=LeerProductosAgregarInventario[indice].nombre+" "+NumeroCadenaCaracterer(LeerProductosAgregarInventario[indice].UnidadesPorPaquete)+" "+NumeroCadenaCaracterer(LeerProductosAgregarInventario[indice].NumeroDePaquetes*cantidad)+" "+NumeroCadenaCaracterer(LeerProductosAgregarInventario[indice].precio*cantidad);
            //Esta funcion escribe en un archo el reporte.
            EscribirRegistroDeComprasParaInventario(reporte);
        }
    }while(opcion == '1');
     EscribirRegistroDeComprasParaInventario("         ");
    LeerProductosAgregarInventario.clear();
}

void EscribirArchivoInventario(map<string,ProductoInvetario>& AlmacenarInventario){
    //Con esta funcion se guardan los cambios hechos en el inventario.
    ofstream archivo("inventario.txt");
    if(archivo.fail())
        cout<<"Error al abrir el archivo. Escribir archivo inventario. "<<endl;
    else {
        for (auto i:AlmacenarInventario) {
            std::ostringstream ss;
            ss << i.second.NumeroDePaquetes;
            std::string s(ss.str());
            archivo<<i.first+"+"+i.second.nombre+"+"+NumeroCadenaCaracterer(i.second.UnidadesPorPaquete)+"+"+s+"+"+NumeroCadenaCaracterer(i.second.precio)<<endl;
        }
    }
    archivo.close();
}

void EscribirEnArchiParaCombos(map<string,ComboCinema>& TodosLosCombos){
    //Con esta funcion guardo los combos creados o modificados.
    ofstream archivo("combos.txt");
    if(archivo.fail())
        cout<<"No se pudo abrir el archivo. EscribirEnArchiParaCombos"<<endl;
    else {
        for(auto i:TodosLosCombos){
            archivo<<i.second.Nombre+"+"+i.second.Componentes+"+"+NumeroCadenaCaracterer(i.second.precio)+"+"+NumeroCadenaCaracterer(i.second.CuantosDeEsteHay)+"+"<<endl;
        }
    }
    archivo.close();

}
void EscribirRegistroDeComprasParaInventario(string reporte){
    //Con esta funcion guardo las comprar que el administrador haya hecho.
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
