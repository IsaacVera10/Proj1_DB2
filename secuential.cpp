#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <cmath>
#include <climits>
#include <vector>
#include <windows.h>

using namespace std;

struct Record{
    char id[7];
    char prenda[10];
    char genero[1];
    float precio;
    char stock[4];
    char marca[7];  
    pair<int, char> punt;
    Record() {}
    Record(string id_,string prenda_,string genero_,float precio_,string stock_,string marca_){
        for(int i=0;i<7;i++){
            this->id[i]=id_[i];
        }
        for(int i=0;i<10;i++){
            this->prenda[i]=prenda_[i];
        }
        for(int i=0;i<1;i++){
            this->genero[i]=genero_[i];
        }
        this->precio=precio_;
        for(int i=0;i<4;i++){
            this->stock[i]=stock_[i];
        }
        for(int i=0;i<7;i++){
            this->marca[i]=marca_[i];
        }
        punt={0,'r'};
    }

    template <typename T>
    void copyFromBuffer(const char*& buffer, T& variable) {
        memcpy(&variable, buffer, sizeof(variable));
        buffer += sizeof(variable);
    }

    void desempaquetar(const char* buffer, int n) {
        const char* current = buffer;
        current += sizeof(size_t);
        copyFromBuffer(current, id);
        copyFromBuffer(current, prenda);
        copyFromBuffer(current, genero);
        copyFromBuffer(current, precio);
        copyFromBuffer(current, stock);
    }

    void show_data(){
        cout<<"ID: "<<this->id<<" Prenda: "<<this->prenda<<" genero: "<<this->genero<<" precio: "<<this->precio<<" stock: "<<this->stock<<endl;
    }
};
    
class secuential{
    string datos="datos.dat";
    string aux="datosaux.dat";
    string datosV="datosV.dat";
    long n = 3;
    long k = log2(n);
    int cont=0;
    pair<int,char> Header;
    int eliminados=0;
    public:
    secuential(){
        Header.first=-1;
        Header.second='d';
        ofstream file(datos,ios::binary);
        file.close();
        ofstream file1(aux,ios::binary);
        file1.close();
    }

    void add(Record Key);
    pair<Record,pair<int,char>> Search_Record(Record Key);
    template<typename T>
    vector<Record> rangeSearch(T begin_key, T end_key);
    template<typename T>
    void remove(T Key);
    void Rebuild();

    void Read_data(string data,Record& record){
        ifstream file(data,ios::binary);
        file.seekg((record.punt.first) * sizeof(Record));
        file.read((char*)&record, sizeof(Record));
        file.close();
    }

    void write_aux(Record Key, pair<int,char>& record){
        ofstream file (aux,ios::app|ios::binary);
        pair<int,char>temp=record;
        record.second='a';
        Key.punt=temp;   
        file.write((char*)&Key, sizeof(Record));
        file.close();
        ifstream file2(aux,ios::ate|ios::binary);
        record.first=((file2.tellg()/sizeof(Record))-1);
        file2.close();
    }

    void write_data(Record Key, pair<int,char>& record){
        ofstream file (datos,ios::app|ios::binary);
        pair<int,char>temp=record;
        record.second='d';
        record.first=(file.tellp()/sizeof(Record));
        Key.punt=temp;   
        file.write((char*)&Key, sizeof(Record));
        file.close();
        ifstream file2(datos,ios::ate|ios::binary);
        record.first=((file2.tellg()/sizeof(Record))-1);
        file2.close();
    }

    void display(){
        Record record;
        int cont=0;
        while(record.punt.first!=-1){
            ifstream file(datos, ios::binary);
            file.seekg(cont * sizeof(Record));
            file.read((char*)&record, sizeof(Record));
            file.close();
            cout<<"Marca: "<<record.id<<" "<<record.punt.first<<" "<<record.punt.second<<endl;
            cont++;
        }
        cont=0;
        while(cont<4){
            ifstream file(aux, ios::binary);
            file.seekg(cont * sizeof(Record));
            file.read((char*)&record, sizeof(Record));
            file.close();
            cout<<"Marca2: "<<record.id<<" "<<record.punt.first<<" "<<record.punt.second<<endl;
            cont++;
        }
    }
    int sizeRecord(){
        return 33;
    }
    
    size_t size(string data){
        ifstream file(data,ios::binary);
        file.seekg(0,ios::end);
        size_t t=file.tellg()/sizeof(Record);
        file.close();
        return t;
    }

};

