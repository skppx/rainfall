# Solution

Here is the decompilation of the binary in C:

```c
int main(int ac, char **av)
{
    int     uid;
    int     gid;
    char    *args[2];

    if (atoi(av[1]) == 423)
    {
        args[0] = strdup("/bin/sh");
        args[1] = 0;

        uid = geteuid();
        gid = getegid();
        setresgid(gid, gid, gid);
        setresuid(uid, uid, uid);
        execv("/bin/sh", args);
    }
    else
    {
        fwrite("No !\n", 1, 5, stderr);
    }
    return (0);
}
```

If we look at the program it atoi the first arguments compare it,
if the value is 423 it will run a shell else it will run fwrite and will output "No !\n"

So to pass the level0 it's pretty simple we just need to input 423

```sh
level0@RainFall:~$ ./level0 423
$ whoami
level1
$ cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```
