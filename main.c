#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main(void)
{
    struct termios original_terminal;
    struct termios new_terminal;

    char input_character = 'a';

    if (tcgetattr(STDIN_FILENO, &original_terminal) == 0)
    {
        new_terminal = original_terminal;
        new_terminal.c_lflag &= ~(ICANON);
        new_terminal.c_lflag &= ~(ECHO);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_terminal);
    }

    while (input_character != 'x')
    {
        input_character = getchar();
    }

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal);

    return 0;
}