# Пользовательский сценарий

Пример того, как пользователь описывает собственный класс набором флагов, не
привязываясь к языку. Тот же вызов `generateProgram` с другой фабрикой даёт
код на нужном языке.

```cpp
auto baseClass = factory.createClass("BaseClass");
baseClass->add(factory.createMethod("draw", "void", MethodUnit::VIRTUAL), ClassUnit::PUBLIC);

auto myClass = factory.createClass("MyClass", ClassUnit::ABSTRACT);
myClass->setParent("BaseClass");

myClass->add(factory.createMethod("draw", "void", MethodUnit::OVERRIDE | MethodUnit::SEALED), ClassUnit::PUBLIC);
myClass->add(factory.createMethod("testFunc6", "void", 0), ClassUnit::INTERNAL);
```

Ожидаемое соответствие модификаторов по языкам приведено в корневом `README.md`.
Сгенерированные эталоны — в файлах `MyClass.cpp`, `MyClass.cs`, `MyClass.java`.
