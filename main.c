#include <stdio.h>

#define MAX_ROW  16
#define MAX_COL 16

#define MIN(a,b) ((a) < (b) ? (a) : (b))

#define SWAP(a, b) { \
    Casa temp = a;   \
    a = b;           \
    b = temp;        \
}

typedef struct
{
    int x; // indice x da coordenada
    int y; // indice y da coordenada
    int d; // distancia da coordenada ate o objetivo
} Casa;

Casa get_vizinho(Casa c, int x, int y)
{
    c.x = x;
    c.y = y;
    return c;
}

int is_maze(int i, int j)
{
    return (i >= 0 && j >= 0 && i < MAX_ROW && j < MAX_COL);
}

/**
* Ler a entrada padrao para informar a resposta.
* Usado junto com os comandos do rato.
* @return int in a resposta do comando
*/
int ler_stdin()
{
    fflush(stdout);
    int in;
    scanf("%d", &in);
    return in;
}

/**
* Vira o rato para esquerda.
* @return int in a resposta do comando
*/
int virar_esquerda()
{
    printf("l\n");
    fflush(stdin);
    return ler_stdin();
}

/**
* Vira o rato para direita.
* @return int in a resposta do comando
*/
int virar_direita()
{
    printf("r\n");
    fflush(stdin);
    return ler_stdin();
}

/**
* Move o rato uma casa para frente.
* @return int in a resposta do comando
*/
int andar()
{
    printf("w\n");
    fflush(stdin);
    return ler_stdin();
}

/**
* Faz o rato correr duas casas para frente.
* @return int in a resposta do comando
*/
int correr_pouco()
{
    printf("j\n");
    fflush(stdin);
    return ler_stdin();
}

/**
* Faz o rato correr tres casas para frente.
* @return int in a resposta do comando
*/
int correr()
{
    printf("R\n");
    fflush(stdin);
    return ler_stdin();
}

/**
* Faz o rato correr quatro casas para frente.
* @return int in a resposta do comando
*/
int correr_muito()
{
    printf("s\n");
    fflush(stdin);
    return ler_stdin();
}

/**
* Ativa o sensor de distancia da posicao atual ate o objetivo.
* @return int in a resposta do comando
*/
int sensor_distancia()
{
    printf("d\n");
    fflush(stdin);
    return ler_stdin();
}

void verificar_resposta(int val, char m)
{
    switch (val)
    {
    case 0:
        if (m == 'w')
            fprintf(stderr, "bateu\n");
        break;
    case 1:
        if (m == 'w')
            fprintf(stderr, "andou uma casa\n");
        else if (m == 'j' || m == 'R' || m == 's')
            fprintf(stderr, "andou uma casa e bateu\n");
        break;
    case 2:
        if (m == 'w')
            fprintf(stderr, "encontrou o objetivo\n");
        else if (m == 'j')
            fprintf(stderr, "andou duas casas\n");
        else if (m == 'R')
            fprintf(stderr, "andou uma casa e bateu\n");
        else if (m == 's')
            fprintf(stderr, "andou duas casas e bateu\n");
        break;
    case 3:
        if (m == 'R')
            fprintf(stderr, "andou tres casas\n");
        else if (m == 's')
            fprintf(stderr, "andou tres casas e bateu\n");
        break;
    case 4:
        if (m == 's')
            fprintf(stderr, "andou quatro casas\n");
        break;
    case 5:
        if(m == 'r')
            fprintf(stderr, "direita\n");
        else if(m == 'l')
            fprintf(stderr, "esquerda\n");
        else if(m == 'R')
            fprintf(stderr, "direita direita\n");
    case -1:
        fprintf(stderr, "sensor quebrado\n");
        break;
    }
}

// ------ corrigir ------ //
void flood_start(int maze[MAX_ROW][MAX_COL], int x, int y, int d)
{
    /* Se fora dos limites do labirinto */
    if (!is_maze(x, y))
        return;

    /* Se ja foi visitado */
    if (maze[x][y] != -1)
        return;

    // Armazena a distancia da coordenada atual ate o objetivo
    maze[x][y] = d;

    Casa current = {x, y, d};

    // Casa do vizinho da frente
    Casa v_frente = get_vizinho(current, x, y+1);

    if(v_frente.d != 0)
    {
        // Call flood for front and left neighbors
        flood_start(maze, v_frente.x, v_frente.y, d - 1);
    }
    else
    {
        return;
    }

    // Casa do vizinho da direita
    Casa v_direita = get_vizinho(current, x+1, y);
    if(v_direita.d != 0)
    {
        // Call flood for right neighbor
        flood_start(maze, v_direita.x, v_direita.y, d - 1);
    }
    else
    {
        return;
    }
}

