#include <stdio.h>
#include <string.h>

int main() {
    char* s = "avarnier";
    int a = 0;

    a = (s[3] ^ 4919) + 6221293;
    for ( int i = 0; i < strlen(s); ++i )
    {
      if ( s[i] <= 31 )
        return 1;
      a += (a ^ (unsigned int)s[i]) % 1337;
    }
    printf("serial: %d", a);
    return 0;
}