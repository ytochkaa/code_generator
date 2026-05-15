class BaseClass {
    public void draw() {
    }
}

abstract class MyClass extends BaseClass {
    public void draw() {
    }
    public void testFunc1() {
    }
    public void testFunc3() {
    }
    public abstract void testFunc5();
    protected static void testFunc4() {
        System.out.println("Hello, world!\n");
    }
    private static final void testFunc2() {
    }
    void testFunc6() {
    }
}
