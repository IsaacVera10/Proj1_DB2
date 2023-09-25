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


### SEQUENTIAL_FILE

<div align="center">
    (https://github.com/IsaacVera10/Proj1_DB2/blob/main/sequential.PNG)
</div>

 - INSERTION
       El insertion se tomaron los diferentes caso. Primero se toma el caso base cuando el archivo esta vacio entonces se inserta en el .dat original. Apartir de este se hara comparaciones de las keys si es menor se              mandara al auxiliar y se enlazara, en caso sea mayor solo se verificara que si es el mayor que el elemento final que tendra un -1 y se insertara luego de este en el .dat original. Cuando se inserte un valor entre          diferentes valores se verificara el anterior mas proximo del .dat original, seguido de eso se verificara si el enlaze es a un registro de el aux o el .dat. Si es en el .dat solo se inserta al final del auxiliar y          se enlaza con el anterior, caso contrario, si enlaza al auxiliar se ira a ese valor del auxiliar y se buscara su enlace y sse verificara hasta encontrar el menor mas cercano para poder insertar nuestra key al final        del auxiliar y enlazarlo a este nuevo menor.
   
 - SEARCH
       
        
   
### AVL_FILE
### HASH_EXTENDIBLE
## Autores

|                     **Salvador  Olivares**                   |                                 **Jeffrey**                                 |                       **Diego Rivadeneyra**                 |  **Isaac Vera Romero** |
|:---------------------------------------------------------------------------------:|:-------------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------:|:----:|
|           ![Sael](https://avatars.githubusercontent.com/u/83047305?v=4)            |      ![Juan Diego Castro](https://avatars.githubusercontent.com/u/79115974?v=4)       |              ![Diego Rivadeneyra](https://avatars.githubusercontent.com/u/55153225?v=4)              | ![Isaac Vera Romero](https://avatars.githubusercontent.com/u/67709665?v=4) |                                             
| <a href="https://github.com/Sael-init" target="_blank">`https://github.com/Sael-init`</a> | <a href="https://github.com/AlexandroChamochumbi" target="_blank">`https://github.com/AlexandroChamochumbi`</a> | <a href="https://github.com/diegorivadeneyra" target="_blank">`https://github.com/diegorivadeneyra`</a> | <a href="https://github.com/IsaacVera10" target="_blank">`https://github.com/IsaacVera10`</a>|


