Level06 protections

```Shell
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level06/level06
```

Level06 rights:
```Shell
-rwsr-s---+ 1 level07 users   7907 Sep 10  2016 level06
```

Testing the binary:
```Shell
level06@OverRide:~$ ./level06
***********************************
*               level06           *
***********************************
-> Enter Login: lrandria
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: whatisserial
```