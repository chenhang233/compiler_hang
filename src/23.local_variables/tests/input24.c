int a;
int b;
int c;
int hang()
{
  a = 42;
  b = 19;
  printint(a & b);
  printint(a | b);
  printint(a ^ b);
  printint(1 << 3);
  printint(63 >> 3);
  return (0);
}
