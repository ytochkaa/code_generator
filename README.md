# Документация к лабораторной работе: генерация кода

Консольное приложение на C++, генерирующее исходный код классов на C++, C# и Java. Разработано в рамках курса «Технологии программирования» (4 курс, 2 семестр).

## Описание

Программа строит дерево элементов кода (класс → методы → операторы) и по нему генерирует законченную программу на выбранном языке. Целевой язык задаётся не на этапе компиляции, а во время выполнения — выбором конкретной фабрики. Для решения задачи применён паттерн «Абстрактная фабрика».

Исходный код генератора из задания размещён первым коммитом репозитория.

## Требования по заданию

1. Расширить исходную реализацию, чтобы генерировать программы не только на C++, но и на C# и Java.
2. Сгенерированные программы должны без ошибок компилироваться компилятором своего языка.
3. Добавить модификаторы классов и методов, которых нет в C++, но есть в C# и Java (`synchronized`, `transient`, `volatile` можно не включать).
4. Применить паттерн «Абстрактная фабрика».

## Технологии

- C++17
- CMake 3.16
- MinGW-w64 (GCC)

## Структура проекта

```
code_generator/
├── main.cpp               точка входа: generateProgram и генерация на трёх языках
├── unit.h                 базовый элемент синтаксиса Unit
├── classunit.h            абстрактный элемент «класс»
├── methodunit.h           абстрактный элемент «метод»
├── printoperatorunit.h    абстрактный элемент «оператор печати»
├── factory.h              абстрактная фабрика Factory
├── cppfactory.h/.cpp      элементы и фабрика C++
├── csharpfactory.h/.cpp   элементы и фабрика C#
├── javafactory.h/.cpp     элементы и фабрика Java
└── tests/                 эталоны, автотест и тест-кейсы
```

## Архитектура

- Абстрактные элементы
  - `Unit` — базовый элемент: `add` (добавление вложенного элемента), `compile` (генерация кода), модификатор доступа элемента
  - `ClassUnit` — класс: секции по модификаторам доступа, модификаторы класса, базовый класс, общие проверки модификаторов
  - `MethodUnit` — метод: имя, тип, модификаторы, тело, общие проверки модификаторов
  - `PrintOperatorUnit` — оператор печати строки
- Абстрактная фабрика
  - `Factory` — интерфейс создания элементов (`createClass`, `createMethod`, `createPrintOperator`) и обёртки программы (`wrapProgram`, `fileName`)
- C++: `CppFactory`, `CppClassUnit`, `CppMethodUnit`, `CppPrintOperatorUnit`
- C#: `CSharpFactory`, `CSharpClassUnit`, `CSharpMethodUnit`, `CSharpPrintOperatorUnit`
- Java: `JavaFactory`, `JavaClassUnit`, `JavaMethodUnit`, `JavaPrintOperatorUnit`

### Абстрактная фабрика

Функция `generateProgram(const Factory&)` собирает дерево элементов только через интерфейс `Factory` и абстрактные элементы и **не знает целевого языка**. Конкретная фабрика создаёт элементы своего языка, а каждый элемент сам генерирует код в синтаксисе этого языка:

| Фабрика         | Создаёт элементы | Обёртка программы                         | Файл           |
|-----------------|------------------|-------------------------------------------|----------------|
| `CppFactory`    | `Cpp*`           | `#include <cstdio>` и `int main()`        | `MyClass.cpp`  |
| `CSharpFactory` | `CSharp*`        | `using System;` и `class Program { Main }`| `MyClass.cs`   |
| `JavaFactory`   | `Java*`          | `class Program { main }`                  | `MyClass.java` |

### Модификаторы

Флаги задаются один раз для всех языков; каждая фабрика переводит их в синтаксис своего языка, а модификаторы, которых в языке нет, отбрасывает.

**Модификаторы доступа** (`ClassUnit::AccessModifier`) — для членов класса и для самого класса:

| Флаг                 | C++          | C#                   | Java              |
|----------------------|--------------|----------------------|-------------------|
| `PUBLIC`             | `public:`    | `public`             | `public`          |
| `PROTECTED`          | `protected:` | `protected`          | `protected`       |
| `PRIVATE`            | `private:`   | `private`            | `private`         |
| `INTERNAL`           | `public:`    | `internal`           | package-private   |
| `PROTECTED_INTERNAL` | `protected:` | `protected internal` | `protected`       |
| `PRIVATE_PROTECTED`  | `protected:` | `private protected`  | `private`         |
| `PACKAGE_PRIVATE`    | `private:`   | `internal`           | package-private   |

