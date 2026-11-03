using System;

public abstract class MyClass {
    public void testFunc1() {
    }
    public virtual void testFunc3() {
    }
    public abstract void testFunc5();
    protected static void testFunc4() {
        Console.Write("Hello, world!\n");
    }
    private static void testFunc2() {
    }
    internal void testFunc6() {
    }
    protected internal void testFunc7() {
    }
    private protected void testFunc8() {
    }
}

class Program {
    static void Main() {
    }
}
