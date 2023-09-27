#pragma once

#include <vector>
#include <fstream>
#include <bitset>
#include <cmath>

using namespace std;

const int fillfactor = 5;

struct Directory{
    char* key;
    long pos_fisica;
};

template<typename RegisterType>
struct Bucket {
    RegisterType keys[fillfactor];
    int count = 0;
    int profundidad_local = 1;
    int next = -1;

    void insert(RegisterType reg){
        keys[count] = reg;
        count++;
    }

    bool isfull(){ return count == fillfactor; }
};

template <typename RegisterType>
class ExtendibleHash{ 
private:

    string hash_file_name;
    string buckets_file_name;

    int profundidad_global = 1;
    int max_depth = 3;

public:

    ExtendibleHash(string hfn, string bfn, int md, int gd=1){
        hash_file_name = hfn;
        buckets_file_name = bfn;  
        max_depth = md;
        profundidad_global = gd;
        
        initialize_buckets();   
    }

    bool remove(typename RegisterType::KeyType key){    
        char* i = hashFunc(key, profundidad_global);
        int index = get_bucket_pos_from_index(i);
        Bucket<RegisterType> bucket = bucket_from_bin(i);

        while(true){
            for(int j = 0; j < bucket.count; j++){
                if(bucket.keys[j].getKey() == key) {
                    redo_bucket(bucket, j);
                    write_bucket(index, bucket);
                    return true;
                }
            }
            if(bucket.next != -1){
                index = bucket.next;
                bucket = read_bucket((bucket.next*sizeof(Bucket<RegisterType>))+sizeof(int));
            } else {
                break;
            }
        }
        return false;
    }

    vector<RegisterType> search(typename RegisterType::KeyType key){    
        vector<RegisterType> result;
        char* i = hashFunc(key, profundidad_global);
        int index = get_bucket_pos_from_index(i);
        Bucket<RegisterType> bucket = bucket_from_bin(i);

        while(true){
            for(int j = 0; j < bucket.count; j++){
                if(bucket.keys[j].getKey() == key)
                {
                    result.push_back(bucket.keys[j]);
                    return result;
                }
            }
            if(bucket.next != -1){
                bucket = read_bucket((bucket.next*sizeof(Bucket<RegisterType>))+sizeof(int));
            } else {
                break;
            }
        }
        return result;
    }

    void insert(RegisterType reg){                      
        if(find(reg.getKey())){
            
            return;
        }

        char* i = hashFunc(reg.getKey(), profundidad_global); 
        int index = get_bucket_pos_from_index(i);    
        Bucket<RegisterType> bucket = bucket_from_bin(i);


        if(!bucket.isfull())
        {                           
            bucket.insert(reg);
            write_bucket(index, bucket);  
            //read_dir();  
            return;    
        } 
        else 
        {
            if(bucket.profundidad_local == profundidad_global)
            {                           
                if(bucket.profundidad_local+1 > max_depth)
                {                       
                    while(bucket.isfull())
                    {                   
                        if(bucket.next != -1)
                        {
                            index = bucket.next;
                            bucket = read_bucket((index*sizeof(Bucket<RegisterType>))+sizeof(int));
                        } else {                        
                            bucket.next = get_num_buckets();
                            write_bucket(index, bucket);

                            Bucket<RegisterType> new_bucket;
                            new_bucket.profundidad_local = bucket.profundidad_local;
                            new_bucket.insert(reg);
                            write_bucket(get_num_buckets(), new_bucket);
                            return;
                        }
                    }                   
                    bucket.insert(reg);
                    write_bucket(index,bucket);
                    return;
                }                       
                profundidad_global++; 
                split(bucket, index);
                duplicate_dir(index+pow(2,profundidad_global-1));
                insert(reg);
                return;
            } else {   
                split(bucket, index);
                reindex(index);
                insert(reg);
                return;
            }
        }
    }


    void display_indexes(){     
        read_dir();
    }

    void display_buckets(){     
        vector<Directory> dirs;
        ifstream file(hash_file_name, ios::in | ios::binary);
        for(int i = 0; i < pow(2, profundidad_global); i++){
            Directory dir;
            file.read((char*)&dir, sizeof(Directory));
            dirs.push_back(dir);
        }
        file.close();
        // Get directories.
        cout << "[" << profundidad_global << "]" << endl;
        for(const auto x: dirs){
            Bucket<RegisterType> bucket = read_bucket(x.pos_fisica);
            cout << "[" << x.key << "." << bucket.profundidad_local << "] ";
            display_bucket(bucket);
        }
        cout << "Successfully read: " << get_num_buckets() << " buckets." << endl;
    }
 
