/*
    Try to run: gcc -E -fdebug-cpp
    and see how the code was broken up
*/
#define MACRO 5
int a = 0;
int b = MACRO;
int c = a + b;
char d = 'd';