// ------ corrigir ------ //
void flood_end(int maze[MAX_ROW][MAX_COL], int x, int y, int d)
{
    /* Fora dos limites do labirintos */
    if (!is_maze(x, y))
        return;

     /* Se ja foi visitado */
    if (maze[x][y] != -1)
        return;

    // Armazena a distancia da coordenada atual ate o objetivo
    maze[x][y] = d;

    Casa current = {x, y, d};

    // Casa do vizinho da esquerda
    Casa v_esquerda = get_vizinho(current, x-1, y);
    if(v_esquerda.d != 0)
    {
        flood_end(maze, v_esquerda.x, v_esquerda.y, d - 1);
    }
    else
    {
        return;
    }

    // Casa dos vizinhos de tras
    Casa v_tras = get_vizinho(current, x, y-1);
    if(v_tras.d != 0)
    {
        flood_end(maze, v_tras.x, v_tras.y, d - 1);
    }
    else
    {
        return;
    }
}

void sort_vizinhos(Casa* neighbors, int size)
{
    /* Pela distancia */
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (neighbors[j].d > neighbors[j + 1].d)
            {
                SWAP(neighbors[j], neighbors[j + 1]);
            }
        }
    }
}

void mergeTwoSortedArrays(Casa arr1[], Casa arr2[], int size1, int size2, Casa arr3[])
{
    int i = 0, j = 0, k = 0;

    while (i<size1 && j <size2)
    {
        if (arr1[i].d < arr2[j].d)
            arr3[k++] = arr1[i++];
        else
            arr3[k++] = arr2[j++];
    }

    while (i < size1)
        arr3[k++] = arr1[i++];

    while (j < size2)
        arr3[k++] = arr2[j++];
}

void virar_rato(int direcao_antiga, int bussula)
{
    /* Se o rato estava no Norte */
    if(direcao_antiga == 0)
    {
        /* O rato foi para o Leste */
        if(bussula == 1)
        {
            virar_direita();
            verificar_resposta(5, 'r');
        }
        /* O rato foi para o Oeste */
        else if(bussula == 2)
        {
            virar_esquerda();
            verificar_resposta(5, 'l');
        }
        /* O rato foi para o Sul*/
        else if(bussula == 3)
        {
            virar_direita();
            virar_direita();
            verificar_resposta(5, 'R');
        }
    }
    /* Se o rato estava no Sul */
    else if(direcao_antiga == 3)
    {
        /* O rato foi para o Leste */
        if(bussula == 1)
        {
            virar_esquerda();
            verificar_resposta(5, 'l');
        }
        /* O rato foi para o Oeste */
        else if(bussula == 2)
        {
            virar_direita();
            verificar_resposta(5, 'r');
        }
        /* O rato foi para o Norte */
        else if(bussula == 0)
        {
            virar_direita();
            virar_direita();
            verificar_resposta(5, 'R');
        }
    }
    /* Se o rato estava no Leste */
    else if (direcao_antiga == 1)
    {
        /* O rato foi para o Norte */
        if (bussula == 0)
        {
            virar_esquerda();
            verificar_resposta(5, 'l');
        }
        /* O rato foi para o Sul */
        else if (bussula == 3)
        {
            virar_direita();
            verificar_resposta(5, 'r');
        }
        /* O rato foi para o Oeste */
        else if (bussula == 2)
        {
            virar_direita();
            virar_direita();
            verificar_resposta(5, 'R');
        }
    }
    /* Se o rato estava no Oeste */
    else if (direcao_antiga == 2)
    {
        /* O rato foi para o Norte*/
        if (bussula == 0)
        {
            virar_direita();
            verificar_resposta(5, 'r');
        }
        /* O rato foi para o Sul */
        else if (bussula == 3)
        {
            virar_esquerda();
            verificar_resposta(5, 'l');
        }
        /* O rato foi para o Leste */
        else if (bussula == 1)
        {
            virar_direita();
            virar_direita();
            verificar_resposta(5, 'R');
        }
    }
}

