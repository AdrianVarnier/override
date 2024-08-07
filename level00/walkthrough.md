Level00 protections:
```Shell
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level00/level00
```

Level00 rights:
```Shell
level00@OverRide:~$ ls -la level00 
-rwsr-s---+ 1 level01 users 7280 Sep 10  2016 level00
```

When executed, the program asks for a password and exits if it is incorrect.
```Shell
level00@OverRide:~$ ./level00 
***********************************
*            -Level00 -           *
***********************************
Password:mypass

Invalid Password!
```

Decompiling with Hex-Rays on https://dogbolt.org/ shows two things:
- the program parses argv[1] with `scanf()` and compares it with a value
- if equal to 5276, it opens a shell

```Shell
level00@OverRide:~$ ./level00
***********************************
*            -Level00 -           *
***********************************
Password:5276

Authenticated!
$ cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```