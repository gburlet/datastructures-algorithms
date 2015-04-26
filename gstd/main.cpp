#include "vector.h"

#include <iostream>

struct A {
    A(int d = 0)
    : data(0) 
    {
        data = new int;
        *data = d;
    }

    ~A()
    {
        delete data;
        data = 0;
    }

    A& operator=(const A& x) {
        *data = *x.data;
        return *this;
    }

    int * data;
};

int main()
{
    gstd::vector<int> v;
    
    for (int i = 0; i < 5; i++) {
        v.push_back(i);
    }

    v.erase(v.end()-1);

    gstd::vector<int>::const_iterator it;
    for (it = v.begin(); it != v.end(); it++) {
        std::cout << *it << std::endl;
    }

    return 0;
}