    void print_all_buckets(){  
        for(int i = 0; i < get_num_buckets(); i++){
            cout << "[" << i << "] ";
            Bucket<RegisterType> bucket = read_bucket((i*sizeof(Bucket<RegisterType>))+sizeof(int));
            for(int j = 0; j < bucket.count; j++){
                cout << bucket.keys[j].getKey() << ",";
            }
            cout << endl;
        }
    }

    vector<RegisterType> range_search(typename RegisterType::KeyType start, typename RegisterType::KeyType end){
        vector<RegisterType> res;
        for(int i = 0; i < get_num_buckets(); i++){
            Bucket<RegisterType> bucket = read_bucket((i*sizeof(Bucket<RegisterType>))+sizeof(int));
            for(int j = 0; j < bucket.count; j++){
                if(bucket.keys[j].getKey() > start && bucket.keys[j].getKey() < end)
                    res.push_back(bucket.keys[j]);
            }
        }
        return res;
    }


private:

   

    void initialize_buckets(){                        
        ifstream file(buckets_file_name, ios::in | ios::binary);
        if(file.good()){
            file.seekg(0, ios::beg);
            file.read((char*)&profundidad_global, sizeof(int));
        }
        else {
            ofstream file(buckets_file_name, ios::out | ios::binary);
            file.write((char*)&profundidad_global, sizeof(int));
            int num_buckets = pow(2, profundidad_global);
            for(int i = 0; i < num_buckets; i++){
                Bucket<RegisterType> b;
                b.profundidad_local = profundidad_global;
                file.write((char*)&b, sizeof(Bucket<RegisterType>));
            }
            
            file.close();

            ofstream dir_file(hash_file_name, ios::out | ios::binary);
            for(int i = 0; i < num_buckets; i++){
                Directory dir;
                dir.key = to_binary(i, profundidad_global);
                dir.pos_fisica = sizeof(int) + (sizeof(Bucket<RegisterType>)*i);
                dir_file.write((char*)&dir, sizeof(Directory));
            }
            dir_file.close();
        }
    }

    void split(Bucket<RegisterType> bucket, int index){ 
        Bucket<RegisterType> new_b;
        Bucket<RegisterType> old_b;
        new_b.profundidad_local = bucket.profundidad_local+1;
        old_b.profundidad_local = bucket.profundidad_local+1;

        char* first_index = hashFunc(bucket.keys[0].getKey(), profundidad_global);;

        for(int i = 0; i < bucket.count; i++){
            char* ind = hashFunc(bucket.keys[i].getKey(), profundidad_global);
            if(ind[0] == '0') old_b.insert(bucket.keys[i]);
            else new_b.insert(bucket.keys[i]);
        }

        write_bucket(index, old_b);
        
        write_bucket(get_num_buckets(), new_b);
    }
   
    void duplicate_dir(int index){                      
        vector<Directory> dirs;
        dirs.clear(); 
        ifstream read_file(hash_file_name, ios::in | ios::binary);
        for(int i = 0; i < pow(2, profundidad_global-1); i++){
            Directory dir;
            read_file.read((char*)&dir, sizeof(Directory));
            dirs.push_back(dir);
        }
        read_file.close();

        ofstream write_file(hash_file_name, ios::out | ios::binary);
        int i = 0;
        for(int j = 0; j < 2; j++){
            for(auto &x: dirs){
                if(i == index){
                
                    x.pos_fisica = ((get_num_buckets()-1)*sizeof(Bucket<RegisterType>))+sizeof(int);
                }
                x.key = to_binary(i, profundidad_global);
                write_file.write((char*)&x, sizeof(Directory));
                i++;
            }
        }
        write_file.close();

    }

    void reindex(int index){                            
        vector<Directory> direcs = get_indexes(index);
        for(int i = 0; i < direcs.size(); i++)
        {
            if(i % 2 != 0)
            {
                direcs[i].pos_fisica = ((get_num_buckets()-1)*sizeof(Bucket<RegisterType>))+sizeof(int);
                write_direc(direcs[i]);
            }
        }  
        return;
    }

    void redo_bucket(Bucket<RegisterType> &bucket, int index){    
        bucket.count--;
        for(int i = index; i < bucket.count; i++){
            if(i < bucket.count) bucket.keys[i] = bucket.keys[i+1];
        }
    }


    void write_direc(Directory direc){                         
        int pos = get_dir_position(direc);
        fstream file(hash_file_name, ios::in | ios::out | ios::binary);
        file.seekp((pos * sizeof(Directory)), ios::beg);
        file.write(reinterpret_cast<const char*>(&direc), sizeof(Directory));
        file.close();
    }

    void write_bucket(int pos, Bucket<RegisterType> bucket){    
        fstream file(buckets_file_name, ios::in | ios::out | ios::binary);
        file.seekp((pos * sizeof(Bucket<RegisterType>)) + sizeof(int), ios::beg);
        file.write(reinterpret_cast<const char*>(&bucket), sizeof(Bucket<RegisterType>));
        file.close();                               
    }


