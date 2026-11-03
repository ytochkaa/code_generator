#include <cstdio>

class MyClass {
public:
    void testFunc1() {
    }
    virtual void testFunc3() const {
    }
    virtual void testFunc5() = 0;
    void testFunc6() {
    }

protected:
    static void testFunc4() {
        printf("Hello, world!\n");
    }
    void testFunc7() {
    }
    void testFunc8() {
    }

private:
    static void testFunc2() {
    }
};

int main() {
    return 0;
}
