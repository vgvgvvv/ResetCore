#include <cstdio>

#define GETTER(T, x) T get_ ## x() { return this->x; }

class Test{
public:
    Test(){};
    ~Test(){};
    int haha;
    GETTER(int, haha);
};

int main(){
    Test a;
    a.haha = 10;
    printf("haha = %d", a.get_haha());
    getchar();
    return 1;
}