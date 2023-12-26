#include <stdio.h>
#include <limits.h>
#include <math.h>

#define MAX_ROW 5
#define MAX_COL 5
#define MAX (MAX_ROW * MAX_COL)

#define X_ORIGEM (MAX_ROW/2)
#define Y_ORIGEM (MAX_COL/2)

#define D_TESTE 2

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

/* Labirinto */
int maze[MAX_ROW][MAX_COL];

/* Mapa de visitados */
int visited[MAX_ROW][MAX_COL];

/* Caminho para o objetivo */
Casa path[MAX];

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

void log_resposta(int m, char c)
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
}

void log_atual(Casa c)
{
    printf("\t atual: (%dx%d):%d \n", c.x, c.y, c.d);
}

void log_fila(Casa q[MAX], int front, int rear)
{
    for (int i = front; i < rear; i++)
    {
        printf("\t q(i = %d): (%dx%d):%d ", i, q[i].x, q[i].y, q[i].d);
        printf("v = %d \n", visited[q[i].x][q[i].y]);
    }
}

void log_indice_fila(int front, int rear)
{
    printf("\t front:%d rear:%d \n", front, rear);
}

void log_bussula(int x_antigo, int y_antigo, int direcao_antiga, int x, int y, int bussula)
{
    printf("\t bussula antiga de (%dx%d) = %d | bussula de (%dx%d) = %d \n", x_antigo, y_antigo, direcao_antiga, x, y, bussula);
}

void log_movimento(int x_antigo, int y_antigo, int bussula, Casa c)
{
    printf("\t (%dx%d) olhando para %d tenta se mover para: (%dx%d):%d\n", x_antigo, y_antigo, bussula, c.x, c.y, c.d);
}

void log_map()
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

void log_path(int size_path)
{
    printf("\nPath: ");
    for(int i = 0; i < size_path; i++)
    {
        printf("(%d): (%dx%d):%d ", i, path[i].x, path[i].y, path[i].d);
    }
    printf("\n");
}

void virar_rato(int direcao_antiga, int bussula)
{
    switch(direcao_antiga)
    {
    /* Se o rato estava no Norte */
    case 0:
        /* O rato foi para o Leste */
        if(bussula == 1)
        {
            virar_direita();
            log_resposta(5, 'r');
        }
        /* O rato foi para o Sul */
        else if(bussula == 2)
        {
            virar_direita();
            virar_direita();
            log_resposta(5, 'R');
        }
        /* O rato foi para o Oeste */
        else if(bussula == 3)
        {
            virar_esquerda();
            log_resposta(5, 'l');
        }
        break;
    /* Se o rato estava no Leste */
    case 1:
        /* O rato foi para o Norte */
        if (bussula == 0)
        {
            virar_esquerda();
            log_resposta(5, 'l');
        }
        /* O rato foi para o Sul */
        else if (bussula == 2)
        {
            virar_direita();
            log_resposta(5, 'r');
        }
        /* O rato foi para o Oeste */
        else if (bussula == 3)
        {
            virar_direita();
            virar_direita();
            log_resposta(5, 'R');
        }
        break;
    /* Se o rato estava no Sul */
    case 2:
        /* O rato foi para o Norte */
        if(bussula == 0)
        {
            virar_direita();
            virar_direita();
            log_resposta(5, 'R');
        }
        /* O rato foi para o Leste */
        else if(bussula == 1)
        {
            virar_esquerda();
            log_resposta(5, 'l');
        }
        /* O rato foi para o Oeste */
        else if(bussula == 3)
        {
            virar_direita();
            log_resposta(5, 'r');
        }
        break;
    /* Se o rato estava no Oeste */
    case 3:
        /* O rato foi para o Norte*/
        if (bussula == 0)
        {
            virar_direita();
            log_resposta(5, 'r');
        }
        /* O rato foi para o Leste */
        else if (bussula == 1)
        {
            virar_direita();
            virar_direita();
            log_resposta(5, 'R');
        }
        /* O rato foi para o Sul */
        else if (bussula == 2)
        {
            virar_esquerda();
            log_resposta(5, 'l');
        }
        break;
    }
}

int mover_rato(char comando)
{
    int m;
    switch(comando)
    {
    case 'w':
        m = andar();
        break;
    case 'j':
        m = correr_pouco();
        break;
    case 'R':
        m = correr();
        break;
    case 's':
        m = correr_muito();
        break;
    }
    return m;
}