Модификатор доступа класса верхнего уровня задаётся через `setAccessModifier`; в C++ он не выводится.

**Модификаторы класса** (`ClassUnit::Modifier`):

| Флаг       | C++     | C#        | Java                         |
|------------|---------|-----------|------------------------------|
| `STATIC`   | -       | `static`  | `static` (только вложенный)  |
| `ABSTRACT` | -       | `abstract`| `abstract`                   |
| `FINAL`    | `final` | `sealed`  | `final`                      |
| `PARTIAL`  | -       | `partial` | -                            |

**Модификаторы метода** (`MethodUnit::Modifier`):

| Флаг       | C++                       | C#         | Java        |
|------------|---------------------------|------------|-------------|
| `STATIC`   | `static`                  | `static`   | `static`    |
| `CONST`    | `const`                   | -          | -           |
| `VIRTUAL`  | `virtual`                 | `virtual`  | - (методы виртуальны по умолчанию) |
| `ABSTRACT` | `virtual ... = 0`         | `abstract` | `abstract`  |
| `OVERRIDE` | `override`                | `override` | `@Override` |
| `SEALED`   | `final`                   | `sealed`   | `final`     |
| `FINAL`    | `final` (только у virtual)| -          | `final`     |
| `ASYNC`    | -                         | `async`    | -           |

Модификаторы, которых нет в C++: `internal`, `protected internal`, `private protected`, package-private, `abstract`/`sealed`/`static`/`partial` у класса, `sealed` и `async` у метода, `@Override`.

### Проверка сочетаний модификаторов

Чтобы сгенерированный код всегда компилировался, `compile` проверяет сочетания модификаторов по правилам целевого языка и при ошибке выбрасывает `std::logic_error` с описанием. `main` выводит сообщение и не создаёт файл для этого языка.

| Правило                                                         | C++ | C#  | Java |
|-----------------------------------------------------------------|-----|-----|------|
| `static` нельзя сочетать с `virtual`/`abstract`/`override`/`sealed` |  +  |  +  |  +   |
| `abstract` нельзя сочетать с `sealed`/`final` (метод и класс)   |  +  |  +  |  +   |
| `sealed` у метода только вместе с `override`                    |  +  |  +  |  +   |
| `override` только в классе с базовым классом                    |  +  |  +  |  +   |
| статический метод не может быть `const`                         |  +  |     |      |
| абстрактный метод только в `abstract`-классе                    |     |  +  |  +   |
| `virtual` нельзя сочетать с `abstract`/`override`               |     |  +  |      |
| `virtual`/`abstract`/`override` не могут быть `private`         |     |  +  |  +   |
| в `sealed`-классе нет `virtual`-методов                         |     |  +  |      |
| `static`-класс: только `static`-методы, без `protected`, без базового класса, не `abstract`/`sealed` | | + | |
| `async` не у `abstract`-метода, тип — `void`, `Task` или `Task<T>` |  |  +  |      |
| класс верхнего уровня только `public`/`internal` (package-private в Java) |  |  +  |  +   |
| `static` только у вложенного класса                             |     |     |  +   |

Не проверяются: наличие переопределяемого метода в базовом классе, согласованность доступа базового и производного класса в C#, единственность публичного класса в Java-файле.

### Как добавить новый язык

1. Создать наследников `ClassUnit`, `MethodUnit`, `PrintOperatorUnit` и реализовать в них `compile` (с проверками модификаторов своего языка).
2. Создать наследника `Factory`, который создаёт эти элементы и оборачивает программу.
3. Добавить фабрику в список `factories` в `main`.

`generateProgram` при этом не меняется.

## Сборка

### CMake

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

Собираются две программы: генератор `code_generator` и автотест `validation_tests`.

## Использование

```bash
./code_generator
```

Программа печатает сгенерированный код на трёх языках в консоль и сохраняет его в папку `generated` текущего каталога (`MyClass.cpp`, `MyClass.cs`, `MyClass.java`). При недопустимом сочетании модификаторов выводится `Ошибка генерации <файл>: <язык>: ...`, файл не создаётся, код возврата — `1`.

Входные данные генерации задаются в функции `generateProgram` в `main.cpp`. Сообщения выводятся в UTF-8; в консоли Windows перед запуском выполнить `chcp 65001`.

