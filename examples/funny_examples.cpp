#include <vector>

#define R "x"
const char* s = R"y"; // Not "x" "y"

int x = 1Ex; // number token
int y = 0xe+foo; // 0xe + foo

std::vector<std::vector<int>> vector; // earlier you have to write std::vector<std::vector<int> >