int atualizar_bussula(Casa c, int x, int y)
{
    int bussula = 0;
    /* Se o rato esta se movendo no eixo x */
    if(c.x == x && c.y != y) // Observe que para se mover no eixo x, as colunas se alteram
        /* O rato tentou ir para o Leste */
        if(y < c.y)
            bussula = 2;
    /* O rato tentou ir para o Oeste */
        else
            bussula = 1;
    /* Se o rato esta se movendo no eixo y */
    else if(c.x != x && c.y == y) // Observe que para se mover no eixo y, as linhas se alteram
        /* O rato tentou ir para o Sul */
        if(x < c.x)
            bussula = 3;
    /* O rato tentou ir para o Norte */
        else
            bussula = 0;
    return bussula;
}

void atualizar_distancia(Casa path[MAX_ROW*MAX_COL], int maze[MAX_ROW][MAX_COL], int row, int col, int m, int size)
{
    if(maze[row][col] == 0 && m != 4)
    {
        for(int i = 0; i < size; i++)
        {
            path[i].d = path[i].d+1;
        }
    }
    if(maze[row][col] == 1 && path[size].d == 1)
    {
        for(int i = 0; i < size-1; i++)
        {
            path[i].d = path[i].d+1;
        }
    }
}

void flood_fill(Casa path[MAX_ROW*MAX_COL], int maze[MAX_ROW][MAX_COL], int visited[MAX_ROW][MAX_COL], int row, int col, int d)
{
    Casa queue[MAX_ROW * MAX_COL]; // Fila de vizinhos da posicao atual

    int front = 0; // Frente da fila

    int rear = 0; // Final da fila

    Casa c = {row, col, d}; // A casa inicial de onde o rato parte
    queue[rear++] = c;

    /* Bussula do rato */
    int bussula = 0; // (0 = norte, 1 = leste, 2 = oeste, 3 = sul)

    int m = 1; // Movimento do rato

    int is_inicio = 1;

    int path_size = 0; // Contador do tamanho do caminho

    /* Enquanto a fila nao estiver vazia */
    while (front != rear)
    {
        /* Se o rato se moveu, remover a casa da frente da fila */
        if(m == 1)
        {
            c = queue[front++]; // Pega a primeira casa da fila
        }
        /* Se nao conseguiu se mover, pega o proximo viizinho */
        else if(m == 0)
        {
            /* Atualiza a fila com os proximos vizinhos */
            for(int i = front; i < rear; i++)
            {
                queue[i] = queue[i+1];
            }
            c = queue[front];
            if(front != rear-1)
            {
                rear--;
            }
        }

        /* Direcao anterior da bussula */
        int direcao_antiga = bussula;

        /* Pega a direcao atual do rato */
        bussula = atualizar_bussula(c, row, col);

        if(direcao_antiga != bussula)
        {
            /* Muda a direcao do rato */
            virar_rato(direcao_antiga, bussula);
        }

        if(!is_inicio)
        {
            /* Move o rato */
            m = andar();
            verificar_resposta(m, 'w');
        }

        /* Se a casa estiver no labirinto e nao tiver sido visitada e for possivel se mover ate ela */
        if (is_maze(row, col) && visited[row][col] == -1 && m == 1)
        {
            /* Atualiza a posicao atual da casa */
            row = c.x;
            col = c.y;

            // printf("\Fim da fila (%d): %d (%dx%d)\n", front-1, queue[front-1].d, c.x, c.y);

            /* Pega o vizinho do Leste */
            Casa v_leste = {c.x+1, c.y, maze[row+1][col]};
            //printf("leste: (%dx%d) %d\n", row+1, col, v_leste.d);

            /* Pega o vizinho do Oeste */
            Casa v_oeste = {c.x-1, c.y, maze[row-1][col]};
            //printf("oeste: (%dx%d) %d\n", row-1, col, v_oeste.d);

            /* Pega o vizinho do Norte */
            Casa v_norte = {c.x, c.y+1, maze[row][col+1]};
            //printf("norte: (%dx%d) %d\n", row, col+1, v_norte.d);

            /* Pega o vizinho do Sul */
            Casa v_sul = {c.x, c.y-1, maze[row][col-1]};
            //printf("sul: (%dx%d) %d\n", row, col-1, v_sul.d);

            /* Marca a casa como visitada */
            visited[row][col] = 1;

            /* Armazena a casa no caminho */
            path[path_size] = c;

            /* Atualiza as distancias das casas do caminho */
            atualizar_distancia(path, maze, row, col, m, path_size);

            path_size++;

            Casa vizinhos_eixo_x[2];
            int i = 0;

            Casa vizinhos_eixo_y[2];
            int j = 0;

            /* Pega os vizinhos nao-visitados */
            if(v_leste.d != -1 && visited[v_leste.x][v_leste.y] != 1)
            {
                vizinhos_eixo_x[i] = v_leste;
                i++;
            }

            if(v_oeste.d != -1 && visited[v_oeste.x][v_oeste.y] != 1)
            {
                vizinhos_eixo_x[i] = v_oeste;
                i++;
            }

            if(v_norte.d != -1 && visited[v_norte.x][v_norte.y] != 1)
            {
                vizinhos_eixo_y[j] = v_norte;
                j++;
            }

            if(v_sul.d != -1 && visited[v_sul.x][v_sul.y] != 1)
            {
                vizinhos_eixo_y[j] = v_sul;
                j++;
            }

            /* Classifica as casas vizinhas pela distancia mais curta */
            sort_vizinhos(vizinhos_eixo_x, i);
            sort_vizinhos(vizinhos_eixo_y, j);

            int v_tamanho = i+j;

            Casa vizinhos[v_tamanho];

            mergeTwoSortedArrays(vizinhos_eixo_x, vizinhos_eixo_y, i, j, vizinhos);

            /* Stores the current cell's neighbors in the queue */
            for (int k = 0; k < v_tamanho; k++)
            {
                //printf("q[rear = %d]: %d <- n(k = %d): %d ", rear, queue[rear].d, k, vizinhos[k].d);
                queue[rear++] = vizinhos[k];
            }

            /* Prioriza o primeiro vizinho da fila */
            if(v_tamanho > 1)
            {
                rear = rear - 1;
            }

            if(is_inicio)
            {
                /* Move o rato */
                m = andar();
                verificar_resposta(m, 'w');
                is_inicio = 0;
            }
            /*
                        printf("Queue: \n");
                        for (int i = front; i < rear; i++)
                        {
                            printf("q[front = %d]: %d visited = %d ", i, queue[i].d, visited[queue[i].x][queue[i].y]);
                        }
                        printf("\n");
            */
        }
    }
}