## Диаграмма классов

```mermaid
classDiagram
    class Unit {
        <<abstract>>
        +NO_ACCESS : int
        #m_accessModifier : int
        +add(unit : Unit, flags : Flags) void
        +compile(level : unsigned int) string*
        +setAccessModifier(accessModifier : int) void
        +accessModifier() int
        #generateShift(level : unsigned int) string
    }

    class ClassUnit {
        <<abstract>>
        #m_name : string
        #m_flags : Flags
        #m_parent : string
        #m_fields : map~int, Fields~
        +ClassUnit(name : string, flags : Flags)
        +setParent(parent : string) void
        +add(unit : Unit, flags : Flags) void
        #checkCommonFlags(language : string) void
        #methods() vector~MethodUnit~
    }

    class MethodUnit {
        <<abstract>>
        #m_name : string
        #m_returnType : string
        #m_flags : Flags
        #m_body : vector~Unit~
        +MethodUnit(name : string, returnType : string, flags : Flags)
        +add(unit : Unit, flags : Flags) void
        +name() string
        +flags() Flags
        #checkCommonFlags(language : string) void
        #compileBody(level : unsigned int) string
    }

    class PrintOperatorUnit {
        <<abstract>>
        #m_text : string
        +PrintOperatorUnit(text : string)
    }

    class Factory {
        <<interface>>
        +createClass(name : string, flags : Flags) ClassUnit
        +createMethod(name : string, returnType : string, flags : Flags) MethodUnit
        +createPrintOperator(text : string) PrintOperatorUnit
        +wrapProgram(classCode : string) string
        +fileName() string
    }

    class CppFactory
    class CSharpFactory
    class JavaFactory

    class CppClassUnit {
        +compile(level : unsigned int) string
    }
    class CSharpClassUnit {
        +accessModifierToString(accessModifier : int) string$
        +compile(level : unsigned int) string
        -check(level : unsigned int) void
    }
    class JavaClassUnit {
        +accessModifierToString(accessModifier : int) string$
        +compile(level : unsigned int) string
        -check(level : unsigned int) void
    }

    class CppMethodUnit {
        +compile(level : unsigned int) string
    }
    class CSharpMethodUnit {
        +compile(level : unsigned int) string
        -check() void
    }
    class JavaMethodUnit {
        +compile(level : unsigned int) string
        -check() void
    }

    class CppPrintOperatorUnit {
        +compile(level : unsigned int) string
    }
    class CSharpPrintOperatorUnit {
        +compile(level : unsigned int) string
    }
    class JavaPrintOperatorUnit {
        +compile(level : unsigned int) string
    }

    Unit <|-- ClassUnit
    Unit <|-- MethodUnit
    Unit <|-- PrintOperatorUnit
    ClassUnit "1" o-- "0..*" Unit : m_fields
    MethodUnit "1" o-- "0..*" Unit : m_body

    ClassUnit <|-- CppClassUnit
    ClassUnit <|-- CSharpClassUnit
    ClassUnit <|-- JavaClassUnit
    MethodUnit <|-- CppMethodUnit
    MethodUnit <|-- CSharpMethodUnit
    MethodUnit <|-- JavaMethodUnit
    PrintOperatorUnit <|-- CppPrintOperatorUnit
    PrintOperatorUnit <|-- CSharpPrintOperatorUnit
    PrintOperatorUnit <|-- JavaPrintOperatorUnit

    Factory <|.. CppFactory
    Factory <|.. CSharpFactory
    Factory <|.. JavaFactory
    CppFactory ..> CppClassUnit : create
    CppFactory ..> CppMethodUnit : create
    CppFactory ..> CppPrintOperatorUnit : create
    CSharpFactory ..> CSharpClassUnit : create
    CSharpFactory ..> CSharpMethodUnit : create
    CSharpFactory ..> CSharpPrintOperatorUnit : create
    JavaFactory ..> JavaClassUnit : create
    JavaFactory ..> JavaMethodUnit : create
    JavaFactory ..> JavaPrintOperatorUnit : create
```

Обозначения: `<|--` — обобщение (наследование), `<|..` — реализация интерфейса,
`o--` — агрегация (элемент хранит вложенные элементы), `..>` — зависимость (фабрика создаёт элемент);
в кавычках указана кратность концов связей.

## Тестирование

Пользовательские тест-кейсы, эталонные результаты и автотест описаны в [`tests/README.md`](tests/README.md).
