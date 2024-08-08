int decrypt(char c)
{
  char  s[29];

  s[17] = '\0';
  strcpy(s, "Q}|u`sfg~sf{}|a3");
  for (int i = 0; i < strlen(s); ++i)
    s[i] ^= c;
  if ( !strcmp(c, "Congratulations!") )
    return system("/bin/sh");
  else
    return puts("\nInvalid Password");
}

int test(int x, int y)
{
  int result = x - y;
  switch (result)
  {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
      decrypt(result);
      break;
    default:
      result  = rand();
      decrypt(result);
      break;
  }
  return (result);
}

int main(int argc, const char **argv, const char **envp)
{
  unsigned int time;
  int x;

  time = time(0);
  srand(time);
  puts("***********************************");
  puts("*\t\tlevel03\t\t**");
  puts("***********************************");
  printf("Password:");
  scanf("%d", &x);
  test(x, 322424845);
  return 0;
}