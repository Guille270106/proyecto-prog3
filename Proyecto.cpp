#include<iostream> //entradas y salidas estandar
#include<sstream>  //procesar cada linea del archivo csv, dividiendola en campos separados por coma
#include<fstream>  //abrir y leer el archivo csv
#include <stdexcept> // excepciones predefinidas para el control de errores
#include "ListaDoble.h" //para listas doblemente enlazadas (ventas)
#include "Venta.h" //clase venta
#include "HashMapList.h" 
#include "Lista.h"
#define NOMBRE_ARCHIVO "ventas_sudamericana.csv" //reemplaza NOMBRE_ARCHIVO automaticamente por "ventas_sudamericanas.csv"
using namespace std;

void top5_ciudades(ListaDoble<venta> &ventas){
    HashMapList<string, HashMapList<string, float>> ventas_ciudad_pais;
    
    for(int i=0; i<ventas.size(); i++){
        venta v= ventas.get(i);
        string pais=v.getpais();
        string ciudad=v.getciudad();
        float monto_total=v.getMontoTotal();
    
        HashMapList<string,float> ciudades;

        try{
            // Intentar obtener hashmap de ciudades para este país
            ciudades=ventas_ciudad_pais.get(pais);
        }catch(const exception &e){
            // Si no existe, inicializo uno vacío
            ciudades=HashMapList<string,float>();
        }

        try {
            // Intentar obtener monto acumulado para la ciudad
            float monto_actual=ciudades.get(ciudad);
            ciudades.put(ciudad, monto_actual+monto_total);
        } catch (const exception &e) {
            // Si no existe la ciudad, agrego el monto inicial
            ciudades.put(ciudad, monto_total);
        }

        // Actualizo el hashmap con la ciudad modificada
        ventas_ciudad_pais.put(pais, ciudades);
    }


        
}

void cargar_ventas(ListaDoble<venta> &ventas, const string NOMBRE_ARCHIVO){ //funcion que recibe como parametro la lista que guardara las ventas y el archivo de donde se van a leer los datos
    ifstream archivo_ventas(NOMBRE_ARCHIVO); //crea el objeto archivo_ventas que abre el archivo
    if(!archivo_ventas.is_open()){ //verifica si se abrio
        cout<<"Error al abrir el archivo."<<endl;
        return 1;
    }

    string linea; //almacena c/linea del archivo
    char delimitador= ','; //separa los campos en el archivo

    getline(archivo_ventas, linea); //lee los encabezados del archivo
     while (getline(archivo, linea)) { //mientras haya lineas por leer
        try{
            stringstream stream(linea); //trata la linea como un flujo
            string id_venta, fecha, pais, ciudad, cliente, producto, categoria, cantidad_str, precio_unitario_str, monto_total_str, medio_envio, estado_envio;
            getline(stream, id_venta, delimitador);
            getline(stream, fecha, delimitador);
            getline(stream, pais, delimitador);
            getline(stream, ciudad, delimitador);
            getline(stream, cliente, delimitador);
            getline(stream, producto, delimitador);
            getline(stream, categoria, delimitador);
            getline(stream, cantidad_str, delimitador);
            getline(stream, precio_unitario_str, delimitador);
            getline(stream, monto_total_str, delimitador);
            getline(stream, medio_envio, delimitador);
            getline(stream, estado_envio, delimitador); //se extraen los campos de la linea y los separa por el delimitador, los almacena en las variables que corresponde.
            
            // Convertir strings numéricos
            int cantidad=stoi(cantidad_str); //convierte a int
            float precio_unitario=stof(precio_unitario); //convierte a float
            float monto_total=stof(monto_total_str);
            
            // Crear objeto Venta y agregar a la lista
            Venta v(id_venta, fecha, pais, ciudad, cliente, producto, categoria, cantidad, precio_unitario, monto_total, medio_envio, estado_envio);
            ventas.insertarUltimo(v);
        }catch(const invalid_argument &e){ //maneja error en la conversion de datos
            cout<<"Error de conversion en la linea: "<<linea<<endl;
            cout<<"Detalles: "<<e.what()<<endl;
        }catch(const exception &e){ //error generico
            cout<<"Error inesperado: "<<e.what()<<endl;
        }
        
     }
     archivo_ventas.close();
}

int main(){
    ListaDoble<venta> ventas;
    cargar_ventas(ventas, "ventas_sudamericanas.csv");



    return 0;
}
