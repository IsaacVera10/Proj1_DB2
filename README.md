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

 - Para la inserción
   
### AVL_FILE
### HASH_EXTENDIBLE
## Autores

|                     **Salvador  Olivares**                   |                                 **Jeffrey**                                 |                       **Diego Rivadeneyra**                 |  **Isaac Vera Romero** |
|:---------------------------------------------------------------------------------:|:-------------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------:|:----:|
|           ![Sael](https://avatars.githubusercontent.com/u/83047305?v=4)            |      ![Juan Diego Castro](https://avatars.githubusercontent.com/u/79115974?v=4)       |              ![Diego Rivadeneyra](https://avatars.githubusercontent.com/u/55153225?v=4)              | ![Isaac Vera Romero](https://avatars.githubusercontent.com/u/67709665?v=4) |                                             
| <a href="https://github.com/Sael-init" target="_blank">`https://github.com/Sael-init`</a> | <a href="https://github.com/AlexandroChamochumbi" target="_blank">`https://github.com/AlexandroChamochumbi`</a> | <a href="https://github.com/diegorivadeneyra" target="_blank">`https://github.com/diegorivadeneyra`</a> | <a href="https://github.com/IsaacVera10" target="_blank">`https://github.com/IsaacVera10`</a>|


