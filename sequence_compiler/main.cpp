#include <iostream>
#include "../sequence/sequence.h"

#define COMPILER_ERR  if (s.err_line()) { cout << s.err_line() << ":" << s.err() << endl; }

using namespace std;

int main()
{
    sequence s("C:\\test_sequence.ts");

    list<string>::const_iterator iter;

    s.parse_vendor();
    COMPILER_ERR;
    cout << "Vendor : " << s.vendor << endl;

    s.parse_product();
    COMPILER_ERR;

    for (iter = s.product.cbegin();iter != s.product.cend();++iter) {
        cout << "Product : " << *iter << endl;
    }

    s.parse();

    getchar();
}