void secuential::add(Record Key){
    //Data vacia
    if(Header.first ==-1){
        fstream file(datos,ios::out|ios::binary);
        Key.punt=Header;
        Header.first= 0;
        Header.second= 'd';
        file.write((char*)&Key, sizeof(Record));
        file.close();
    }else if(size(aux)<n){ //Mientras que no se llene la Data
        pair<Record,pair<int,char>> anterior = Search_Record(Key);
        if(anterior.second.first==-1){//Caso que el valor de la key sea al inicio de todo
            write_aux(Key,Header);
        }else if(anterior.first.punt.first==-1){//cuando es el ultimo termino
            if(anterior.first.punt.second = 'd'){//insertar sabiendo que el ultimo esta en el datos
                //escribir datos d
                write_data(Key,anterior.first.punt);
                //actualizar el anterior
                fstream file2(datos,ios::out|ios::in|ios::ate|ios::binary);
                file2.seekp(anterior.second.first*sizeof(Record));
                file2.write((char*)&anterior.first,sizeof(Record));
                file2.close();
            }else if(anterior.first.punt.second = 'a'){//insertar sabiendo que es siguietne esta en aux
                bool val=true;
                int pos;
                while(val){
                    pos=anterior.first.punt.first;//guardar su posicion del registro final de aux
                    Read_data(aux,anterior.first);
                    if(anterior.first.punt.first=-1){
                        val=false;
                    }
                }
                //Grabar en datos
                ofstream file (datos,ios::app|ios::binary);
                anterior.first.punt.first=(file.tellp()/sizeof(Record));
                anterior.first.punt.second='d';
                Key.punt={-1,'d'};   
                file.write((char*)&Key, sizeof(Record));
                file.close();
                //actualizar el anterior
                fstream file3(aux,ios::out|ios::in|ios::ate|ios::binary);
                file3.seekp(pos*sizeof(Record));
                file3.write((char*)&anterior.first,sizeof(Record));
                file3.close();
            }
        }else{//cuando la key esta entre valores
            if(anterior.first.punt.second == 'd'){//agregamos directamente al aux ya que el siguiente es del d
                write_aux(Key,anterior.first.punt);
                //actualizar el  anterior
                fstream file2(datos,ios::out|ios::in|ios::ate|ios::binary);
                file2.seekp(anterior.second.first*sizeof(Record));
                file2.write((char*)&anterior.first,sizeof(Record));
                file2.close(); 
            }else if(anterior.first.punt.second == 'a'){
                Record val_a = anterior.first;
                Read_data(aux,val_a);
                
                if(Key.id<val_a.id){
                    write_aux(Key,anterior.first.punt);
                    //actualizar el anterior
                    fstream file2(datos,ios::out|ios::in|ios::ate|ios::binary);
                    file2.seekp(anterior.second.first*sizeof(Record));
                    file2.write((char*)&anterior.first,sizeof(Record));
                    file2.close(); 
                }else{
                    bool val=true;
                    anterior.second=anterior.first.punt;  //almacenar posicion actual
                    anterior.first=val_a;//almacenar posicion de enlaze
                    Read_data(aux,val_a);
                    while(Key.id>val_a.id){
                        if(val_a.punt.second=='d'){
                            val=false;
                            break;
                        }
                        anterior.second=anterior.first.punt;  
                        anterior.first=val_a;   
                        Read_data(aux,val_a);
                    }  
                    if(val==false){
                        write_aux(Key,val_a.punt);
                        //actualizar el anterior
                        fstream file2(aux,ios::out|ios::in|ios::ate|ios::binary);
                        file2.seekp(anterior.first.punt.first*sizeof(Record));  
                        file2.write((char*)&val_a,sizeof(Record));
                        file2.close(); 
                    }else{
                        write_aux(Key,anterior.first.punt);
                        //actualizar el anterior
                        fstream file2(aux,ios::out|ios::in|ios::ate|ios::binary);
                        file2.seekp(anterior.second.first*sizeof(Record));  
                        file2.write((char*)&anterior.first,sizeof(Record));
                        file2.close(); 
                    }
                }
            }
        }
    }else{ //Cuando la Data se lleno empezamos solo a llenar el aux
        Rebuild();  
        add(Key);
    }
}

