using System;

public abstract class MyClass {
    public virtual void draw() {
    }
    public void testFunc1() {
    }
    public virtual void testFunc3() {
    }
    public abstract void testFunc5();
    internal void testFunc6() {
    }
    protected static void testFunc4() {
        Console.Write("Hello, world!\n");
    }
    private static void testFunc2() {
    }
}

internal sealed class ChildClass : MyClass {
    public override sealed void draw() {
    }
    public override void testFunc5() {
    }
}

class Program {
    static void Main() {
    }
}
