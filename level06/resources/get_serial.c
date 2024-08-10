#include <stdio.h>
#include <string.h>

int main()
{
  char* login = "avarnier";
  int key = (login[3] ^ 4919) + 6221293;
  for (int i = 0; i < strlen(login); ++i)
  {
    if (login[i] <= 31)
      return 1;
    key += (key ^ (unsigned int)login[i]) % 1337;
  }
  printf("serial: %d", key);
  return 0;
}