pair<Record,pair<int,char>> secuential::Search_Record(Record Key){
    ifstream file(datos,ios::ate|ios::binary);
    if(!file.is_open())throw "No se pudo abrir el archivo";
    pair<Record,pair<int,char>> retornar;
    //busqueda proporcionada en clase
    int m,l=0;
    int u=(file.tellg()/sizeof(Record))-1;
    Record record;
    while(u>=l){
        m=(u+l)/2;
        file.seekg(m*sizeof(Record));
        file.read((char*)&record, sizeof(Record));
        if(Key.id < record.id){
            u= m-1;
        }else if(Key.id  > record.id){
            l= m+1;
        }else{
            retornar.first=record;
            retornar.second=record.punt;
            return retornar;
        }
    }
    if(u!=-1){
        file.seekg(u*sizeof(Record));
        file.read((char*)&record, sizeof(Record));
        file.close();
    }
    
    return pair<Record,pair<int,char>>(record,{u, record.punt.second});
}

template<typename T>
vector<Record> secuential::rangeSearch(T begin_key, T end_key){
    Rebuild();
    pair<Record,pair<int,char>> inicio = Search_Record(begin_key);
    pair<Record,pair<int,char>> fin = Search_Record(end_key);
    vector<Record>rango;
    while(inicio.first.id != fin.first.id ){
        rango.push_back(inicio.first);
        ifstream file(datos,ios::binary);
        file.seekg(inicio.first.punt.first*sizeof(Record));
        file.read((char*)&inicio.first,sizeof(Record));
        file.close();
        cout<<"inic "<<inicio.first.id<<endl;
    }
    rango.push_back(fin.first);
    return rango; 
}

template<typename T>
void secuential::remove(T Key){
    Record record;
    record.id=Key;
    pair<Record,pair<int,char>>valor=Search_Record(record);
    if(record.id==valor.first.id){
        ifstream file2(datos,ios::binary);
        file2.seekg(((valor.second.first-1)*sizeof(Record)));
        file2.read((char*)&record,sizeof(Record));
        file2.close();
        if(record.punt.second == 'd'){//Verificar si el anterior esta en  datos o aux
            record.punt = valor.first.punt;
            valor.first.punt.second = 'x';
            ofstream file(datos,ios::out||ios::binary);
            file.seekp(valor.second.first*sizeof(Record));
            file.write((char*)&valor.first,sizeof(Record)); //escribir el x en el borrado
            file.seekp((valor.second.first-1)*sizeof(Record));
            file.write((char*)&record,sizeof(Record)); //escribir en el anterio el puntero al siguiente
            file.close();
            eliminados++;
        }else{// el anterior esta en aux
            //Mover entre el aux con el read;
            int pos=0;
            ifstream file3(aux,ios::binary);
            file3.seekg(pos*sizeof(Record));
            file3.read((char*)&record,sizeof(Record));
            while(record.punt.first!=valor.second.first && 'd'!=record.punt.second){
                pos++;
                file3.seekg(pos*sizeof(Record));
                file3.read((char*)&record,sizeof(Record));
            }
            file3.close();
            record.punt = valor.first.punt;
            valor.first.punt.second = 'x';

            ofstream file4(aux,ios::binary);
            file4.seekp(pos*sizeof(Record));
            file4.write((char*)&record,sizeof(Record)); //escribir el aux modificado
            file4.close();

            ofstream file6(datos,ios::binary);
            file6.seekp(valor.second.first*sizeof(Record));
            file6.write((char*)&valor.first,sizeof(Record)); //escribir x en eleliminado
            file6.close();
            eliminados++;
        }
    }else{//el key.id es > que el valor.first.id
        if(valor.first.punt.second == 'd'){//verificar que el siguiente es d
            throw("No se encuentra la Key");
        }else{//verificar en el aux
            pair<int,char> puntero=valor.second;
            Record almacen=valor.first;
            Read_data(aux,valor.first);
            while(valor.first.id<Key){
                puntero=almacen.punt;
                almacen=valor.first;
                Read_data(aux,valor.first);
            }
            if(valor.first.id!=Key){
                throw("No se encuentra la Key");
            }else{//cuando el key esta en el aux
                valor.second = almacen.punt;
                almacen.punt=valor.first.punt;
                valor.first.punt.second = 'x';
                if(puntero.second=='a'){
                    ofstream file5(aux, ios::binary|ios::app);//agregar a los registros del archivo auxiliar
                    file5.seekp(valor.second.first*sizeof(Record));
                    file5.write((char*)&valor.first , sizeof (Record));
                    file5.seekp(puntero.first*sizeof(Record));
                    file5.write((char*)&almacen, sizeof (Record));
                    file5.close();
                }else{
                    ofstream file5(aux, ios::binary|ios::app);//agregar a los registros del archivo auxiliar
                    file5.seekp(valor.second.first*sizeof(Record));
                    file5.write((char*)&valor.first , sizeof (Record));
                    file5.close();
                    ofstream file7(datos, ios::binary|ios::app);
                    file7.seekp(puntero.first*sizeof(Record));
                    file7.write((char*)&almacen, sizeof (Record));
                    file7.close();
                }
                eliminados++;
            }
        }       
    }
}
void secuential::Rebuild(){
    Record record;
    record.punt=Header;
    ofstream file(datosV,ios::out|ios::binary);
    while(record.punt.first!=-1 ||  record.punt.second!='d'){
        if(record.punt.second=='a'){
            Read_data(aux,record);
            file.write((char*)&record,sizeof(Record));
        }else if(record.punt.second=='d'){
            Read_data(datos,record);
            file.write((char*)&record,sizeof(Record));
        }
    }
    file.close();
    
    ifstream file2(datosV,ios::binary);
    ofstream file3(datos,ios::binary);
    for(int i=0;i<size(datosV);i++){
        file2.seekg(i* sizeof(Record));
        file2.read((char*)&record,sizeof(Record));
        if(record.punt.first!=-1)
            record.punt.first=i+1;
            record.punt.second='d';
        file3.write((char*)&record,sizeof(Record));
    }
    Header={0,'d'};//acutalizar header
    file2.close();
    file3.close();
    std::remove("datosaux.dat");//Eliminar y crear structura
    ofstream nuevoAux(aux);
    nuevoAux.close();
}

