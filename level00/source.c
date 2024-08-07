int main(int argc, const char **argv, const char **envp)
{
    int pass;

    puts("***********************************");
    puts("* \t     -Level00 -\t\t  *");
    puts("***********************************");
    printf("Password:");
    scanf("%d", &pass);
    if (pass == 5276)
    {
        puts("\nAuthenticated!");
        system("/bin/sh");
        return (0);
    }
    else
    {
        puts("\nInvalid Password!");
        return (1);
    }
}