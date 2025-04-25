#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        write(STDERR_FILENO, "Usage: ./repeat <something>\n", 25);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        write(STDOUT_FILENO, argv[i], strlen(argv[i]));
        write(STDOUT_FILENO, " ", 1);
    }
    
    return 0;
}