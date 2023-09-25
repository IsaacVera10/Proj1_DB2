#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

struct Record
{
    int cod;
    char nombre[12];
    int ciclo;
    long left = -1, right = -1;
    int height = 1; 

    void showData()
    {
        cout << "\nCodigo: " << cod;
        cout << "\nNombre: " << nombre;
        cout << "\nCiclo : " << ciclo << endl;
    }
};

class AVLFile
{
private:
    string filename;
    long pos_root;

public:
    AVLFile(string filename)
    {
        this->filename = filename;
        fstream file(filename, ios::binary | ios::in | ios::out);
        this->pos_root = -1;
        if (!file.is_open())
        {
            file.close();
            fstream newFile(filename, ios::binary | ios::out);
            newFile.close();
        }
        else
        {
            file.read(reinterpret_cast<char *>(&pos_root), sizeof(long));
            file.close();
        }
    }

    ~AVLFile()
    {
        fstream file(filename, ios::binary | ios::in | ios::out);
        file.write(reinterpret_cast<char *>(&pos_root), sizeof(long));
        file.close();
    }

    Record find(int key)
    {
        return find(pos_root, key);
    }

    void insert(Record record)
    {
        insert(pos_root, record);
    }

    void remove(int key)
    {
        remove(pos_root, key);
    }

    vector<Record> inorder()
    {
        return inorder(pos_root);
    }

    vector<Record> range_search(int low, int high)
    {
        vector<Record> result;
        range_search(pos_root, low, high, result);
        return result;
    }

private:
    Record find(long pos_node, int key)
    {
        ifstream file(filename, ios::binary);
        if (!file)
        {
            cerr << "Error" << endl;
            return Record{-1, "", -1};
        }
        file.seekg(pos_node * sizeof(Record));
        Record node;
        file.read(reinterpret_cast<char *>(&node), sizeof(Record));
        file.close();
        if (node.cod == key)
        {
            return node;
        }
        else if (key < node.cod && node.left != -1)
        {
            return find(node.left, key);
        }
        else if (key > node.cod && node.right != -1)
        {
            return find(node.right, key);
        }
        return Record{-1, "", -1};
    }

    void insert(long &pos_node, Record record)
    {
        if (pos_node == -1)
        {
            fstream file(filename, ios::binary | ios::in | ios::out);
            file.seekp(0, ios::end);
            pos_node = file.tellp() / sizeof(Record);
            file.write(reinterpret_cast<char *>(&record), sizeof(Record));
            file.close();
        }
        else
        {
            fstream file(filename, ios::binary | ios::in | ios::out);
            file.seekg(pos_node * sizeof(Record));
            Record node;
            file.read(reinterpret_cast<char *>(&node), sizeof(Record));
            if (record.cod < node.cod)
            {
                if (node.left == -1)
                {
                    file.seekp(0, ios::end);
                    node.left = file.tellp() / sizeof(Record);
                    file.write(reinterpret_cast<char *>(&record), sizeof(Record));
                    file.seekp(pos_node * sizeof(Record));
                    file.write(reinterpret_cast<char *>(&node), sizeof(Record));
                    file.close();
                }
                else
                {
                    insert(node.left, record);
                }
            }
            else
            {
                if (node.right == -1)
                {
                    file.seekp(0, ios::end);
                    node.right = file.tellp() / sizeof(Record);
                    file.write(reinterpret_cast<char *>(&record), sizeof(Record));
                    file.seekp(pos_node * sizeof(Record));
                    file.write(reinterpret_cast<char *>(&node), sizeof(Record));
                    file.close();
                }
                else
                {
                    insert(node.right, record);
                }
            }
            node.height = 1 + max(getHeight(node.left), getHeight(node.right));
            balance(pos_node);
        }
    }

    void remove(long &pos_node, int key)
{
    if (pos_node == -1)
    {
        return;
    }

    Record node = getRecord(pos_node);

    if (key < node.cod)
    {
        remove(node.left, key);
    }
    else if (key > node.cod)
    {
        remove(node.right, key);
    }
    else
    {
        if (node.left == -1 && node.right == -1)
        {
       
            pos_node = -1; 

            fstream file(filename, ios::binary | ios::in | ios::out);
            file.seekp(pos_node * sizeof(Record));
            Record deletedNode;
            file.write(reinterpret_cast<char *>(&deletedNode), sizeof(Record));
            file.close();
        }
        else if (node.left == -1 || node.right == -1)
        {

            pos_node = (node.left == -1) ? node.right : node.left; 
            fstream file(filename, ios::binary | ios::in | ios::out);
            file.seekp(pos_node * sizeof(Record));
            Record deletedNode;
            file.write(reinterpret_cast<char *>(&deletedNode), sizeof(Record));
            file.close();
        }
        else
        {
           
            long successorPos = findMin(node.right); 
            Record successor = getRecord(successorPos);
            node.cod = successor.cod; 
            remove(node.right, successor.cod); 
        }
    }

    
    balance(pos_node);
}
long findMin(long pos_node)
{
    while (getRecord(pos_node).left != -1)
    {
        pos_node = getRecord(pos_node).left;
    }
    return pos_node;
}
Record& getRecord(long pos)
{
    ifstream file(filename, ios::binary);
    static Record node; 
    file.seekg(pos * sizeof(Record));
    file.read(reinterpret_cast<char *>(&node), sizeof(Record));
    file.close();
    return node;
}


