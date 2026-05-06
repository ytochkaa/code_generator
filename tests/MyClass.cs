using System;

class BaseClass {
    public virtual void draw() {
    }
}

abstract class MyClass : BaseClass {
    public override sealed void draw() {
    }
    public void testFunc1() {
    }
    public virtual void testFunc3() {
    }
    public abstract void testFunc5();
    internal void testFunc6() {
    }
    protected static void testFunc4() {
        Console.WriteLine("Hello, world!\n");
    }
    private static void testFunc2() {
    }
}

class Program {
    static void Main() {
    }
}
