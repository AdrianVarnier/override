//----- (000000000000088C) ----------------------------------------------------
int secret_backdoor()
{
  char s[128]; // [rsp+0h] [rbp-80h] BYREF

  fgets(s, 128, stdin);
  return system(s);
}

//----- (00000000000008C0) ----------------------------------------------------
int handle_msg()
{
  char buffer[140]; // [rsp+0h] [rbp-C0h] BYREF

  set_username((int64_t)buffer);
  set_msg((int64_t)buffer);
  return puts(">: Msg sent!");
}

//----- (0000000000000932) ----------------------------------------------------
char *__fastcall set_msg(int64_t a1)
{
  char s[1024]; // [rsp+10h] [rbp-400h] BYREF

  memset(s, 0, sizeof(s));
  puts(">: Msg @Unix-Dude");
  printf(">>: ");
  fgets(s, 1024, stdin);
  return strncpy((char *)a1, s, *(int *)(a1 + 180));
}

//----- (00000000000009CD) ----------------------------------------------------
int set_username(int64_t a1)
{
  char s[140]; // [rsp+10h] [rbp-90h] BYREF
  int i; // [rsp+9Ch] [rbp-4h]

  memset(s, 0, 128);
  puts(">: Enter your username");
  printf(">>: ");
  fgets(s, 128, stdin);
  for (i = 0;i <= 40 && s[i]; ++i)
    *(_BYTE *)(a1 + i + 140) = s[i];
  return printf(">: Welcome, %s", (const char *)(a1 + 140));
}
// 9CD: using guessed type char s[140];

//----- (0000000000000AA8) ----------------------------------------------------
int __fastcall main(int argc, const char **argv, const char **envp)
{
  puts(
    "--------------------------------------------\n"
    "|   ~Welcome to l33t-m$n ~    v1337        |\n"
    "--------------------------------------------");
  handle_msg();
  return 0;
}