#include <cstdio>

class BaseClass {
public:
    virtual void draw() {
    }

};

class MyClass : public BaseClass {
public:
    void draw() override final {
    }
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

private:
    static void testFunc2() {
    }

};

int main() {
    return 0;
}
