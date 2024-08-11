int get_unum()
{
  unsigned int unum = 0;
  fflush(stdout);
  scanf("%u", &unum);
  clear_stdin();
  return unum;
}

int store_number(int arr_addr)
{
  unsigned int unum;
  unsigned int index;

  printf(" Number: ");
  unum = get_unum();
  printf(" Index: ");
  index = get_unum();
  if (index == 3 * (index / 3) || HIBYTE(unum) == 183)
  {
    puts(" *** ERROR! ***");
    puts("   This index is reserved for wil!");
    puts(" *** ERROR! ***");
    return 1;
  }
  else
  {
    *(_DWORD *)(arr_addr + 4 * index) = unum;
    return 0;
  }
}

int read_number(int arr_addr)
{
  int unum;

  printf(" Index: ");
  unum = get_unum();
  printf(" Number at data[%u] is %u\n", unum, *(_DWORD *)(arr_addr + 4 * unum));
  return 0;
}

int main(int argc, const char **argv, const char **envp)
{
  unsigned int  arr[100];
  char          buffer[20];
  int           ret = 0;

  bzero(buffer, 20);
  memset(arr, 0, sizeof(arr));
  while (*argv)
  {
    memset((void *)*argv, 0, strlen(*argv));
    ++argv;
  }
  while (*envp)
  {
    memset((void *)*envp, 0, strlen(*envp));
    ++envp;
  }
  puts(
    "----------------------------------------------------\n"
    "  Welcome to wil's crappy number storage service!   \n"
    "----------------------------------------------------\n"
    " Commands:                                          \n"
    "    store - store a number into the data storage    \n"
    "    read  - read a number from the data storage     \n"
    "    quit  - exit the program                        \n"
    "----------------------------------------------------\n"
    "   wil has reserved some storage :>                 \n"
    "----------------------------------------------------\n");
  while ( 1 )
  {
    printf("Input command: ");
    ret = 1;
    fgets(buffer, 20, stdin);
    s[strlen(s) - 1] = 0;
    if (!memcmp(s, "store", 5u))
      ret = store_number((int)arr);
    else if (!memcmp(s, "read", 4u))
      ret = read_number((int)arr);
    else if (!memcmp(s, "quit", 4u))
      return 0;
    if (ret)
      printf(" Failed to do %s command\n", buffer);
    else
      printf(" Completed %s command successfully\n", buffer);
    bzero(buffer, 20);
  }
}