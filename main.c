#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <time.h>

#define clear() printf("\033[2J")
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))

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

void delay(unsigned duration)
{
    time_t start = time(NULL);
    double end = duration;
    time_t now = time(NULL);

    do
    {
        now = time(NULL);
    } while (difftime(now, start) < end);
}

int main(void)
{
    struct termios original_terminal;

    char input_character = 'a';
    int x_position = 5;
    int y_position = 5;

    clear();
    system("tput civis");

    if (initizalize_terminal(&original_terminal))
        return 1;

    while (input_character != 'x')
    {
        input_character = 0;
        if (_kbhit())
        {
            input_character = getchar();
            switch (input_character)
            {
            case 'a':
                gotoxy(x_position, y_position);
                printf("%c", ' ');
                x_position--;
                break;
            case 'd':
                gotoxy(x_position, y_position);
                printf("%c", ' ');
                x_position++;
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