int olhar_bussula(Casa c, int x_antigo, int y_antigo)
{
    /* Se for o inicio, o rato esta olhando para o Leste */
    int bussula = 1; // Leste

    /* Se o rato esta se movendo no Leste ou Oeste */
    if(c.x == x_antigo && c.y != y_antigo)
    {
        /* O rato tentou ir para o Leste */
        if(c.y > y_antigo)
            bussula = 1;
        /* O rato tentou ir para o Oeste */
        else
            bussula = 3;
    }
    /* Se o rato esta se movendo no Norte ou Sul */
    else if(c.x != x_antigo && c.y == y_antigo)
    {
        /* O rato tentou ir para o Sul */
        if(c.x > x_antigo)
            bussula = 2;
        /* O rato tentou ir para o Norte */
        else
            bussula = 0;
    }
    return bussula;
}

int menor_direcao(int bussula_1, int bussula)
{
    if(bussula == 0 && bussula_1 == 3)
        return 1;
    else if(bussula == 3 && bussula_1 == 0)
        return 1;
    else
        return abs(bussula_1 - bussula);
}

/* Verificar */
void sort_distancia_direcao(Casa* v, int tamanho, int x, int y, int bussula)
{
    for (int i = 0; i < tamanho - 1; i++)
    {
        for (int j = 0; j < tamanho - i - 1; j++)
        {
            /* Se as distancias forem iguais, priorizar a mesma direcao da bussula ou a menor direcao */
            if(v[j].d == v[j + 1].d)
            {
                int b_1 = olhar_bussula(v[j], x, y);
                int b_2 = olhar_bussula(v[j + 1], x, y);
                if(b_1 != bussula && b_2 == bussula)
                {
                    SWAP(v[j], v[j + 1]);
                }
                else if(menor_direcao(b_1, bussula) > menor_direcao(b_2, bussula))
                {
                    SWAP(v[j], v[j + 1]);
                }
            }
            /* Se as distancias forem diferentes, priorizar o de menor distancia */
            else if(v[j].d > v[j + 1].d)
            {
                SWAP(v[j], v[j + 1]);
            }
        }
    }
}

/* Verificar */
void juntar_vizinhos(Casa* arr1, Casa* arr2, int size1, int size2, Casa* arr3, int x, int y, int bussula)
{
    int i = 0, j = 0, k = 0;

    while (i < size1 && j < size2)
    {
        /* Se as distancias forem iguais, priorizar a mesma direcao ou a menor direcao possivel */
        if(arr1[i].d == arr2[j].d)
        {
            int b_1 = olhar_bussula(arr1[i], x, y);
            int b_2 = olhar_bussula(arr2[j], x, y);
            if(b_1 == bussula && b_2 != bussula)
            {
                arr3[k++] = arr1[i++];
            }
            else if(b_2 == bussula && b_1 != bussula)
            {
                arr3[k++] = arr2[j++];
            }
            else if(menor_direcao(b_1, bussula) <= 1)
            {
                //printf("b_1  = %d (%dx%d) dif = %d\n", b_1, arr1[i].x, arr1[i].y, abs(b_1 - bussula));
                arr3[k++] = arr1[i++];
            }
            else if(menor_direcao(b_2, bussula) <= 1)
            {
                //printf("b_1 = %d (%dx%d) dif = %d\n", b_2, arr2[j].x, arr2[j].y, abs(b_2 - bussula));
                arr3[k++] = arr2[j++];
            }
        }
        /* Se as distancias forem diferentes, priorizar o de menor distancia */
        else if(arr1[i].d < arr2[j].d)
            arr3[k++] = arr1[i++];
        else
            arr3[k++] = arr2[j++];
    }

    while (i < size1)
        arr3[k++] = arr1[i++];

    while (j < size2)
        arr3[k++] = arr2[j++];
}

void atualizar_distancias(int tamanho)
{
    for(int i = 0; i < tamanho; i++)
    {
        int x_p = path[i].x;
        int y_p = path[i].y;

        maze[x_p][y_p] = ++path[i].d;
    }
}

