/*
 *------------------------------------------------------------------------------
 * LIBRARIES
 *------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

/*
 *------------------------------------------------------------------------------
 * DEFINITIONS
 *------------------------------------------------------------------------------
 */

// tower names
#define TOWER_A 'A'
#define TOWER_B 'B'
#define TOWER_C 'C'

// character used to represent the discs
#define DISC_CHARACTER '*'

/*
 *------------------------------------------------------------------------------
 * GLOBAL VARIABLES
 *------------------------------------------------------------------------------
 */

unsigned int numberOfDiscs = 0;
char *tower;

/*
 *------------------------------------------------------------------------------
 * FUNCTIONS
 *------------------------------------------------------------------------------
 */

// https://stackoverflow.com/questions/50884685/how-to-get-cursor-position-in-c-using-ansi-code
int getCursorPosition(int *x, int *y)
{
    struct termios term, restore;
    char buf[30] = {0};
    int ret, i, pow;
    char ch;
    *y = 0;
    *x = 0;

    tcgetattr(0, &term);
    tcgetattr(0, &restore);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);
    write(1, "\033[6n", 4);

    for (i = 0, ch = 0; ch != 'R'; i++)
    {
        ret = read(0, &ch, 1);
        if (!ret)
        {
            tcsetattr(0, TCSANOW, &restore);
            fprintf(stderr, "getpos: error reading response!\n");
            return 1;
        }
        buf[i] = ch;
    }
    if (i < 2)
    {
        tcsetattr(0, TCSANOW, &restore);
        return 1;
    }
    for (i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
        *x = *x + (buf[i] - '0') * pow;
    for (i--, pow = 1; buf[i] != '['; i--, pow *= 10)
        *y = *y + (buf[i] - '0') * pow;
    tcsetattr(0, TCSANOW, &restore);
    return 0;
}

void gotoxy(const int x, const int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

void printCentered(char *str, unsigned int size)
{
    const unsigned int spaces = (size - strlen(str));
    const unsigned int spacesBefore = spaces / 2;
    const unsigned int spacesAfter = spacesBefore + spaces % 2;
    for (unsigned int i = 0; i < spacesBefore; i++)
        putchar(' ');
    printf("%s", str);
    for (unsigned int i = 0; i < spacesAfter; i++)
        putchar(' ');
}

void printHeader()
{
    printCentered("Tower A", 2 * numberOfDiscs);
    printCentered("Tower B", 2 * numberOfDiscs);
    printCentered("Tower C", 2 * numberOfDiscs);
    putchar('\n');
}

void printTower()
{
    static int x = -1, y = -1;
    static const unsigned int offset = 1;
    const unsigned int baseDiscSize = 2 * numberOfDiscs - 1;
    const unsigned int numberOfColumns = 3 * (baseDiscSize + offset);
    unsigned int a = numberOfDiscs, b = numberOfDiscs, c = numberOfDiscs;
    char table[numberOfDiscs][numberOfColumns];
    memset(table, ' ', sizeof(table));

    if (x < 0 || y < 0)
        getCursorPosition(&x, &y);
    else
        gotoxy(x, y);

    for (unsigned int i = numberOfDiscs - 1; i < numberOfDiscs; i--)
    {
        unsigned int line;
        const unsigned int discSize = baseDiscSize - 2 * (numberOfDiscs - i - 1);
        const unsigned int column = (tower[i] - TOWER_A) * (baseDiscSize + offset) + numberOfDiscs - i + offset - 1;
        if (tower[i] == TOWER_A)
            line = --a;
        else if (tower[i] == TOWER_B)
            line = --b;
        else if (tower[i] == TOWER_C)
            line = --c;
        memset(&table[line][column], DISC_CHARACTER, discSize);
    }
    for (unsigned int line = 0; line < numberOfDiscs; line++)
    {
        printf("%.*s\n", numberOfColumns, table[line]);
    }
}

void move(unsigned int n, char a, char b)
{
    static unsigned int steps = 0;
    sleep(1);
    tower[n - 1] = b;
    printTower();
    printf("Step %d: Move disc %d from %c to %c \n", ++steps, n, a, b);
}

void hanoi(unsigned int n, char a, char b, char c)
{
    if (n == 1)
        move(n, a, b);
    else
    {
        hanoi(n - 1, a, c, b);
        move(n, a, b);
        hanoi(n - 1, c, b, a);
    }
}

/*
 *------------------------------------------------------------------------------
 * MAIN
 *------------------------------------------------------------------------------
 */

int main(const int argc, const char *const argv[])
{
    if (argc >= 2)
    {
        numberOfDiscs = atoi(argv[1]);
    } 
    if (numberOfDiscs <= 3) {
        numberOfDiscs = 5;
    }
    fflush(stdin);
    tower = malloc(numberOfDiscs);
    memset(tower, TOWER_A, numberOfDiscs);
    printHeader();
    printTower();
    hanoi(numberOfDiscs, TOWER_A, TOWER_B, TOWER_C);
    free(tower);
    return EXIT_SUCCESS;
}

/*
 *------------------------------------------------------------------------------
 * END
 *------------------------------------------------------------------------------
 */
