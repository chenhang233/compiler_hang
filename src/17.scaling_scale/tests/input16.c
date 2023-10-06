int c;
int d;
int *e;
int f;
char g;

int main()
{
  c = 12;
  d = 18;
  printint(c);
  e = &c + 1;
  f = *e;
  printint(f);
  g = 150;
  return (0);
}
