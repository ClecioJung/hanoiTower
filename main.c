/**
 ------------------------------------------------------------
    BIBLIOTECAS
 ------------------------------------------------------------
**/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

/**
 ------------------------------------------------------------
    MACROS
 ------------------------------------------------------------
**/
/* Macro que quando diferente de zero habilita a contagem
    de tempo que o software dispendeu em sua execução */
#define CONTAR_TEMPO    0

// Nomes das Torres
#define TORRE_A         'A'
#define TORRE_B         'B'
#define TORRE_C         'C'

// Caractere utilizado para representaros discos
#define CARACTER        254//219

/**
 ------------------------------------------------------------
    VARIÀVEIS GLOBAIS
 ------------------------------------------------------------
**/

// Posição atual dos discos
char *torre;

// Quantidade de discos
unsigned int numDiscos;

// Contador de passos já efetuados
unsigned int passos = 0;

/**
 ------------------------------------------------------------
    FUNÇÕES
 ------------------------------------------------------------
**/

// Função responsável por definir possição do cursor
void gotoxy(int x, int y)
{
    COORD coord = {0, 0};

    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Imprime a torre passada como parametro na tela
void ImpTorre(char a)
{
	// Variáveis locais
    int i, j;
    unsigned int x1, x2;
    unsigned int linha = 4 + numDiscos;

    switch (a)
    {
    case TORRE_A:
        x1 = 1;
        break;
    case TORRE_B:
        x1 = max(2*numDiscos + 1, 9);
        break;
    case TORRE_C:
    default:
        x1 = max(4*numDiscos + 1, 17);
    }
    x2 = x1;
    if (numDiscos > 4)
        x1 += (numDiscos - 3);
    else x2 += (3 - numDiscos);

    gotoxy(x1, 3);
    printf("Torre %c", a);

    for (i = (numDiscos - 1); i >= 0; i--)
    {
        gotoxy(x2, linha);

        if (torre[i] == a)
        {
            linha--;

            // Imprime lado esquerdo da torre
            for (j = 0; j <= numDiscos; j++)
            {
                if (j < (numDiscos - i))
                    putchar(' ');
                else
                    putchar(CARACTER);
            }

            // Imprime lado direito da torre
            for (j = numDiscos; j >= 0; j--)
            {
                if (j > (numDiscos - i))
                    putchar(CARACTER);
                else
                    putchar(' ');
            }
            //putchar(torre[i]);
            putchar('\n');
        }
    }
}

// Imprime o passo atual do jogo na tela
void Imprime(void)
{
    ImpTorre(TORRE_A);
    ImpTorre(TORRE_B);
    ImpTorre(TORRE_C);

    gotoxy(0, (5 + numDiscos));
}

// Realiza um movimento do hanoi (um disco)
void Move(unsigned int n, char a, char b)
{
    // Exibe os dados na tela por um segundo
    Sleep(1000);

    // Limpa a tela
    system("cls");

    // Incrementa contador de passos
    passos++;
    printf("\nPasso %d: Mova disco %d de %c para %c\n", passos, n, a, b);
    torre[n-1] = b;

    // Imprime torre
    Imprime();
}

// Função responsável pela lógica do jogo
void hanoi(unsigned int n, char a, char b, char c)
{
    /** Mova n discos da torre a para a torre b usando
        a torre c como intermediário **/

    if (n == 1)
        Move(n, a, b);
    else
    {
        hanoi(n - 1, a, c, b);
        Move(n, a, b);
        hanoi(n - 1, c, b, a);
    }
}

/**
 ------------------------------------------------------------
    MAIN
 ------------------------------------------------------------
**/

int main()
{
	// Variáveis locais
    unsigned int i;
#if CONTAR_TEMPO
    long long int inicio;
    long long int fim;
#endif // CONTAR_TEMPO

    puts("\nInforme o numero de discos:\n");
    scanf("%d", &numDiscos);
    fflush(stdin);

    // Inicializa torre virtual
    torre = malloc(numDiscos);
    for (i = 0; i < numDiscos; i++)
    {
        torre[i] = TORRE_A;
    }

#if CONTAR_TEMPO
    inicio = clock();
#endif // CONTAR_TEMPO

    // Limpa a tela
    system("cls");

    // Imprime condição inicial
    printf("\nCondicao Inicial: \n");
    Imprime();

    // Executa a resolução do jogo
    hanoi(numDiscos, TORRE_A, TORRE_B, TORRE_C);

#if CONTAR_TEMPO
    fim = clock();
    printf("Tempo decorrido: %dms", (fim - inicio));
#endif // CONTAR_TEMPO

    // Desaloca memória alocada dinamicamente
    free(torre);
    torre = NULL;

    // Espera o usuário clicar uma tecla
    putchar('\n');
    system("pause");

    return 0;
}

/**
 ------------------------------------------------------------
    FIM
 ------------------------------------------------------------
**/
