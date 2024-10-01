int greetuser(char a1)
{
  _DWORD v2[4]; // [esp+10h] [ebp-48h] BYREF
  __int16 v3; // [esp+20h] [ebp-38h]
  char v4; // [esp+22h] [ebp-36h]

  switch ( language )
  {
    case 1:
      v2[0] = unk_8048717;
      v2[1] = *((_DWORD *)&unk_8048717 + 1);
      v2[2] = *((_DWORD *)&unk_8048717 + 2);
      v2[3] = *((_DWORD *)&unk_8048717 + 3);
      v3 = *((_WORD *)&unk_8048717 + 8);
      v4 = *((_BYTE *)&unk_8048717 + 18);
      break;
    case 2:
      strcpy((char *)v2, "Goedemiddag! ");
      break;
    case 0:
      strcpy((char *)v2, "Hello ");
      break;
  }
  strcat(v2, &a1);
  return puts(v2);
}

int main(int argc, const char **argv, const char **envp)
{
  char v4[76]; // [esp+0h] [ebp-ACh] BYREF
  _BYTE v5[76]; // [esp+50h] [ebp-5Ch] BYREF
  int v6; // [esp+9Ch] [ebp-10h]

  if ( argc != 3 )
    return 1;
  memset(v5, 0, sizeof(v5));
  strncpy(v5, argv[1], 40);
  strncpy(&v5[40], argv[2], 32);
  v6 = getenv("LANG");
  if ( v6 )
  {
    if ( !memcmp(v6, "fi", 2) )
    {
      language = 1;
    }
    else if ( !memcmp(v6, "nl", 2) )
    {
      language = 2;
    }
  }
  qmemcpy(v4, v5, sizeof(v4));
  return greetuser(v4[0]);
}
