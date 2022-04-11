#include <iostream>
#include "../sequence/sequence.h"

#define COMPILER_ERR  if (s.err_line()) { cout << s.err_line() << ":" << s.err() << endl; }

using namespace std;

int main(int argv,char *argc[])
{
    sequence s("C:\\test_sequence.ts");

    s.compile();
    COMPILER_ERR;

    s.print_plugin();
    s.print_vendor();
    s.print_product();
    s.print_include();
    s.print_tree();
    s.print_config();

    getchar();
}