int main()
{
    /* Labirinto */
    int maze[MAX_ROW][MAX_COL];

    /* Mapa de visitados */
    int visited[MAX_ROW][MAX_COL];

    /* Caminho para o objetivo */
    Casa path[MAX_ROW*MAX_COL];

    /* Inicializa labirinto e mapa */
    for (int i = -1; i < MAX_ROW; i++)
    {
        for (int j = -1; j < MAX_COL; j++)
        {
            maze[i][j] = -1; // -1  significa nao-explorado
            visited[i][j] = -1; // -1 significa nao-visitado
        }
    }
    /* Inicializa caminho */
    for (int i = 0; i < MAX_ROW*MAX_COL; i++)
    {
        path[i].d = -1;
    }
    /*
        int values[MAX_ROW][MAX_COL] = {{6, 5, 4}, {3, 2, 3}, {0, 1, 4}};

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                maze[i][j] = values[i][j];
            }
        }
    */

    /* Distancia ate o objetivo */
    int d = sensor_distancia();

    /* Inicializa o labirinto com distancias ate o objetivo */
    flood_start(maze, 0, 0, d);
    flood_end(maze, d, d, d);

    // Flood fill da origem ate o objetivo
    flood_fill(path, maze, visited, 0, 0, d);

    /* */
    printf("\nMaze: \n");
    //Print maze
    for (int i = MAX_ROW - 1; i >= 0; i--)
    {
        for (int j = 0; j < MAX_COL; j++)
        {
            printf("%3d ", maze[i][j]);
        }
        printf("\n");
    }

    printf("\nMap: \n");
    //Print map visited
    for (int i = MAX_ROW - 1; i >= 0; i--)
    {
        for (int j = 0; j < MAX_COL; j++)
        {
            printf("%3d ", visited[i][j]);
        }
        printf("\n");
    }

    //Print path
    printf("Path: ");
    for(int i = 0; i < MAX_ROW*MAX_COL; i++)
    {
        if(path[i].d != -1)
        {
            printf("(%d) %d ", i, path[i].d);
        }
    }
    printf("\n");

    return 0;
}
