#include <stdbool.h>

char user_name[100];

bool verify_user_name()
{
    puts("verifying username....\n");
    return (memcmp(user_name, "dat_wil", 7) != 0);
}

bool verify_user_pass(char* s)
{
    return (memcmp(s, "admin", 5) != 0);
}

int main(int argc, const char **argv, const char **envp)
{
    char s[64];
    bool is_invalid;

    memset(s, 0, sizeof(s));
    is_invalid = 0;
    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(user_name, 256, 0);
    is_invalid = verify_username();
    if (is_invalid)
        puts("nope, incorrect username...\n");
    else
    {
        puts("Enter Password: ");
        fgets(s, 100, 0);
        is_invalid = verify_user_pass(s);
        puts("nope, incorrect password...\n");
    }
    return (1);
}