int main() {
    //secuential sequential;
    Record record;

    ifstream file("datos2.dat", ios::binary); 
    char * buffer = new char[sizeof(Record)];
    int dato=0;
    while(dato<40){
        file.seekg(dato* sizeof(Record));
        file.read(buffer, sizeof(Record));
        record.desempaquetar(buffer,sizeof(Record));
        record.show_data();
        dato++;
    }
    
    file.close();

    //sequential.display();

    /*
    secuential sequential;
    Record record1 = {"2020","polo","M",13.3,"12","Dove"};
    Record record2 = {"2040","polo","M",13.3,"12","Dove"};
    Record record3 = {"2030","polo","M",13.3,"12","Dove"};
    Record record4 = {"2050","polo","M",13.3,"12","Dove"};
    Record record5 = {"2025","polo","M",13.3,"12","Dove"};
    Record record6 = {"1033","polo","M",13.3,"12","Dove"};
    Record record8 = {"1000","polo","M",13.3,"12","Dove"};
    Record record9 = {"2180","polo","M",13.3,"12","Dove"};
    Record record10 = {"2280","polo","M",13.3,"12","Dove"};
    Record record11 = {"2380","polo","M",13.3,"12","Dove"};
    Record record12 = {"2480","polo","M",13.3,"12","Dove"};
    Record record13 = {"2580","polo","M",13.3,"12","Dove"};
    Record record14 = {"2680","polo","M",13.3,"12","Dove"};
    Record record15 = {"2780","polo","M",13.3,"12","Dove"};
    Record record16 = {"2880","polo","M",13.3,"12","Dove"};
    Record record17 = {"2980","polo","M",13.3,"12","Dove"};
    Record record18 = {"3000","polo","M",13.3,"12","Dove"};
    Record record19 = {"3010","polo","M",13.3,"12","Dove"};

    
    sequential.add(record1);
    sequential.add(record2);
    sequential.add(record3);
    sequential.add(record4);
    sequential.add(record5);
    sequential.add(record6);
    sequential.add(record8);
    sequential.add(record9);
    sequential.add(record10);
    sequential.add(record11);
    sequential.add(record12);
    sequential.add(record13);
    sequential.add(record14);
    sequential.add(record15);
    sequential.add(record16);
    sequential.add(record17);
    sequential.add(record18);
    sequential.add(record19);
    //sequential.display();

    vector<Record> almacen = sequential.rangeSearch(record1,record4);
    
   
    for(int i=0;i<almacen.size();i++){
        almacen[i].show_data();
    }
    while(pos<10){

        file.seekg(pos * sizeof(Record), ios::beg);
        file.read((char *)&record, sizeof(Record));
        pos +=1;
        cout<<record.id<<" - "<<record.genero<<" - "<<record.marca<<" - "<<record.precio<<" -"<<record.prenda<<endl;

    }*/

    return 0;
}