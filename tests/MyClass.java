public abstract class MyClass {
    public void draw() {
    }
    public void testFunc1() {
    }
    public void testFunc3() {
    }
    public abstract void testFunc5();
    protected static void testFunc4() {
        System.out.print("Hello, world!\n");
    }
    private static final void testFunc2() {
    }
    void testFunc6() {
    }
}

final class ChildClass extends MyClass {
    @Override
    public final void draw() {
    }
    @Override
    public void testFunc5() {
    }
}

class Program {
    public static void main(String[] args) {
    }
}
