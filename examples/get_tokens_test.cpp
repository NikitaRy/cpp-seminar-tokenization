// Source - https://stackoverflow.com/a/77839521
// Posted by Scott McPeak, modified by community. See post 'Timeline' for change history
// Retrieved 2026-05-31, License - CC BY-SA 4.0

// test.cc
// Input for print-node-tokens.cc.

// Easy case.
int f()
{
  return 2;
}

// Some macro expansions.
#define PLUS +
int g(int x)
{
  #define ECKS x
  return ECKS PLUS 4;
}

// Macro expansions at start and end.
#define INTEGER int
#define RBRACE }
INTEGER h()
{
  return 5;
RBRACE

// The entire function is inside a macro expansion.  For this
// definition, we don't get any tokens, and it's not obvious how to
// improve that.
#define IDENT(x) x
IDENT(int j() { return 6; })

// EOF
