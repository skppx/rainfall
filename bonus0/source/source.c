int p(int a1, int a2)
{
  char v3[4104]; // [esp+10h] [ebp-1008h] BYREF

  puts(a2);
  read(0, v3, 4096);
  *strchr(v3, 10) = 0;
  return strncpy(a1, v3, 20);
}

int pp(char *a1)
{
  char v2[20]; // [esp+28h] [ebp-30h] BYREF
  char v3[28]; // [esp+3Ch] [ebp-1Ch] BYREF

  p((int)v2, " - ");
  p((int)v3, " - ");
  strcpy(a1, v2);
  a1[strlen(a1)] = " ";
  return strcat(a1, v3);
}

int main(int argc, char **argv)
{

  char buf[42]; // [esp+16h] [ebp-2Ah] BYREF

  pp(buf);
  puts(buf);
  return 0;
}
