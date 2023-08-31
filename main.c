#include <stdio.h>
#include <termios.h>
#include <unistd.h>

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

int main(void)
{
    struct termios original_terminal;

    char input_character = 'a';

    if (initizalize_terminal(&original_terminal))
        return 1;

    while (input_character != 'x')
    {
        input_character = getchar();
    }

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal);

    return 0;
}