    long minValueNode(long pos_node)
    {
        Record node = getRecord(pos_node);
        while (node.left != -1)
        {
            node = getRecord(node.left);
        }
        return pos_node;
    }

    int getHeight(long pos_node)
    {
        if (pos_node == -1)
        {
            return 0;
        }
        return getRecord(pos_node).height;
    }

    int getBalance(long pos_node)
    {
        if (pos_node == -1)
        {
            return 0;
        }
        return getHeight(getRecord(pos_node).left) - getHeight(getRecord(pos_node).right);
    }

    void balance(long &pos_node)
{
    int balance = getBalance(pos_node);

    // Left Heavy
    if (balance > 1)
    {
        long leftChild = getRecord(pos_node).left;
        // Left-Right Case
        if (getBalance(leftChild) < 0)
        {
            leftRotate(leftChild);
        }
        rightRotate(pos_node);
    }
    // Right Heavy
    else if (balance < -1)
    {
        long rightChild = getRecord(pos_node).right;
        // Right-Left Case
        if (getBalance(rightChild) > 0)
        {
            rightRotate(rightChild);
        }
        leftRotate(pos_node);
    }
}

void leftRotate(long &pos_node)
{
    long newRoot = getRecord(pos_node).right;
    long &parentNode = pos_node;
    long leftSubtree = getRecord(newRoot).left;

    Record &parentRecord = getRecord(parentNode);
    Record &newRootRecord = getRecord(newRoot);

    newRootRecord.left = parentNode;
    parentRecord.right = leftSubtree;

    // Update heights
    parentRecord.height = 1 + max(getHeight(parentRecord.left), getHeight(parentRecord.right));
    newRootRecord.height = 1 + max(getHeight(newRootRecord.left), getHeight(newRootRecord.right));

    parentNode = newRoot;
}

void rightRotate(long &pos_node)
{
    long newRoot = getRecord(pos_node).left;
    long &parentNode = pos_node;
    long rightSubtree = getRecord(newRoot).right;

    Record &parentRecord = getRecord(parentNode);
    Record &newRootRecord = getRecord(newRoot);

    newRootRecord.right = parentNode;
    parentRecord.left = rightSubtree;

    // Update heights
    parentRecord.height = 1 + max(getHeight(parentRecord.left), getHeight(parentRecord.right));
    newRootRecord.height = 1 + max(getHeight(newRootRecord.left), getHeight(newRootRecord.right));

    parentNode = newRoot;
}


    void range_search(long pos_node, int low, int high, vector<Record> &result)
    {
        if (pos_node == -1)
        {
            return;
        }
        Record node = getRecord(pos_node);
        if (node.cod >= low && node.cod <= high)
        {
            result.push_back(node);
        }
        if (node.cod >= low)
        {
            range_search(node.left, low, high, result);
        }
        if (node.cod <= high)
        {
            range_search(node.right, low, high, result);
        }
    }

    vector<Record> inorder(long pos_node)
    {
        ifstream file(filename, ios::binary);
        if (!file.is_open())
        {
            cerr << "No se pudo abrir el archivo." << endl;
            return vector<Record>();
        }
        vector<Record> result;
        if (pos_node == -1)
        {
            file.close();
            return result;
        }
        Record node;
        file.seekg(pos_node * sizeof(Record));
        file.read(reinterpret_cast<char *>(&node), sizeof(Record));
        file.close();
        if (node.cod == -1)
        {
            return result;
        }
        vector<Record> leftSubtree = inorder(node.left);
        result.insert(result.end(), leftSubtree.begin(), leftSubtree.end());
        result.push_back(node);
        vector<Record> rightSubtree = inorder(node.right);
        result.insert(result.end(), rightSubtree.begin(), rightSubtree.end());
        return result;
    }
};
