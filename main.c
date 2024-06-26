#include <stdio.h>
#include <limits.h>
#include <math.h>
/**
* Written in Brazilian Portuguese without accents.
* Original repository in https://github.com/AlefMemTav/micromouse
* The author is Pedro Lucas Garcia (garcia.lucas@aluno.unb.br)
*/

#define MAX_ROW 5 // Alterar conforme o tamanho maximo do labirinto
#define MAX_COL 5
#define MAX (MAX_ROW * MAX_COL)

#define X_ORIGEM (MAX_ROW/2)
#define Y_ORIGEM (MAX_COL/2)

#define D_TESTE 2 // Distancia de teste

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
* @author
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

void log_bussola(int x_antigo, int y_antigo, int direcao_antiga, int x, int y, int bussola)
{
    printf("\t bussola antiga de (%dx%d) = %d | bussola de (%dx%d) = %d \n", x_antigo, y_antigo, direcao_antiga, x, y, bussola);
}

void log_movimento(int x_antigo, int y_antigo, int bussola, Casa c)
{
    printf("\t (%dx%d) olhando para %d tenta se mover para: (%dx%d):%d\n", x_antigo, y_antigo, bussola, c.x, c.y, c.d);
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

void virar_rato(int direcao_antiga, int bussola)
{
    switch(direcao_antiga)
    {
    /* Se o rato estava no Norte */
    case 0:
        /* O rato foi para o Leste */
        if(bussola == 1)
        {
            virar_direita();
            log_resposta(5, 'r');
        }
        /* O rato foi para o Sul */
        else if(bussola == 2)
        {
            virar_direita();
            virar_direita();
            log_resposta(5, 'R');
        }
        /* O rato foi para o Oeste */
        else if(bussola == 3)
        {
            virar_esquerda();
            log_resposta(5, 'l');
        }
        break;
    /* Se o rato estava no Leste */
    case 1:
        /* O rato foi para o Norte */
        if (bussola == 0)
        {
            virar_esquerda();
            log_resposta(5, 'l');
        }
        /* O rato foi para o Sul */
        else if (bussola == 2)
        {
            virar_direita();
            log_resposta(5, 'r');
        }
        /* O rato foi para o Oeste */
        else if (bussola == 3)
        {
            virar_direita();
            virar_direita();
            log_resposta(5, 'R');
        }
        break;
    /* Se o rato estava no Sul */
    case 2:
        /* O rato foi para o Norte */
        if(bussola == 0)
        {
            virar_direita();
            virar_direita();
            log_resposta(5, 'R');
        }
        /* O rato foi para o Leste */
        else if(bussola == 1)
        {
            virar_esquerda();
            log_resposta(5, 'l');
        }
        /* O rato foi para o Oeste */
        else if(bussola == 3)
        {
            virar_direita();
            log_resposta(5, 'r');
        }
        break;
    /* Se o rato estava no Oeste */
    case 3:
        /* O rato foi para o Norte*/
        if (bussola == 0)
        {
            virar_direita();
            log_resposta(5, 'r');
        }
        /* O rato foi para o Leste */
        else if (bussola == 1)
        {
            virar_direita();
            virar_direita();
            log_resposta(5, 'R');
        }
        /* O rato foi para o Sul */
        else if (bussola == 2)
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

int olhar_bussola(Casa c, int x_antigo, int y_antigo)
{
    /* Se for o inicio, o rato esta olhando para o Leste */
    int bussola = 1; // Leste

    /* Se o rato esta se movendo no Leste ou Oeste */
    if(c.x == x_antigo && c.y != y_antigo)
    {
        /* O rato tentou ir para o Leste */
        if(c.y > y_antigo)
            bussola = 1;
        /* O rato tentou ir para o Oeste */
        else
            bussola = 3;
    }
    /* Se o rato esta se movendo no Norte ou Sul */
    else if(c.x != x_antigo && c.y == y_antigo)
    {
        /* O rato tentou ir para o Sul */
        if(c.x > x_antigo)
            bussola = 2;
        /* O rato tentou ir para o Norte */
        else
            bussola = 0;
    }
    return bussola;
}

int menor_direcao(int bussola_1, int bussola)
{
    if(bussola == 0 && bussola_1 == 3)
        return 1;
    else if(bussola == 3 && bussola_1 == 0)
        return 1;
    else
        return abs(bussola_1 - bussola);
}

void sort_vizinhos_distancia_direcao(Casa* v, int tamanho, int x, int y, int bussola)
{
    for (int i = 0; i < tamanho - 1; i++)
    {
        for (int j = 0; j < tamanho - i - 1; j++)
        {
            /* Se as distancias forem iguais, priorizar a mesma direcao da bussola ou a menor direcao */
            if(v[j].d == v[j + 1].d)
            {
                int b_1 = olhar_bussola(v[j], x, y);
                int b_2 = olhar_bussola(v[j + 1], x, y);
                if(b_1 != bussola && b_2 == bussola)
                {
                    SWAP(v[j], v[j + 1]);
                }
                else if(menor_direcao(b_1, bussola) > menor_direcao(b_2, bussola))
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

    int bussola = 1; // (0 = norte, 1 = leste, 2 = sul, 3 = oeste)
    int direcao_antiga = bussola; // Direcao antiga do rato

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
            maze[x_antigo][y_antigo] = -1; // Marca como caminho morto
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
            direcao_antiga = bussola; // Direcao antiga do rato
            bussola = olhar_bussola(c, x_antigo, y_antigo); // Pega a direcao atual do rato

            log_bussola(x_antigo, y_antigo, direcao_antiga, x, y, bussola); // Log

            /* Se o rato esta olhando para outra direcao ou se bateu em uma parede */
            if(direcao_antiga != bussola || m == 0)
                virar_rato(direcao_antiga, bussola);

            log_movimento(x_antigo, y_antigo, bussola, c); // Log

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
        if ((m == 1 || m == 2) && is_maze(x, y) && (!visited[x][y] || is_ciclo || fase == 2))
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

            Casa vizinhos[4];

            /* Se o vizinho esta no labirinto e nao foi visitado ou fase 2, armazena nos vetores auxiliares */
            if(v_norte.d != -1 && is_maze(v_norte.x, v_norte.y) && (!visited[v_norte.x][v_norte.y] || fase == 2))
                vizinhos[size_vizinhos++] = v_norte;

            if(v_leste.d != -1 && is_maze(v_leste.x, v_leste.y) && (!visited[v_leste.x][v_leste.y] || fase == 2))
                vizinhos[size_vizinhos++] = v_leste;

            if(v_sul.d != -1 && is_maze(v_sul.x, v_sul.y) && (!visited[v_sul.x][v_sul.y] || fase == 2))
                vizinhos[size_vizinhos++] = v_sul;

            if(v_oeste.d != -1 && is_maze(v_oeste.x, v_oeste.y) && (!visited[v_oeste.x][v_oeste.y] || fase == 2))
                vizinhos[size_vizinhos++] = v_oeste;

            /* Classifica as casas vizinhas pela distancia mais curta e pela mesma direcao da bussola */
            sort_vizinhos_distancia_direcao(vizinhos, size_vizinhos, x, y, bussola);

            /* Se houver vizinhos, armazena os vizinhos da casa na fila */
            for (int k = 0; k < size_vizinhos; k++)
            {
                q[rear] = vizinhos[k];
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
