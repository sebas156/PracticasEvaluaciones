#include "funciones.h"
#include "objetospertinentes.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
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


void GastarRecursosInventario(map<string,ProductoInvetario>& AlmacenarInventario,string NombreProductoUsado, int UnidadesUsadas){
    auto i = AlmacenarInventario.find( NombreProductoUsado);
    i->second.NumeroDePaquetes= (((i->second.UnidadesPorPaquete)*(i->second.NumeroDePaquetes))-UnidadesUsadas)*(i->second.NumeroDePaquetes)/((i->second.UnidadesPorPaquete)*(i->second.NumeroDePaquetes));
}

void ConstruirElCombo(map<string,ProductoInvetario>& AlmacenarInventario,string NombreCombo){
    vector<string> ParaFormarUnCombo{"Hamburguesa","Perro Caliente","Papas a la Francesa","Vaso Plastico 44 Oz","Vaso Plastico 32 Oz","Vaso Plastico 22 Oz","Caja de palomitas 170 Oz","Caja de palomitas 130 Oz","Caja de palomitas 85 Oz","Paquetes de papas"};
    vector<string> ParaHamburguesa{"Pan para hamburguesa","Carne para hamburguesa","Queso tajado","Tomate"};
    vector <string> ParaPerros{"Pan para perro","Chorizo","Queso tajado","Huevos de codorniz","Ripio de papas"};
    char IndicadorGlobal='A';
    int CantidadComida;
    int CostoCombo=0;
    int elemento;
    for(int i=0;i<ParaFormarUnCombo.size();i++){
        cout<<i+1<<") "<<ParaFormarUnCombo[i]<<endl;
    }
    cout<<"Ingrese la posicion de la preparacion que va a incluir el combo: "<<endl;
    cin>>elemento;
    cout<<"Ingrese el numero de estas preparaciones que tendra el combo: "<<endl;
    cin>>CantidadComida;
    switch (elemento) {
       case 1:{
        for (int iterador=0;iterador<ParaHamburguesa.size();iterador++) {
            GastarRecursosInventario(AlmacenarInventario,ParaHamburguesa[iterador],1);
            CostoCombo+=AlmacenarInventario.find(ParaHamburguesa[iterador])->second.PrecioTotal/((AlmacenarInventario.find(ParaHamburguesa[iterador])->second.NumeroDePaquetes)*(AlmacenarInventario.find(ParaHamburguesa[iterador])->second.UnidadesPorPaquete))+1000;
        }
    break;
    }
    case 2:{
        for (int iterador=0;iterador<ParaPerros.size()-1;iterador++) {
            if(ParaPerros[iterador]!= "Ripio de papas")
                GastarRecursosInventario(AlmacenarInventario,ParaPerros[iterador],1);
           else
                GastarRecursosInventario(AlmacenarInventario,ParaPerros[iterador],150);
            CostoCombo+=AlmacenarInventario.find(ParaPerros[iterador])->second.PrecioTotal/((AlmacenarInventario.find(ParaPerros[iterador])->second.NumeroDePaquetes)*(AlmacenarInventario.find(ParaPerros[iterador])->second.UnidadesPorPaquete))+1000;
        }
        break;
    }
    case 3:{
        GastarRecursosInventario(AlmacenarInventario,"Papas a la francesa",250);
         CostoCombo+=AlmacenarInventario.find("Papas a la francesa")->second.PrecioTotal/((AlmacenarInventario.find("Papas a la francesa")->second.NumeroDePaquetes)*(AlmacenarInventario.find("Papas a la francesa")->second.UnidadesPorPaquete))+1000;
        break;
    }
    case 4:{
        GastarRecursosInventario(AlmacenarInventario,"Vaso Plastico 44 Oz",1);
        CostoCombo+=AlmacenarInventario.find("Vaso Plastico 44 Oz")->second.PrecioTotal/((AlmacenarInventario.find("Vaso Plastico 44 Oz")->second.NumeroDePaquetes)*(AlmacenarInventario.find("Vaso Plastico 44 Oz")->second.UnidadesPorPaquete))+500;
        break;
    }
    case 5:{
        GastarRecursosInventario(AlmacenarInventario,"Vaso Plastico 32 Oz",1);
        CostoCombo+=AlmacenarInventario.find("Vaso Plastico 32 Oz")->second.PrecioTotal/((AlmacenarInventario.find("Vaso Plastico 32 Oz")->second.NumeroDePaquetes)*(AlmacenarInventario.find("Vaso Plastico 32 Oz")->second.UnidadesPorPaquete))+500;
        break;
    }
    case 6:{
        GastarRecursosInventario(AlmacenarInventario,"Vaso Plastico 22 Oz",1);
        CostoCombo+=AlmacenarInventario.find("Vaso Plastico 22 Oz")->second.PrecioTotal/((AlmacenarInventario.find("Vaso Plastico 22 Oz")->second.NumeroDePaquetes)*(AlmacenarInventario.find("Vaso Plastico 22 Oz")->second.UnidadesPorPaquete))+500;
        break;
    }
    case 7:{
        GastarRecursosInventario(AlmacenarInventario,"Caja de palomitas 170 Oz",1);
        CostoCombo+=AlmacenarInventario.find("Caja de palomitas 170 Oz")->second.PrecioTotal/((AlmacenarInventario.find("Caja de palomitas 170 Oz")->second.NumeroDePaquetes)*(AlmacenarInventario.find("Caja de palomitas 170 Oz")->second.UnidadesPorPaquete))+500;
        GastarRecursosInventario(AlmacenarInventario,"Palomitas",150);
        CostoCombo+=AlmacenarInventario.find("Palomitas")->second.PrecioTotal/((AlmacenarInventario.find("Palomitas")->second.NumeroDePaquetes)*(AlmacenarInventario.find("Palomitas")->second.UnidadesPorPaquete))+200;
        break;
    }
    case 8:{
        GastarRecursosInventario(AlmacenarInventario,"Caja de palomitas 130 Oz",1);
        CostoCombo+=AlmacenarInventario.find("Caja de palomitas 130 Oz")->second.PrecioTotal/((AlmacenarInventario.find("Caja de palomitas 130 Oz")->second.NumeroDePaquetes)*(AlmacenarInventario.find("Caja de palomitas 130 Oz")->second.UnidadesPorPaquete))+500;
        GastarRecursosInventario(AlmacenarInventario,"Palomitas",100);
        CostoCombo+=AlmacenarInventario.find("Palomitas")->second.PrecioTotal/((AlmacenarInventario.find("Palomitas")->second.NumeroDePaquetes)*(AlmacenarInventario.find("Palomitas")->second.UnidadesPorPaquete))+200;

    }
    case 9:{
        GastarRecursosInventario(AlmacenarInventario,"Caja de palomitas 85 Oz",1);
        CostoCombo+=AlmacenarInventario.find("Caja de palomitas 85 Oz")->second.PrecioTotal/((AlmacenarInventario.find("Caja de palomitas 85 Oz")->second.NumeroDePaquetes)*(AlmacenarInventario.find("Caja de palomitas 85 Oz")->second.UnidadesPorPaquete))+500;
        GastarRecursosInventario(AlmacenarInventario,"Palomitas",75);
        CostoCombo+=AlmacenarInventario.find("Palomitas")->second.PrecioTotal/((AlmacenarInventario.find("Palomitas")->second.NumeroDePaquetes)*(AlmacenarInventario.find("Palomitas")->second.UnidadesPorPaquete))+200;
        break;
    }
    case 10:
        GastarRecursosInventario(AlmacenarInventario,"Paquete de papas",1);
     break;
    case 11:
     break;
    default:
        cout<<"Opcion ingresada deconocida. "<<endl;
        break;
    }
}
void FormarCombos(map<string,ProductoInvetario>& AlmacenarInventario, map<string,ComboCinema>& TodosLosCombos){
    char opcion;
    do{
        opcion=SubMenuFormarCombo();
        switch (opcion) {
        case '1':{
            MostrarCombos(TodosLosCombos);
        }
            break;
        case '2':{
            string NombreComboRecargar;
            cout<<"Ingrese el nombre del combo que desea recargar: "<<endl;
            getline(cin,NombreComboRecargar);
        }
            break;
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
string LeerLinea,nombre,UnidadesPorPaquete,NumeroDePaquetes,PrecioTotal;
    fstream archivo("inventario.txt");
    if(archivo.fail())
        cout<<"Error al abrir el archivo, formar combos..."<<endl;
    else {
        while (!archivo.eof()) {
            getline(archivo,LeerLinea);
            if(LeerLinea!=""){
                ProductoInvetario auxiliar;
                auxiliar.nombre=LeerLinea.substr(0,LeerLinea.find("+"));
                LeerLinea=LeerLinea.substr(LeerLinea.find("+")+1,LeerLinea.size()-LeerLinea.find("+"));
                UnidadesPorPaquete=LeerLinea.substr(0,LeerLinea.find("+"));
                auxiliar.UnidadesPorPaquete=StringAnumero(UnidadesPorPaquete);
                LeerLinea=LeerLinea.substr(LeerLinea.find("+")+1,LeerLinea.size()-LeerLinea.find("+"));
               NumeroDePaquetes=LeerLinea.substr(0,LeerLinea.find("+"));
               auxiliar.NumeroDePaquetes=StringAFloat(NumeroDePaquetes);
               LeerLinea=LeerLinea.substr(LeerLinea.find("+")+1,LeerLinea.size()-LeerLinea.find("+"));
               PrecioTotal=LeerLinea.substr(0,LeerLinea.find("+"));
               auxiliar.PrecioTotal=StringAnumero(PrecioTotal);
               AlmacenarInventario.insert(pair<string,ProductoInvetario>(nombre,auxiliar));
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
            cout<<"Costo Total "<<i.second.PrecioTotal<<endl;
            cout<<endl<<endl;
        }
    }
    else {
        cout<<"Por el momento el inventario se encuentra vacio. "<<endl;
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
