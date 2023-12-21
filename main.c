#include <stdio.h>

#define MAX_ROW 22
#define MAX_COL 22
#define MAX (MAX_ROW*MAX_COL)

#define D_TESTE 10

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
    int in;
    scanf("%d", &in);
    fflush(stdin);
    return in;
}

/**
* Vira o rato para esquerda.
* @return int in a resposta do comando
*/
int virar_esquerda()
{
    printf("l\n");
    fflush(stdout);
    return ler_stdin();
}

/**
* Vira o rato para direita.
* @return int in a resposta do comando
*/
int virar_direita()
{
    printf("r\n");
    fflush(stdout);
    return ler_stdin();
}

/**
* Move o rato uma casa para frente.
* @return int in a resposta do comando
*/
int andar()
{
    printf("w\n");
    fflush(stdout);
    return ler_stdin();
}

/**
* Faz o rato correr duas casas para frente.
* @return int in a resposta do comando
*/
int correr_pouco()
{
    printf("j\n");
    fflush(stdout);
    return ler_stdin();
}

/**
* Faz o rato correr tres casas para frente.
* @return int in a resposta do comando
*/
int correr()
{
    printf("R\n");
    fflush(stdout);
    return ler_stdin();
}

/**
* Faz o rato correr quatro casas para frente.
* @return int in a resposta do comando
*/
int correr_muito()
{
    printf("s\n");
    fflush(stdout);
    return ler_stdin();
}

/**
* Ativa o sensor de distancia da posicao atual ate o objetivo.
* @return int in a resposta do comando
*/
int sensor_distancia()
{
    printf("d\n");
    fflush(stdout);
    return ler_stdin();
}

int verificar_resposta(int m, char c)
{
    switch (m)
    {
    case 0:
        if (c == 'w')
            fprintf(stderr, "bateu\n");
        break;
    case 1:
        if (c == 'w')
            fprintf(stderr, "andou uma casa\n");
        else if (c == 'j' || c == 'R' || c == 's')
            fprintf(stderr, "andou uma casa e bateu\n");
        break;
    case 2:
        if (c == 'w')
            fprintf(stderr, "encontrou o objetivo\n");
        else if (c == 'j')
            fprintf(stderr, "andou duas casas\n");
        else if (c == 'R')
            fprintf(stderr, "andou uma casa e bateu\n");
        else if (c == 's')
            fprintf(stderr, "andou duas casas e bateu\n");
        break;
    case 3:
        if (c == 'R')
            fprintf(stderr, "andou tres casas\n");
        else if (c == 's')
            fprintf(stderr, "andou tres casas e bateu\n");
        break;
    case 4:
        if (c == 's')
            fprintf(stderr, "andou quatro casas\n");
        break;
    case 5:
        if(c == 'r')
            fprintf(stderr, "direita\n");
        else if(c == 'l')
            fprintf(stderr, "esquerda\n");
        else if(c == 'R')
            fprintf(stderr, "direita direita\n");
        break;
    case -1:
        if(c == 'd')
            fprintf(stderr, "sensor quebrado\n");
        break;
    }
    return m;
}

void sort_vizinhos(Casa* v, int tamanho)
{
    /* Pela distancia */
    for (int i = 0; i < tamanho - 1; i++)
    {
        for (int j = 0; j < tamanho - i - 1; j++)
        {
            if (v[j].d > v[j + 1].d)
            {
                SWAP(v[j], v[j + 1]);
            }
        }
    }
}

