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

Decompiling with Hex-Rays on https://dogbolt.org/ shows that the correct serial number is obtained through a series of calculations. It depends on the login you suggest, but the idea is that you can put whatever you want as login, then apply the algorithm and retrieve its corresponding serial number as a result.

So the only thing we had to do was copypasting the algorithm from the source code, feed it the login of our choice (`avarnier`), and we got this serial number: `6234492`.

Here it is!
```Shell
level06@OverRide:~$ ./level06
***********************************
*               level06           *
***********************************
-> Enter Login: avarnier
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6234492
Authenticated!
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```

