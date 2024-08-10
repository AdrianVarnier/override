bool  auth(char *login, int serial)
{
  int i;
  int key;
  int len;

  login[strcspn(login, "\n")] = '\0';
  len = strnlen(login, 32);
  if (len <= 5)
    return 1;
  if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
  {
    puts("\x1B[32m.---------------------------.");
    puts("\x1B[31m| !! TAMPERING DETECTED !!  |");
    puts("\x1B[32m'---------------------------'");
    return 1;
  }
  else
  {
    key = (login[3] ^ 4919) + 6221293;
    for (i = 0; i < len; ++i)
    {
      if (login[i] <= 31)
        return (1);
      key += (key ^ (unsigned int)login[i]) % 1337;
    }
    return (serial != key);
  }
}

int main(int argc, const char **argv, const char **envp)
{
  int serial;
  char login[28];

  puts("***********************************");
  puts("*\t\tlevel06\t\t  *");
  puts("***********************************");
  printf("-> Enter Login: ");
  fgets(login, 32, stdin);
  puts("***********************************");
  puts("***** NEW ACCOUNT DETECTED ********");
  puts("***********************************");
  printf("-> Enter Serial: ");
  scanf("%d", &serial);
  if (auth(login, serial))
    return (1);
  puts("Authenticated!");
  system("/bin/sh");
  return (0);
}