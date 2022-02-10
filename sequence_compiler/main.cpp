#include <iostream>
#include "../sequence/sequence.h"

#define COMPILER_ERR  if (s.err_line()) { cout << s.err_line() << ":" << s.err() << endl; }

using namespace std;

int main(int argv,char *argc[])
{
    sequence s("C:\\test_sequence.ts");

    s.parse_vendor();
    COMPILER_ERR;
    s.print_vendor();

    s.parse_product();
    COMPILER_ERR;
    s.print_product();

    s.parse();
    s.print_include();
    s.print_tree();
    s.print_config();

    getchar();
}
