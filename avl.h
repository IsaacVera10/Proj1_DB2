#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

struct MovieRecord {
    char id[7];
    char prenda[10];
    char genero[1]; 
    float precio;
    char stock[4]; 
    char marca[7];
    long left = -1, right = -1;
    int height = 1; 

		MovieRecord() {}
    MovieRecord(string id_, string prenda_, string genero_, float precio_, string stock_, string marca_) {
        for (int i = 0; i < 7; i++) {
            this->id[i] = id_[i];
        }
        for (int i = 0; i < 10; i++) {
            this->prenda[i] = prenda_[i];
        }
        for (int i = 0; i < 1; i++) {
            this->genero[i] = genero_[i];
        }
        this->precio = precio_;
        for (int i = 0; i < 4; i++) {
            this->stock[i] = stock_[i];
        }
        for (int i = 0; i < 7; i++) {
            this->marca[i] = marca_[i];
        }
    }
    void showData() {
    if (id[0] == '\0') {
        return; // Skip displaying data for empty nodes
    }
    cout << "\nID: " << id;
    cout << "\nPrenda: " << prenda;
    cout << "\nGénero: " << genero;
    cout << "\nPrecio: " << precio;
    cout << "\nStock: " << stock;
    cout << "\nMarca: " << marca << endl;
}

};

class AVLFile {
private:
    string filename;
    long pos_root;

public:
    AVLFile(string filename) {
        this->filename = filename;
        fstream file(filename, ios::binary | ios::in | ios::out);
        this->pos_root = -1;
        if (!file.is_open()) {
            file.close();
            fstream newFile(filename, ios::binary | ios::out);
            newFile.close();
        }
        else {
            file.read(reinterpret_cast<char *>(&pos_root), sizeof(long));
            file.close();
        }
    }

    ~AVLFile() {
        fstream file(filename, ios::binary | ios::in | ios::out);
        file.write(reinterpret_cast<char *>(&pos_root), sizeof(long));
        file.close();
    }

    MovieRecord find(char id[7]) {
        return find(pos_root, id);
    }

    void insert(MovieRecord record) {
        insert(pos_root, record);
    }

    void remove(char *id) {
        remove(pos_root, id);
    }

    vector<MovieRecord> inorder() {
        return inorder(pos_root);
    }

    vector<MovieRecord> range_search(char start[7], char end[7]) {
        vector<MovieRecord> result;
        range_search(pos_root, start, end, result);
        return result;
    }

private:
    MovieRecord find(long pos_node, char id[7]) {
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "Error" << endl;
            return MovieRecord{"", "", "", 0.0, "", ""};
        }
        file.seekg(pos_node * sizeof(MovieRecord));
        MovieRecord node;
        file.read(reinterpret_cast<char *>(&node), sizeof(MovieRecord));
        file.close();
        if (strcmp(node.id, id) == 0) {
            return node;
        }
        else if (strcmp(id, node.id) < 0 && node.left != -1) {
            return find(node.left, id);
        }
        else if (strcmp(id, node.id) > 0 && node.right != -1) {
            return find(node.right, id);
        }
        return MovieRecord{"", "", "", 0.0, "", ""};
    }

    void insert(long &pos_node, MovieRecord record) {
        if (pos_node == -1) {
            fstream file(filename, ios::binary | ios::in | ios::out);
            file.seekp(0, ios::end);
            pos_node = file.tellp() / sizeof(MovieRecord);
            file.write(reinterpret_cast<char *>(&record), sizeof(MovieRecord));
            file.close();
        }
        else {
            fstream file(filename, ios::binary | ios::in | ios::out);
            file.seekg(pos_node * sizeof(MovieRecord));
            MovieRecord node;
            file.read(reinterpret_cast<char *>(&node), sizeof(MovieRecord));
            if (strcmp(record.id, node.id) < 0) {
                if (node.left == -1) {
                    file.seekp(0, ios::end);
                    node.left = file.tellp() / sizeof(MovieRecord);
                    file.write(reinterpret_cast<char *>(&record), sizeof(MovieRecord));
                    file.seekp(pos_node * sizeof(MovieRecord));
                    file.write(reinterpret_cast<char *>(&node), sizeof(MovieRecord));
                    file.close();
                }
                else {
                    insert(node.left, record);
                }
            }
            else {
                if (node.right == -1) {
                    file.seekp(0, ios::end);
                    node.right = file.tellp() / sizeof(MovieRecord);
                    file.write(reinterpret_cast<char *>(&record), sizeof(MovieRecord));
                    file.seekp(pos_node * sizeof(MovieRecord));
                    file.write(reinterpret_cast<char *>(&node), sizeof(MovieRecord));
                    file.close();
                }
                else {
                    insert(node.right, record);
                }
            }
            // Update the height of the current node
            node.height = 1 + max(getHeight(node.left), getHeight(node.right));
            // Perform balancing
            balance(pos_node);
        }
    }
