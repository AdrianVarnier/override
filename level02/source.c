 #include <stdio.h>

int main(int argc, const char **argv, const char **envp)
{
    char    user_pass[96];
    char    pass[48];
    char    user_name[96];
    int     read_bytes;
    FILE*   stream;

    memset(user_name, 0, sizeof(user_name));
    memset(pass, 0, 41);
    memset(user_pass, 0, sizeof(user_pass));
    stream = NULL;
    read_bytes = 0;
    stream = fopen("/home/users/level03/.pass", "r");
    if (!stream)
    {
        fwrite("ERROR: failed to open password file\n", 1, 36, stderr);
        exit(1);
    }
    read_bytes = fread(pass, 1, 41, stream);
    pass[strcspn(pass, "\n")] = '\0';
    if (read_bytes != 41)
    {
        fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
        fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
        exit(1);
    }
    fclose(stream);
    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\**************************************/");
    printf("--[ Username: ");
    fgets(user_name, 100, stdin);
    user_name[strcspn(user_name, "\n")] = '\0';
    printf("--[ Password: ");
    fgets(user_pass, 100, stdin);
    user_pass[strcspn(user_pass, "\n")] = '\0';
    puts("*****************************************");
    if (strncmp(pass, user_pass, 41))
    {
        printf(user_name);
        puts(" does not have access!");
        exit(1);
    }
    printf("Greetings, %s!\n", user_name);
    system("/bin/sh");
    return 0;
}