#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <time.h>

#define clear() printf("\033[2J")
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))

#define MAX_INVADERS 55

// 5 por 11

typedef struct invader
{
    unsigned int x_position : 4;
    unsigned int y_position : 4;

} invader_t;

// The function recieves a pointer to an empty termios struct
// Saves the original terminal settings to original_terminal
// Changes the current terminal canonity to non-canonical
// returns 1 if the original settings could not be fetched.
int initizalize_terminal(struct termios *original_terminal)
{
    struct termios new_terminal;

    if (tcgetattr(STDIN_FILENO, original_terminal) != 0)
        return 1;

    if (tcgetattr(STDIN_FILENO, &new_terminal) == 0)
    {
        new_terminal.c_lflag &= ~(ICANON);
        new_terminal.c_lflag &= ~(ECHO);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_terminal);
    }
    else
        return 1;

    return 0;
}

// Returns the amount of keys waiting in the buffer.
int _kbhit()
{
    int keys_pressed = 0;
    ioctl(0, FIONREAD, &keys_pressed);
    return keys_pressed;
}

void init_invaders(invader_t *all_invaders)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            int invader_index = (i * 11) + j;

            all_invaders[invader_index].x_position = j + 4;
            all_invaders[invader_index].y_position = i + 1;
        }
    }
}

void print_invaders(invader_t *all_invaders)
{
    for (int i = 0; i < MAX_INVADERS; i++)
    {
        int x = all_invaders[i].x_position;
        int y = all_invaders[i].y_position;

        gotoxy(x, y);
        printf("M");
    }
}

int main(void)
{
    struct termios original_terminal;

    char input_character = 'a';
    int x_position = 5;
    int y_position = 9;

    invader_t all_invaders[MAX_INVADERS];

    if (initizalize_terminal(&original_terminal))
        return 1;

    clear();
    system("tput civis");

    init_invaders(all_invaders);
    print_invaders(all_invaders);

    while (input_character != 'x')
    {
        input_character = 0;
        if (_kbhit())
        {
            input_character = getchar();
            switch (input_character)
            {
            case 'a':
                if (x_position >= 2)
                {
                    gotoxy(x_position, y_position);
                    printf("%c", ' ');
                    x_position--;
                }
                break;
            case 'd':
                if (x_position < 17)
                {
                    gotoxy(x_position, y_position);
                    printf("%c", ' ');
                    x_position++;
                }
                break;
            }
        }
        gotoxy(x_position, y_position);
        printf("%c", 'A');
        usleep(1);
    }

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal);
    system("tput cnorm");
    system("clear");

    return 0;
}