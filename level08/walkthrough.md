Level08 protections:
```Shell
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Full RELRO      Canary found      NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level08/level08
```

level08 rights:
```Shell
level08@OverRide:~$ ls -la level08 
-rwsr-s---+ 1 level09 users 12975 Oct 19  2016 level08
```

```Shell
level08@OverRide:~$ ./level08 
Usage: ./level08 filename
ERROR: Failed to open (null)
level08@OverRide:~$ ./level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
```