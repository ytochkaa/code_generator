#include <cstdio>

class MyClass {
public:
    virtual void draw() {
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

class ChildClass final : public MyClass {
public:
    void draw() override final {
    }
    void testFunc5() override {
    }

};

int main() {
    return 0;
}
