#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>


void usage(const char *name, const char *message)
{
    fprintf(
        stderr,
        "usage: %1$s proxy command [arg [arg [...]]]\n"
        "  the proxy parameter may be set to '-' so no proxy will be used\n"
        "%1$s: %2$s\n",
        name, message
    );
    exit(1);
}

size_t get_envname(char *dest, const char *var)
{
    size_t i = 0;
    while ((dest[i] = tolower(var[i])) != '=') ++i;
    dest[i] = '\0';
    return i;
}

int main(int argc, char **argv, char **env)
{
    char envname[128];

    if (argc < 2)
        usage(argv[0], "no proxy was specified");
    else if (argc < 3)
        usage(argv[0], "no command was specified");

    for (char **envp = env; *envp != NULL; ++envp, get_envname(envname, *envp))
        if (strstr(envname, "_proxy"))
            unsetenv(envname);

    if (strcmp(argv[1], "-"))
        setenv("all_proxy", argv[1], 1);

    return execvp(argv[2], &argv[2]);
}
