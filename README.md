# Proyecto_1 BD2

## Introducción 

Para este proyecto se implementó un sistema de gestión de base de datos (DBMS) que permite la manipulación de **metadata** en una base de datos usando estrategias de organización de archivos. Para ello, se implementó un parser que permite la interpretación de sentencias SQL, y se implementaron tres estrategias de organización de archivos: Sequential File, Extendible Hashing y AVL.

### Objetivos
#### Principal
- Implementar un sistema de gestión de base de datos (DBMS) y manipular un DataSet usando estrategias de organización de archivos.

#### Secundarios
- Implementar un parser que permita la interpretación de sentencias SQL.
- Comparar las estrategias de organización de archivos implementadas e identificar ventajas y desventajas de cada una.
- Implementar un GUI en QT para una mejor visualización del proyecto.

### Organización de Archivos

Las estrategias usadas para este proyecto son las siguientes:

- Sequential File
- Extendible Hashing
- AVL

### Funciones implementadas

Cada estrategia debe tener las siguientes funciones:

- vector<Registro> search(T key)
- vector<Registro> rangeSearch(T begin-key, T end-key)
- bool add(Registro registro)
- bool remove(T key)

### DataSet
Hemos usado un dataset que contiene información de los productos que ofrece una tienda de ropas.

A continuación, explicamos los atributos del DataSet previamente mencionado.

```c++
struct MovieRecord {
    char id[7];
    char prenda[10];
    char genero[1];
    float precio;
    char stock[4];
    char marca[7];  
}
```

|       Campo       |                         Descripción                          |
|:-----------------:|:------------------------------------------------------------:|
|   ```id```    | Id único de cada registro, sirve para identificar una tupla. |
| ```prenda``` |         Nombre del producto que ofrece la tienda         |
|    ```genero```    |                     Puede ser 'M' o 'F'                     |
|   ```precio```    |              Dato float que representa el costo de cada producto               |
| ```stock``` |               Valor entero que representa la cantidad que existe del producto              |
|   ```marca```   |      Nombre de la marca del producto     |


## SEQUENTIAL_FILE

<div align="center">
    <img src="/sequential.PNG" />
</div>

 - INSERTION:
       El insertion se tomaron los diferentes caso. Primero se toma el caso base cuando el archivo esta vacio entonces se inserta en el .dat original. Apartir de este se hara comparaciones de las keys si es menor se              mandara al auxiliar y se enlazara, en caso sea mayor solo se verificara que si es el mayor que el elemento final que tendra un -1 y se insertara luego de este en el .dat original. Cuando se inserte un valor entre          diferentes valores se verificara el anterior mas proximo del .dat original, seguido de eso se verificara si el enlaze es a un registro de el aux o el .dat. Si es en el .dat solo se inserta al final del auxiliar y          se enlaza con el anterior, caso contrario, si enlaza al auxiliar se ira a ese valor del auxiliar y se buscara su enlace y sse verificara hasta encontrar el menor mas cercano para poder insertar nuestra key al final        del auxiliar y enlazarlo a este nuevo menor.
   
 - SEARCH:
       En la busqueda se implementa una funcion para obtener un par de elementos uno en el cual se esta tomando el record que se busca y el otro para almacenar la posicicon exacta donde se        encuentra el record. En el caso del sequential los enlaces se almacenan en un pair que tendra un int y un char. Esta funcion buscara unicamente en el .dat original, ya que, este            siempre se mantendra ordenado y si no lo encuentra buscara en menor mas cercano para asi hacer la busqueda en el aux.
   
 - RANGE_SEARCH: 
       Iniciamos haciendo un rebuild a nuestros registros y asi tener nuestro.dat con todos los registros y ordenados. Con ello, simplemente buscamos la key inicial y la key final. Finalmente, se hace un recorrido por los enlazes y se va haciendo push de los registros en un vector que luego sera mostrado.

- REBUILD:
      La funcion rebuild simplemente toma el Header y comienza a hacer todo el recorrido de enlazes almacenando cada registro en un nuevo .dat que almacenara todo de forma ordenada y al llegar al ultimo se mandara todo al .dat original sobreescribiendolo. Finalmente se vacia el auxiliar para volver ser usado.
        
   
### AVL_FILE
    -FIND :
        La función find en el AVL File se encarga de buscar un registro por su ID en el árbol AVL. Recorre el árbol comparando el id de cada nodo y en caso no se encuentre retornará vacío.
    -RANGE_SEARCH:
        Busca registros dentro de un rango de IDs. La función devuelve un vector que contiene todos los registros que se encuentran dentro del rango especificado.
    -In-order:
        La función inorder en el AVL File realiza un recorrido en orden del árbol AVL y devuelve los registros en ese orden.
        La función recorre todo el árbol en orden ascendente según los IDs y devuelve un vector que contiene los registros en ese orden.
        
### HASH_EXTENDIBLE
## Autores

|                     **Salvador  Olivares**                   |                                 **Jeffrey**                                 |                       **Diego Rivadeneyra**                 |  **Isaac Vera Romero** |
|:---------------------------------------------------------------------------------:|:-------------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------:|:----:|
|           ![Sael](https://avatars.githubusercontent.com/u/83047305?v=4)            |      ![Juan Diego Castro](https://avatars.githubusercontent.com/u/79115974?v=4)       |              ![Diego Rivadeneyra](https://avatars.githubusercontent.com/u/55153225?v=4)              | ![Isaac Vera Romero](https://avatars.githubusercontent.com/u/67709665?v=4) |                                             
| <a href="https://github.com/Sael-init" target="_blank">`https://github.com/Sael-init`</a> | <a href="https://github.com/AlexandroChamochumbi" target="_blank">`https://github.com/AlexandroChamochumbi`</a> | <a href="https://github.com/diegorivadeneyra" target="_blank">`https://github.com/diegorivadeneyra`</a> | <a href="https://github.com/IsaacVera10" target="_blank">`https://github.com/IsaacVera10`</a>|