Casa flood_fill(int x, int y, int d, int *size_path, char comando, int fase)
{
    Casa q[MAX]; // Fila de vizinhos da posicao atual

    int front = 0; // Frente da fila
    int rear = 0; // Final da fila

    Casa c = {x, y, d}; // A casa inicial de onde o rato parte
    q[rear++] = c; // Coloca a casa na frente da fila

    int bussula = 1; // (0 = norte, 1 = leste, 2 = sul, 3 = oeste)
    int direcao_antiga = bussula; // Direcao antiga do rato

    /* Coordenadas antigas do rato */
    int x_antigo = x;
    int y_antigo = y;

    int size_vizinhos = 0; // Contador de vizinhos de uma casa

    int m = 1; // No inicio, como o rato esta no labirinto, considera que ele se moveu para a casa inicial

    int is_inicio = 1;
    int is_ciclo = 0;

    /* Enquanto a fila nao estiver vazia */
    while (front != rear)
    {
        log_indice_fila(front, rear); // Log
        /* Se o rato se moveu, pega a casa vizinha na frente da fila */
        if(m == 1)
        {
            c = q[front];
        }
        /* Se nao conseguiu se mover, pega a proxima casa vizinha na fila */
        else if(m == 0 && size_vizinhos > 0)
        {
            front = (front + 1) % MAX;
            c = q[front];
        }
        /* Se ciclo, pegar a ultima casa no caminho */
        else if(m == 0 && is_ciclo && size_vizinhos == 0)
        {
            rear = (rear + 1) % MAX;
            c = path[(*size_path) - 1];
            printf("Voltar para: %dx%d\n", c.x, c.y);
        }

        /* Atualiza as coordenadas da casa atual */
        x = c.x;
        y = c.y;

        log_atual(c); // Log

        /* Se nao for inicio, a casa atual na fila sera para onde o rato esta olhando e tentara se mover */
        if(!is_inicio)
        {
            direcao_antiga = bussula; // Direcao antiga do rato
            bussula = olhar_bussula(c, x_antigo, y_antigo); // Pega a direcao atual do rato

            log_bussula(x_antigo, y_antigo, direcao_antiga, x, y, bussula); // Log

            /* Se o rato esta olhando para outra direcao ou se bateu em uma parede */
            if(direcao_antiga != bussula || m == 0)
                virar_rato(direcao_antiga, bussula);

            log_movimento(x_antigo, y_antigo, bussula, c); // Log

            m = mover_rato(comando);

            log_resposta(m, comando); // Log

            if(m == 0 && size_vizinhos > 0)
            {
                //printf("entrei size_vizinhos era %d ", size_vizinhos);
                --size_vizinhos;
                //printf("agora %d\n", size_vizinhos);
                if(size_vizinhos == 0)
                    is_ciclo = 1;
            }
        }

        /* Se for possivel se mover ate a casa e ela estiver no labirinto e nao tiver sido visitada ou for a fase 2 */
        if ((m == 1 || m == 2) && (is_maze(x, y) && (!visited[x][y] || is_ciclo || fase == 2)))
        {
            /* Se nao for ciclo e a casa nao tiver sido visitada, armazena a casa no caminho */
            if(!is_ciclo && !visited[x][y])
                path[++(*size_path)] = c;

            visited[x][y] = 1; // Marca a casa como visitada

            /* Se encontrou o objetivo, terminar */
            if(m == 2 && comando == 'w')
                return c;

            /* Se encontrou o inicio, terminar */
            if(fase == 2 && c.x == X_ORIGEM && c.y == Y_ORIGEM)
                return c;

            size_vizinhos = 0;

            front = rear; // Pega o proximo vizinho da casa atual

            /* Guarda as coordenadas antigas */
            x_antigo = x;
            y_antigo = y;

            /* Pega os vizinhos da casa */
            Casa v_norte = {x+1, y, maze[x+1][y]};
            Casa v_sul = {x-1, y, maze[x-1][y]};
            Casa v_leste = {x, y+1, maze[x][y+1]};
            Casa v_oeste = {x, y-1, maze[x][y-1]};

            int i = 0, j = 0;
            Casa v_em_x[2]; // Leste e Oeste
            Casa v_em_y[2]; // Norte e Sul

            /* Se o vizinho esta no labirinto e nao foi visitado ou fase 2, armazena nos vetores auxiliares */
            if(v_norte.d != -1 && is_maze(v_norte.x, v_norte.y) && (!visited[v_norte.x][v_norte.y] || fase == 2))
                v_em_y[j++] = v_norte;

            if(v_sul.d != -1 && is_maze(v_sul.x, v_sul.y) && (!visited[v_sul.x][v_sul.y] || fase == 2))
                v_em_y[j++] = v_sul;

            if(v_leste.d != -1 && is_maze(v_leste.x, v_leste.y) && (!visited[v_leste.x][v_leste.y] || fase == 2))
                v_em_x[i++] = v_leste;

            if(v_oeste.d != -1 && is_maze(v_oeste.x, v_oeste.y) && (!visited[v_oeste.x][v_oeste.y] || fase == 2))
                v_em_x[i++] = v_oeste;

            /* Classifica as casas vizinhas pela distancia mais curta e pela mesma direcao da bussula */
            sort_distancia_direcao(v_em_x, i, x, y, bussula);
            sort_distancia_direcao(v_em_y, j, x, y, bussula);

            /* Os vizinhos resultantes */
            size_vizinhos = i + j;
            Casa v[size_vizinhos];

            juntar_vizinhos(v_em_x, v_em_y, i, j, v, x, y, bussula);

            /* Se houver vizinhos, armazena os vizinhos da casa na fila */
            for (int k = 0; k < size_vizinhos; k++)
            {
                q[rear] = v[k];
                rear = (rear + 1) % MAX;
            }

            log_fila(q, front, rear); // Log

            /* Se for inicio, negar */
            if(is_inicio)
                is_inicio = 0;

            /* Se for ciclo, negar */
            if(is_ciclo)
                is_ciclo = 0;
        }
    }
    return c;
}

