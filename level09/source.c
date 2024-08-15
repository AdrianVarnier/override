user_t {
  char message[140];
  char name[40];
  int size;
}

int secret_backdoor()
{
  char buffer[128];

  fgets(buffer, 128, stdin);
  return system(buffer);
}

int handle_msg()
{
  user_t  user;

  set_username(user);
  set_msg(user);
  return puts(">: Msg sent!");
}

char *__fastcall set_msg(user_t user)
{
  char buffer[1024];

  memset(buffer, 0, sizeof(s));
  puts(">: Msg @Unix-Dude");
  printf(">>: ");
  fgets(buffer, 1024, stdin);
  return strncpy(user.message, buffer, user.size);
}

int set_username(int64_t user)
{
  char buffer[140];
  int i;

  memset(buffer, 0, 128);
  puts(">: Enter your username");
  printf(">>: ");
  fgets(buffer, 128, stdin);
  for (i = 0; i <= 40 && buffer[i]; ++i)
    user->name[i] = buffer[i];
  return printf(">: Welcome, %s", user.name);
}

int __fastcall main(int argc, const char **argv, const char **envp)
{
  puts(
    "--------------------------------------------\n"
    "|   ~Welcome to l33t-m$n ~    v1337        |\n"
    "--------------------------------------------");
  handle_msg();
  return 0;
}