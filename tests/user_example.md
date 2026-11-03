# Входные данные генератора

Пользователь описывает классы набором флагов, не привязываясь к языку. Тот же вызов
`generateProgram` с другой фабрикой даёт код на нужном языке.

```cpp
auto myClass = factory.createClass("MyClass", ClassUnit::ABSTRACT);
myClass->setAccessModifier(ClassUnit::PUBLIC);

myClass->add(factory.createMethod("draw", "void", MethodUnit::VIRTUAL), ClassUnit::PUBLIC);
myClass->add(factory.createMethod("testFunc1", "void", 0), ClassUnit::PUBLIC);
myClass->add(factory.createMethod("testFunc2", "void", MethodUnit::STATIC | MethodUnit::FINAL), ClassUnit::PRIVATE);
myClass->add(factory.createMethod("testFunc3", "void", MethodUnit::VIRTUAL | MethodUnit::CONST), ClassUnit::PUBLIC);

auto testFunc4 = factory.createMethod("testFunc4", "void", MethodUnit::STATIC);
testFunc4->add(factory.createPrintOperator(R"(Hello, world!\n)"));
myClass->add(testFunc4, ClassUnit::PROTECTED);

myClass->add(factory.createMethod("testFunc5", "void", MethodUnit::ABSTRACT), ClassUnit::PUBLIC);
myClass->add(factory.createMethod("testFunc6", "void", 0), ClassUnit::INTERNAL);

auto childClass = factory.createClass("ChildClass", ClassUnit::FINAL);
childClass->setAccessModifier(ClassUnit::INTERNAL);
childClass->setParent("MyClass");

childClass->add(factory.createMethod("draw", "void", MethodUnit::OVERRIDE | MethodUnit::SEALED), ClassUnit::PUBLIC);
childClass->add(factory.createMethod("testFunc5", "void", MethodUnit::OVERRIDE), ClassUnit::PUBLIC);
```

| Элемент      | Модификаторы                   | Что проверяет                                         |
|--------------|--------------------------------|-------------------------------------------------------|
| `MyClass`    | `public`, `abstract`           | модификаторы доступа и abstract у класса              |
| `draw`       | `virtual`                      | виртуальный метод базового класса                     |
| `testFunc1`  | -                              | обычный публичный метод                               |
| `testFunc2`  | `private static final`         | final у метода (в C++ и C# отбрасывается)             |
| `testFunc3`  | `virtual const`                | const есть только в C++                               |
| `testFunc4`  | `protected static`             | оператор печати в теле метода                         |
| `testFunc5`  | `abstract`                     | абстрактный метод без тела                            |
| `testFunc6`  | `internal`                     | internal (в Java — package-private)                   |
| `ChildClass` | `internal`, `final`            | sealed-класс в C#, final-класс в C++ и Java           |
| `draw`       | `override sealed`              | запрет дальнейшего переопределения метода             |
| `testFunc5`  | `override`                     | реализация абстрактного метода (`@Override` в Java)   |

Ожидаемый результат для каждого языка — эталонные файлы `MyClass.cpp`, `MyClass.cs`,
`MyClass.java` в этой папке.