void remove(long &pos_node, const char *key) {
    if (pos_node == -1) {
        return; // Key not found, nothing to delete
    }

    MovieRecord node = getRecord(pos_node);

    // Compare characters individually
    int i = 0;
    while (key[i] != '\0' && node.id[i] != '\0' && key[i] == node.id[i]) {
        i++;
    }

    if (key[i] == '\0' && node.id[i] == '\0') {
        // Node with the key to be deleted found

        // Node with one child or no child
        if (node.left == -1 || node.right == -1) {
            long temp = (node.left == -1) ? node.right : node.left;

            // No child case
            if (temp == -1) {
                temp = pos_node;
                pos_node = -1; // Set the current node to null
            } else {
                node = getRecord(temp); // Copy the contents of the child
                pos_node = temp;
            }
        } else {
            // Node with two children: get the in-order successor
            long successorPos = findMin(node.right);
            MovieRecord &successor = getRecord(successorPos);

            // Copy the in-order successor's data to this node
            strcpy(node.id, successor.id);
            strcpy(node.prenda, successor.prenda);
            strcpy(node.genero, successor.genero);
            node.precio = successor.precio;
            strcpy(node.stock, successor.stock);
            strcpy(node.marca, successor.marca);

            // Delete the in-order successor recursively
            remove(node.right, successor.id);
        }
    } else if (key[i] < node.id[i]) {
        remove(node.left, key);
    } else {
        remove(node.right, key);
    }

    if (pos_node == -1) {
        return; // The tree is empty or the node was removed
    }

    // Update height of the current node
    node.height = 1 + max(getHeight(node.left), getHeight(node.right));

    // Rebalance the tree
    balance(pos_node);
}



    long findMin(long pos_node) {
        while (getRecord(pos_node).left != -1) {
            pos_node = getRecord(pos_node).left;
        }
        return pos_node;
    }

    MovieRecord& getRecord(long pos) {
        ifstream file(filename, ios::binary);
        static MovieRecord node; // A static variable to store the result
        file.seekg(pos * sizeof(MovieRecord));
        file.read(reinterpret_cast<char *>(&node), sizeof(MovieRecord));
        file.close();
        return node;
    }

    int getHeight(long pos_node) {
        if (pos_node == -1) {
            return 0;
        }
        return getRecord(pos_node).height;
    }

    int getBalance(long pos_node) {
        if (pos_node == -1) {
            return 0;
        }
        return getHeight(getRecord(pos_node).left) - getHeight(getRecord(pos_node).right);
    }

    void balance(long &pos_node) {
        int balance = getBalance(pos_node);

        // Left Heavy
        if (balance > 1) {
            long leftChild = getRecord(pos_node).left;
            // Left-Right Case
            if (getBalance(leftChild) < 0) {
                leftRotate(leftChild);
            }
            rightRotate(pos_node);
        }
        // Right Heavy
        else if (balance < -1) {
            long rightChild = getRecord(pos_node).right;
            // Right-Left Case
            if (getBalance(rightChild) > 0) {
                rightRotate(rightChild);
            }
            leftRotate(pos_node);
        }
    }

    void leftRotate(long &pos_node) {
        long newRoot = getRecord(pos_node).right;
        long &parentNode = pos_node;
        long leftSubtree = getRecord(newRoot).left;

        MovieRecord &parentRecord = getRecord(parentNode);
        MovieRecord &newRootRecord = getRecord(newRoot);

        newRootRecord.left = parentNode; // Assign the values directly
        parentRecord.right = leftSubtree; // Assign the values directly

        // Update heights
        parentRecord.height = 1 + max(getHeight(parentRecord.left), getHeight(parentRecord.right));
        newRootRecord.height = 1 + max(getHeight(newRootRecord.left), getHeight(newRootRecord.right));

        parentNode = newRoot;
    }

    void rightRotate(long &pos_node) {
        long newRoot = getRecord(pos_node).left;
        long &parentNode = pos_node;
        long rightSubtree = getRecord(newRoot).right;

        MovieRecord &parentRecord = getRecord(parentNode);
        MovieRecord &newRootRecord = getRecord(newRoot);

        newRootRecord.right = parentNode; // Assign the values directly
        parentRecord.left = rightSubtree; // Assign the values directly

        // Update heights
        parentRecord.height = 1 + max(getHeight(parentRecord.left), getHeight(parentRecord.right));
        newRootRecord.height = 1 + max(getHeight(newRootRecord.left), getHeight(newRootRecord.right));

        parentNode = newRoot;
    }

   void range_search(long pos_node, const char *start, const char *end, vector<MovieRecord> &result) {
    if (pos_node == -1) {
        return;
    }
    MovieRecord node = getRecord(pos_node);
    int id_as_int = atoi(node.id);
    int start_as_int = atoi(start);
    int end_as_int = atoi(end);

    if (id_as_int >= start_as_int && id_as_int <= end_as_int) {
        result.push_back(node);
    }
    if (id_as_int > start_as_int) {
        range_search(node.left, start, end, result);
    }
    if (id_as_int < end_as_int) {
        range_search(node.right, start, end, result);
    }
}



    vector<MovieRecord> inorder(long pos_node) {
        if (pos_node == -1) {
            return vector<MovieRecord>(); // Return an empty vector when the node is empty.
        }
        MovieRecord node = getRecord(pos_node);
        vector<MovieRecord> result;
        vector<MovieRecord> leftSubtree = inorder(node.left);
        result.insert(result.end(), leftSubtree.begin(), leftSubtree.end());
        result.push_back(node);
        vector<MovieRecord> rightSubtree = inorder(node.right);
        result.insert(result.end(), rightSubtree.begin(), rightSubtree.end());
        return result;
    }
};
