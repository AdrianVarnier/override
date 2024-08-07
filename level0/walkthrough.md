level00 protections:
```Shell
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level00/level00
```

level00 rights:
```Shell
level00@OverRide:~$ ls -la level00 
-rwsr-s---+ 1 level01 users 7280 Sep 10  2016 level00
```

When it is executed, the program asks for a password and exits if it is incorrect.
```Shell
level00@OverRide:~$ ./level00 
***********************************
*            -Level00 -           *
***********************************
Password:mypass

Invalid Password!
```

Decompiling with Hex-Rays on https://dogbolt.org/ shows two things:
- the program use `scanf()` to retrieve our input with `%d` to stock it in an int
- and if it is equal to 5276, open a shell

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