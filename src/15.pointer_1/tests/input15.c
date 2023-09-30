void test()
{
  printint(22);
}

int main()
{
  char a;
  char *b;
  char c;

  int d;
  int *e;
  int f;

  a = 18;
  printint(a);
  b = &a;
  c = *b;
  printint(c);

  d = 12;
  printint(d);
  e = &d;
  f = *e;
  printint(f);

  test(1);
  return (0);
}
