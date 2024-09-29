int __cdecl main(int argc, const char **argv, const char **envp)
{
  _BYTE v4[132]; // [esp+18h] [ebp-90h] BYREF
  int v5; // [esp+9Ch] [ebp-Ch]

  v5 = fopen("/home/user/end/.pass", &unk_80486F0);
  memset(v4, 0, sizeof(v4));
  if ( !v5 || argc != 2 )
    return -1;
  fread(v4, 1, 66, v5);
  v4[65] = 0;
  v4[atoi(argv[1])] = 0;
  fread(&v4[66], 1, 65, v5);
  fclose(v5);
  if ( !strcmp(v4, argv[1]) )
    execl("/bin/sh", "sh", 0);
  else
    puts(&v4[66]);
  return 0;
}
