#include <iostream> //para introducir los datos y leer cin y cout
#include <cstdlib> // random
#include <ctime> //usa la hora del sistema, usa como para la semilla para generar numero aleatorios
using namespace std;

// Crear laberinto dinámico //dibuja el laberinto
char** crearLaberinto(int filas, int columnas) {
    char** lab = new char*[filas];
    for (int i = 0; i < filas; i++) {
        lab[i] = new char[columnas]; //itera la fila para meter las columnas 
    }
    return lab;
}

// Liberar memoria // limpia lo ingresado para el laberinto
void liberarLaberinto(char** lab, int filas) {
    for (int i = 0; i < filas; i++) {
        delete[] lab[i];
    }
    delete[] lab;
}

// Imprimir laberinto // imprime el laberinto base
void imprimirLaberinto(char** lab, int filas, int columnas) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            cout << lab[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

// Verifica si es válido para DFS de generación //revisa si aun no se visito una posicion y si esta todo dentro del rango
bool esValido(int x, int y, int filas, int columnas, char** lab) {
    return x > 0 && x < filas - 1 && y > 0 && y < columnas - 1 && lab[x][y] == '#';
}

// Mezclar direcciones para aleatoriedad //comineza el laberinto de forma aleatoria, busca direcciones
void mezclar(int arr[]) {
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4; //r es random // %4 varia entre 0 1 2 3 
        swap(arr[i], arr[r]); //para intercambio de direcciones
    }
}

// DFS para generar el laberinto
void dfsGenerar(int x, int y, int filas, int columnas, char** lab) {
    lab[x][y] = '*'; //asterisco

    int dx[4] = {0, 0, -2, 2}; //direcciones posibles de a dos casillas 
    int dy[4] = {-2, 2, 0, 0};
    int dir[4] = {0, 1, 2, 3};
    mezclar(dir); //llama a la funcion [mezclar] para pasar las direcciones posibles

    for (int i = 0; i < 4; i++) { //itera para saber el nuevo camino por recorrer
        int nx = x + dx[dir[i]];
        int ny = y + dy[dir[i]];
        if (esValido(nx, ny, filas, columnas, lab)) {
            lab[x + dx[dir[i]] / 2][y + dy[dir[i]] / 2] = '*'; //si es un muro y si esta dentro del tablero se crea un camino de *
            dfsGenerar(nx, ny, filas, columnas, lab); // se llama de forma recursiva hasta que ya no tenga muros por convertir en caminos
        }
    }
}

// Generar laberinto completo
void generarLaberinto(int filas, int columnas, char** lab) {
    for (int i = 0; i < filas; i++) { //rango para iterar
        for (int j = 0; j < columnas; j++) {
            lab[i][j] = '#'; // laberinto solo hecho con muros
        }
    }

    dfsGenerar(1, 1, filas, columnas, lab); // es 1 y 1 porqu eson x e y para que comience en una posicion y esa es la de E
    lab[1][1] = 'E';
    lab[filas - 2][columnas - 2] = 'S'; 
}

// Verifica si es válido para DFS de búsqueda// dfs usa esta funcion, le llama a esta funcion
bool esCaminoValido(int x, int y, int filas, int columnas, char** lab, bool** visitado) {
    return x >= 0 && x < filas && y >= 0 && y < columnas && // se asegura que no salga del tablero
           (lab[x][y] == '*' || lab[x][y] == 'S') && !visitado[x][y]; //analiza si es un camino, si es S para no seguir o si no esta visitado, sigue
}

// DFS para encontrar camino de E a S
bool dfsEncontrarCamino(int x, int y, char** lab, int filas, int columnas, bool** visitado) {
    if (lab[x][y] == 'S') return true; //caso base, cuando llega a la S deja de buscar

    visitado[x][y] = true; //hace que se aconvierta en visitado y pasa de falso o verdadero

    int dx[4] = {0, 0, -1, 1}; // posibles direcciones de a 1
    int dy[4] = {-1, 1, 0, 0};

    for (int i = 0; i < 4; i++) { //itera para saber el nuevo camino por recorrer// itera dentro de las posibles direcciones
        int nx = x + dx[i]; //se crea nuevo para ir recorriendo
        int ny = y + dy[i]; // se suma la actual de E + la posible direccion

        if (esCaminoValido(nx, ny, filas, columnas, lab, visitado)) { //si es valido pasa  ala sioguienete condicion
            if (dfsEncontrarCamino(nx, ny, lab, filas, columnas, visitado)) {
                // Marca el camino con 'o'
                if (lab[x][y] != 'E') lab[x][y] = 'o'; // si es diferente a E entoneces se marca "o"
                return true; //se usa para terminar la busqueda 
            }
        }
    }

    return false; //  Significa que desde esa posición NO hay camino hacia la salida.
}

// Buscar 'E' y lanzar DFS desde ahí
void resolverCaminoConDFS(char** lab, int filas, int columnas) {
    int startX = -1, startY = -1; //deja en claro de que no hallo tdv a E

    // Buscar la 'E'
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (lab[i][j] == 'E') {
                startX = i;
                startY = j;
                break;
            }
        }
        if (startX != -1) break;
    }

    // Crear matriz de visitados //es una matriz de las casillas visitadas o sea true o false
    bool** visitado = new bool*[filas];
    for (int i = 0; i < filas; i++) {
        visitado[i] = new bool[columnas];
        for (int j = 0; j < columnas; j++) {
            visitado[i][j] = false;
        }
    }

    dfsEncontrarCamino(startX, startY, lab, filas, columnas, visitado);

    for (int i = 0; i < filas; i++) { // se elimina para liberar espacio
        delete[] visitado[i]; //lo que se elimina es visitados el array puntero de punteros
    }
    delete[] visitado;
}

// Función principal
int main() {
    int filas, columnas;
    cout << "Tamanho del laberinto (minimo 5x5). Debe de ser numero impar: ";
    cin >> filas >> columnas;

    if (filas < 5 || columnas < 5) {
        cout << "El tamanho minimo es 5x5 y numero impar.\n";
        return 1;
    }

    srand(time(0)); // para que cada que se ejecute siempre sean aleatorios los nuemros
    //usa el tiemporeal de la compu para usar otras numeros en aleatorio

    char** laberinto = crearLaberinto(filas, columnas);
    generarLaberinto(filas, columnas, laberinto);

    cout << "\nLaberinto generado:\n";
    imprimirLaberinto(laberinto, filas, columnas);

    resolverCaminoConDFS(laberinto, filas, columnas);

    cout << "Camino encontrado con DFS marcado con 'o':\n";
    imprimirLaberinto(laberinto, filas, columnas);

    liberarLaberinto(laberinto, filas);
    return 0;
}
