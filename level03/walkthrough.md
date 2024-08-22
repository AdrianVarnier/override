level03 protections:
```Shell
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level03/level03
```

level03 rights:
```Shell
level03@OverRide:~$ ls -la level03 
-rwsr-s---+ 1 level04 users 7677 Sep 10  2016 level03
```

Testing the program shows that it is looking for a password:
```Shell
level03@OverRide:~$ ./level03
***********************************
*               level03         **
***********************************
Password:coucou

Invalid Password
```

Decompiling with Hex-Rays on https://dogbolt.org/ shows that the correct password is a value satisfying some conditions:
- first, your input will be subtracted from `322424845`
- the result is then used to `XOR` characters from a string =>  ```Q}|u`sfg~sf{}|a3```
- if your input equals 18 after the subtraction, the string will be successfully converted into `Congratulations` and you will open the shell.

How do we know that it is 18? Because the ASCII values of `Q` and `C` are 81 and 67. 81 XOR 67 = 18.
So, if you do 322424845 - 18, you obtain : `322424827`, which is our password.

```Shell
level03@OverRide:~$ ./level03
***********************************
*               level03         **
***********************************
Password:322424827
$ 
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```