    void read_dir(){                                    
        vector<Directory> dirs;
        ifstream file(hash_file_name, ios::in | ios::binary);
        for(int i = 0; i < pow(2, profundidad_global); i++){
            Directory dir;
            file.read((char*)&dir, sizeof(Directory));
            dirs.push_back(dir);
        }
        cout << "-- Directory:" << endl;
        for(const auto x: dirs){
            cout << x.key << " - " << x.pos_fisica << endl;
        }
        cout << "-----\n";
    }

    void display_bucket(Bucket<RegisterType> bucket){ 
        for(int i = 0; i < bucket.count; i++){
            cout << bucket.keys[i].getKey() << ", ";
        }
        if(bucket.next == -1){
            cout << bucket.next << endl;
        } else {
            cout << "-> ";
            display_bucket(read_bucket((bucket.next*sizeof(Bucket<RegisterType>))+sizeof(int)));
        }
    }

    int get_dir_position(Directory direc){             
        return std::stoi(direc.key, nullptr, 2);
    }

    int get_num_buckets(){                              
        std::ifstream file(buckets_file_name, ios::in | ios::binary);
        file.seekg(0, ios::end);
        int length = file.tellg();
        length -= sizeof(int);
        file.seekg(0, ios::beg);
        file.close();
        return length/(sizeof(Bucket<RegisterType>));
    }

    int get_bucket_pos_from_index(char* index) {
        int index_new = std::stoi(index, nullptr, 2);
        ifstream file(hash_file_name, ios::in | ios::binary);
        Directory direc;
        file.seekg(index_new * sizeof(Directory), ios::beg);
        file.read((char*)&direc, sizeof(Directory));
        file.close();
        return ((direc.pos_fisica-sizeof(int))/sizeof(Bucket<RegisterType>));
    }


    Bucket<RegisterType> bucket_from_bin(string bin){   
        int index = std::stoi(bin, nullptr, 2);
        ifstream file(hash_file_name, ios::in | ios::binary);
        Directory direc;
        file.seekg(index * sizeof(Directory), ios::beg);
        file.read((char*)&direc, sizeof(Directory));
        file.close();
        return read_bucket(direc.pos_fisica);
    }

    Bucket<RegisterType> read_bucket(int pos){         
        ifstream file(buckets_file_name, ios::binary | ios::in); 
        file.seekg(pos, ios::beg);
        Bucket<RegisterType> bucket;
        file.read(reinterpret_cast<char*>(&bucket), sizeof(Bucket<RegisterType>));
        file.close();
        return bucket;
    }

    vector<Directory> get_indexes(int index){         
        std::ifstream file(hash_file_name, ios::in | ios::binary);
        Directory dir;
        file.seekg(index*sizeof(Directory), ios::beg);
        file.read((char*)&dir, sizeof(Directory));
        int x = dir.pos_fisica;
        file.seekg(0, ios::beg);
        for(int i = 0; i < pow(2,profundidad_global); i++){
            Directory t_dir;
            file.seekg(i*sizeof(Directory), ios::beg);
            file.read((char*)&t_dir, sizeof(Directory));
            if(t_dir.pos_fisica == x) {
                dirs.push_back(t_dir);
            }
        }
        return dirs;
    }

    bool find(typename RegisterType::KeyType key){     
        char* i = hashFunc(key, profundidad_global);
        int index = get_bucket_pos_from_index(i);
        Bucket<RegisterType> bucket = bucket_from_bin(i);

        while(true){
            for(int j = 0; j < bucket.count; j++){
                if(bucket.keys[j].getKey() == key) return true;
            }
            if(bucket.next != -1){
                bucket = read_bucket((bucket.next*sizeof(Bucket<RegisterType>))+sizeof(int));
            } else {
                break;
            }
        }
        return false;
    }

    char* to_binary(int num, int digits) {        
        
        std::bitset<32> bits(num); 
        std::string binary_str = bits.to_string().substr(32 - digits); 
        char* binary = new char[10]; 
        for (int i = 0; i < digits; i++) {
            binary[i] = binary_str[i]; 
        }
        binary[digits] = '\0'; 
        return binary;
    }

    char* hashFunc(std::string key, int digits){
        int sum = 0;
        for(int i = 0; i < key.length(); i++){
            sum += int(key[i]);
        }
        int n = pow(2, digits);
        return to_binary(sum % int(pow(2,max_depth)), digits);
    }


    char* hashFunc(int key, int digits){               
        return to_binary(key % int(pow(2, max_depth)), digits);
    }

public:
    ~ExtendibleHash(){
        fstream output_file(buckets_file_name, ios::binary | ios::in | ios::out);
        output_file.seekp(0, ios::beg);
        output_file.write(reinterpret_cast<const char*>(&profundidad_global), sizeof(int));
        output_file.close();
    }
};