void inunda_maze_direita(int x, int y, int d)
{
    /* Se estiver fora do labirinto ou ja estiver inundada, retorna */
    if (!is_maze(x, y) || maze[x][y] != -1)
        return;

    if(d < 0)
        maze[x][y] = d*-1;
    else
        maze[x][y] = d;

    inunda_maze_direita(x, y+1, d - 1); // Direta
    inunda_maze_direita(x+1, y, d - 1); // Baixo
    inunda_maze_direita(x-1, y, d - 1); // Cima
}

void inunda_maze_esquerda(int x, int y, int d)
{
    /* Se estiver fora do labirinto ou ja estiver inundada, retorna */
    if (!is_maze(x, y) || maze[x][y] != -1)
        return;

    if(d < 0)
        maze[x][y] = d*-1;
    else
        maze[x][y] = d;

    inunda_maze_esquerda(x, y - 1, d - 1); // Esquerda
    inunda_maze_esquerda(x + 1, y, d - 1); // Baixo
    inunda_maze_esquerda(x - 1, y, d - 1); // Cima
}

void inunda_maze(int x, int y, int x_a, int y_b, int d, int d_m)
{
    /* Se encontrou o objetivo, retorna */
    //if((x != x_a || y != y_b) && (d == d_m))
    //return;

    if (is_maze(x, y) && maze[x][y] > d)
    {
        maze[x][y] = d;

        inunda_maze(x - 1, y, x_a, y_b, d + 1, d_m);
        inunda_maze(x + 1, y, x_a, y_b, d + 1, d_m);
        inunda_maze(x, y - 1, x_a, y_b, d + 1, d_m);
        inunda_maze(x, y + 1, x_a, y_b, d + 1, d_m);
    }
}

void log_maze()
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

void inicializa_maze(int valor)
{
    for(int i = -1; i < MAX_ROW; i++)
        for(int j = -1; j < MAX_COL; j++)
            maze[i][j] = valor;
}

void inicializa_visited()
{
    for(int i = -1; i < MAX_ROW; i++)
        for(int j = -1; j < MAX_COL; j++)
            visited[i][j] = 0;
}

void inicializa_path()
{
    for(int i = 0; i < MAX; i++)
        path[i].d = -1;
}

int main()
{
    /* Distancia ate o objetivo */
    //int d = sensor_distancia();
    int d = D_TESTE;

    //printf("inicial: %dx%d\n", X_ORIGEM, Y_ORIGEM);

    inicializa_visited();
    inicializa_maze(-1);
    inicializa_path();

    int size_path = -1; // Contador do tamanho do caminho

    /* Inicializa o labirinto com distancias ate o objetivo */
    inunda_maze_direita(X_ORIGEM, Y_ORIGEM, d);
    inunda_maze_esquerda(X_ORIGEM, Y_ORIGEM - 1, d - 1);

    /* Flood fill do inicio ate o objetivo */
    Casa a = flood_fill(X_ORIGEM, Y_ORIGEM, d, &size_path, 'w', 1);

    /* Coordenadas do objetivo */
    int x_a = a.x;
    int y_a = a.y;

    //printf("objetivo: %dx%d\n", x_a, y_a);

    /* Inicializa novamente o labirinto */
    inicializa_maze(INT_MAX);
    /* Atualiza o labirinto com as distancias do objetivo ate a casa inicial */
    inunda_maze(X_ORIGEM, Y_ORIGEM, x_a, y_a, 0, d);
    /* Flood fill do objetivo ate o inicio */
    Casa b = flood_fill(x_a, y_a, 0, &size_path, 'w', 2);

    log_maze();
    log_map();
    //log_path(size_path);

    return 0;
}