void juntar_vizinhos(Casa arr1[], Casa arr2[], int size1, int size2, Casa arr3[])
{
    int i = 0, j = 0, k = 0;

    while (i < size1 && j < size2)
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

/* ----- verificar ----- */
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

/* ----- verificar ----- */
int olhar_bussula(Casa c, int x_antigo, int y_antigo)
{
    int bussula = 0; // Norte

    /* Se for o inicio, o rato esta olhando para o Norte */
    if(c.x == 0 && c.y == 0)
        return bussula;

    /* Se o rato esta se movendo no eixo x */
    if(c.x == x_antigo && c.y != y_antigo)  // Observe que para se mover no eixo x, as colunas se alteram
    {
        /* O rato tentou ir para o Norte */
        if(c.y > y_antigo)
            bussula = 0;
        /* O rato tentou ir para o Sul */
        else
            bussula = 3;
        /* Se o rato esta se movendo no eixo y */
    }
    else if(c.x != x_antigo && c.y == y_antigo) // Observe que para se mover no eixo y, as linhas se alteram
    {
        /* O rato tentou ir para o Leste */
        if(c.x > x_antigo)
            bussula = 1;
        /* O rato tentou ir para o Oeste */
        else
            bussula = 2;
    }
    return bussula;
}

void atualizar_distancia(Casa path[MAX_ROW * MAX_COL], int maze[MAX_ROW][MAX_COL], int x, int y, int m, int size)
{
    if(maze[x][y] == 0 && m != 2)
    {
        for(int i = 0; i < size; i++)
        {
            path[i].d = path[i].d+1;
        }
    }
    if(maze[x][y] == 1 && path[size].d == 1)
    {
        for(int i = 0; i < size-1; i++)
        {
            path[i].d = path[i].d+1;
        }
    }
}


void flood_fill(Casa path[MAX_ROW * MAX_COL], int maze[MAX_ROW][MAX_COL], int visited[MAX_ROW][MAX_COL], int x, int y, int d)
{
    Casa q[MAX_ROW * MAX_COL]; // Fila de vizinhos da posicao atual

    int front = 0; // Frente da fila
    int rear = 0; // Final da fila

    Casa c = {x, y, d}; // A casa inicial de onde o rato parte
    q[rear++] = c; // Coloca a casa na frente da fila

    int bussula = 0; // (0 = norte, 1 = leste, 2 = oeste, 3 = sul)
    /* Coordenadas antigas do rato */
    int x_antigo = x;
    int y_antigo = y;

    int m = 1; // No inicio, como o rato esta no labirinto, considera que ele se moveu

    int path_size = 0; // Contador do tamanho do caminho

    /* Enquanto a fila nao estiver vazia */
    while (front != rear)
    {
        /* ------------------------ teste ------------------------ */
        // Neste teste eu imprimo o tamanho da fila
        printf("\t front:%d rear:%d \n", front, rear);
        /* -------------------------------------------------------- */

        /* Se o rato se moveu, pega a casa vizinha na frente da fila */
        if(m == 1)
        {
            c = q[front];
        }
        /* Se nao conseguiu se mover, pega a proxima casa da fila nao-visitada */
        else
        {
            front = (front + 1) % MAX;
            c = q[front];
        }

        /* ------------------------ teste ------------------------ */
        printf("\t atual: (%dx%d):%d \n", c.x, c.y, c.d);
        /* -------------------------------------------------------- */

        /* Atualiza as coordenadas com a posicao da casa */
        x = c.x;
        y = c.y;

        int direcao_antiga = bussula; // Direcao antiga do rato

        /* Pega a direcao atual do rato */
        bussula = olhar_bussula(c, x_antigo, y_antigo); // No inicio sera 0

        /* ------------------------ teste ------------------------ */
        // Neste teste eu imprimo a direcao atual do rato e a direcao antiga
        printf("\t bussula(%dx%d):%d antiga(%dx%d):%d \n", c.x, c.y, bussula, x_antigo, y_antigo, direcao_antiga);
        /* -------------------------------------------------------- */

        /* Se o robo esta olhando para outra direcao ou bateu em uma parede */
        if(direcao_antiga != bussula || m == 0)
        {
            virar_rato(direcao_antiga, bussula);

            /* ------------------------ teste ------------------------*/
            // Neste teste eu imprimo onde o rato esta olhando
            printf("\t (%dx%d): olhando para: %d tenta se mover para: (%dx%d):%d\n", x_antigo, y_antigo, bussula, q[front].x, q[front].y, q[front].d);
            /* -------------------------------------------------------- */

            m = verificar_resposta(andar(), 'w');
        }

        /* Se a casa estiver no labirinto e nao tiver sido visitada e for possivel se mover ate ela */
        if (is_maze(x, y) && visited[x][y] == -1 && m == 1)
        {
            x_antigo = x;
            y_antigo = y;

            visited[x][y] = 1; // Marca a casa como visitada
            front = (front + 1) % MAX; // Remove da frente da fila

            //printf("front: %d\n", front);
            path[path_size] = c; // Armazena a casa no caminho

            /* Atualiza as distancias das casas do caminho */
            atualizar_distancia(path, maze, x, y, m, path_size);
            path_size++;

            /* Pega os vizinhos da casa */
            Casa v_leste = {c.x+1, c.y, maze[x+1][y]};
            Casa v_oeste = {c.x-1, c.y, maze[x-1][y]};
            Casa v_norte = {c.x, c.y+1, maze[x][y+1]};
            Casa v_sul = {c.x, c.y-1, maze[x][y-1]};

            int i = 0, j = 0;
            Casa v_em_x[2]; // Leste e Oeste
            Casa v_em_y[2]; // Norte e Sul

            /* Se o vizinho esta no labirinto e nao foi visitado, armazena nos vetores auxiliares */
            if(v_leste.d != -1 && is_maze(v_leste.x, v_leste.y) && visited[v_leste.x][v_leste.y] == -1)
                v_em_x[i++] = v_leste;

            if(v_oeste.d != -1 && is_maze(v_oeste.x, v_oeste.y) && visited[v_oeste.x][v_oeste.y] == -1)
                v_em_x[i++] = v_oeste;

            if(v_norte.d != -1 && is_maze(v_norte.x, v_norte.y) && visited[v_norte.x][v_norte.y] == -1)
                v_em_y[j++] = v_norte;

            if(v_sul.d != -1 && is_maze(v_sul.x, v_sul.y) && visited[v_sul.x][v_sul.y] == -1)
                v_em_y[j++] = v_sul;

            /* Classifica as casas vizinhas pela distancia mais curta */
            sort_vizinhos(v_em_x, i);
            sort_vizinhos(v_em_y, j);

            /* Os vizinhos resultantes */
            int tam_v = i+j;
            Casa v[tam_v];
            juntar_vizinhos(v_em_x, v_em_y, i, j, v);

            int primeiro_vizinho = rear;

            /* Armazena os vizinhos da casa na fila */
            for (int k = 0; k < tam_v; k++)
            {

                /* ------------------------ teste ------------------------ */
                // Neste teste eu imprimo todos os vizinhos que entraram na fila
                printf("\t (rear = %d): (%dx%d):%d \n", rear, v[k].x, v[k].y, v[k].d);
                /* -------------------------------------------------------- */
                q[rear] = v[k];

                rear = (rear + 1) % MAX;
            }

            for(int i = primeiro_vizinho; i < rear; i++)
            {
                int x = q[front].x;
                int y = q[front].y;
                int x_v = q[i].x;
                int y_v = q[i].y;

                if(x != x_v && y != y_v)
                {
                    front = (front + 1) % MAX;
                }
                else
                {
                    break;
                }
            }

            /* ------------------------ teste ------------------------ */
            // Neste teste eu imprimo a fila
            //for (int i = front; i < rear; i++)
            //{
                //printf("\t q(front = %d): (%dx%d):%d v = %d \n", i, q[i].x, q[i].y, q[i].d, visited[q[i].x][q[i].y]);
            //}
            /* -------------------------------------------------------- */

            bussula = olhar_bussula(q[front], x_antigo, y_antigo); // Pega a direcao atual do rato

            /* Se o robo esta olhando para outra direcao*/
            if(direcao_antiga != bussula || m == 0)
                virar_rato(direcao_antiga, bussula); // Vira o rato para outra direcao

            /* ------------------------ teste ------------------------*/
            // Neste teste eu imprimo onde o rato esta olhando
            printf("\t (%dx%d):%d olhando para: %d tenta se mover para: (%dx%d):%d\n", c.x, c.y, c.d, bussula, q[front].x, q[front].y, q[front].d);
            /* -------------------------------------------------------- */

            m = verificar_resposta(andar(), 'w'); // Move o rato
        }
    }
}

void inunda_maze_direita_baixo(int maze[MAX_ROW][MAX_COL], int x, int y, int d)
{
    /* Se fora dos limites do labirinto ou se ja foi visitado */
    if (!is_maze(x, y) || maze[x][y] != -1 || d < 0)
        return;

    maze[x][y] = d;

    inunda_maze_direita_baixo(maze, x, y+1, d - 1); // Direta
    inunda_maze_direita_baixo(maze, x+1, y, d - 1); // Baixo
}

void inunda_maze_direita_cima(int maze[MAX_ROW][MAX_COL], int x, int y, int d)
{
    /* Se fora dos limites do labirinto ou se ja foi visitado */
    if (!is_maze(x, y) || d < 0)
        return;

    maze[x][y] = d;

    inunda_maze_direita_cima(maze, x, y+1, d - 1); // Direta
    inunda_maze_direita_cima(maze, x-1, y, d - 1); // Cima
}

void inunda_maze_esquerda_baixo(int maze[MAX_ROW][MAX_COL], int x, int y, int d)
{
    /* Se fora dos limites do labirinto ou se ja foi visitado */
    if (!is_maze(x, y) || d < 0)
        return;

    maze[x][y] = d;

    inunda_maze_esquerda_baixo(maze, x, y-1, d - 1); // Esquerda
    inunda_maze_esquerda_baixo(maze, x+1, y, d - 1); // Baixo
}

void inunda_maze_esquerda_cima(int maze[MAX_ROW][MAX_COL], int x, int y, int d)
{
    /* Se fora dos limites do labirinto ou se ja foi visitado */
    if (!is_maze(x, y) || d < 0)
        return;

    maze[x][y] = d;

    inunda_maze_esquerda_cima(maze, x, y-1, d - 1); // Esquerda
    inunda_maze_esquerda_cima(maze, x-1, y, d - 1); // Cima
}

void print_maze(int maze[MAX_ROW][MAX_COL])
{
    printf("\nMaze: \n");
    for (int i = 0; i < MAX_ROW; i++)
    {
        for (int j = 0; j < MAX_COL; j++)
        {
            printf("%3d", maze[i][j]);
        }
        printf("\n");
    }
}

void print_map(int visited[MAX_ROW][MAX_COL])
{

    printf("\nMap: \n");
    for (int i = 0; i < MAX_ROW; i++)
    {
        for (int j = 0; j < MAX_COL; j++)
        {
            printf("%3d ", visited[i][j]);
        }
        printf("\n");
    }
}

void print_path(Casa path[MAX_ROW * MAX_COL])
{
    printf("\nPath: ");
    for(int i = 0; i < MAX_ROW * MAX_COL; i++)
    {
        printf("(%d): (%dx%d):%d ", i, path[i].x, path[i].y, path[i].d);
    }
    printf("\n");
}

int main()
{
    /* Labirinto */
    int maze[MAX_ROW][MAX_COL];

    /* Mapa de visitados */
    int visited[MAX_ROW][MAX_COL];

    /* Caminho para o objetivo */
    Casa path[MAX_ROW * MAX_COL];

    /* Inicializa labirinto e mapa como nao-explorado e nao-visitado, respectivamente */
    for (int i = -1; i < MAX_ROW; i++)
    {
        for (int j = -1; j < MAX_COL; j++)
        {
            maze[i][j] = -1;
            visited[i][j] = -1;
        }
    }

    /* Inicializa o caminho */
    for (int i = 0; i < MAX_ROW * MAX_COL; i++)
    {
        path[i].d = -1;
    }

    /* Distancia ate o objetivo */
    //int d = sensor_distancia();
    int d = D_TESTE;

    /* Inicializa o labirinto com distancias ate o objetivo */
    inunda_maze_direita_baixo(maze, MAX_ROW/2, MAX_COL/2, d);
    inunda_maze_direita_cima(maze, MAX_ROW/2, MAX_COL/2, d);

    inunda_maze_esquerda_baixo(maze, MAX_ROW/2, MAX_COL/2, d);
    inunda_maze_esquerda_cima(maze, MAX_ROW/2, MAX_COL/2, d);

    //inunda_maze(maze, MAX_ROW/2, MAX_COL/2, d);

    /* Flood fill da origem ate o objetivo */
    flood_fill(path, maze, visited, MAX_ROW/2, MAX_COL/2, d);

    /* ------------------------ teste ------------------------ */
    print_maze(maze);
    //print_map(visited);
    //print_path(path);
    /* -------------------------------------------------------- */

    return 0;
}
