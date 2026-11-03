# Тестирование

В папке лежат эталонные результаты генерации: `MyClass.cpp`, `MyClass.cs`, `MyClass.java`.

## Входные данные

Класс описывается в `generateProgram` набором флагов, не привязанных к языку:

| Элемент     | Модификаторы                | Что проверяет                                   |
|-------------|-----------------------------|-------------------------------------------------|
| `MyClass`   | `public`, `abstract`        | модификатор доступа и abstract у класса         |
| `testFunc1` | `public`                    | обычный метод                                   |
| `testFunc2` | `private static final`      | final у метода (в C++ и C# отбрасывается)       |
| `testFunc3` | `public virtual const`      | const есть только в C++, virtual — в C++ и C#   |
| `testFunc4` | `protected static`          | оператор печати в теле метода                   |
| `testFunc5` | `public abstract`           | абстрактный метод без тела                      |
| `testFunc6` | `internal`                  | `internal` в C#, package-private в Java         |
| `testFunc7` | `protected internal`        | `protected internal` в C#                       |
| `testFunc8` | `private protected`         | `private protected` в C#                        |

Все команды ниже выполняются из папки `build`, в которой собран проект.

## Пользовательские тест-кейсы

### Case №1
**Проверка сборки проекта**

Входные параметры: -

- Шаг 1 — выполнить `cmake ..` в папке `build`
- Шаг 2 — выполнить `cmake --build .`

Результат: программа `code_generator` собирается без ошибок и предупреждений

---

### Case №2
**Проверка генерации кода на трёх языках**

Входные параметры: класс из раздела «Входные данные»

- Шаг 1 — запустить приложение `./code_generator`

Результат: в консоль выводится код на C++, C# и Java; в папке `generated` создаются файлы
`MyClass.cpp`, `MyClass.cs`, `MyClass.java`

---

### Case №3
**Проверка совпадения с эталонами**

Входные параметры: файлы из Case №2

- Шаг 1 — выполнить `diff generated/MyClass.cpp ../tests/MyClass.cpp`
- Шаг 2 — выполнить `diff generated/MyClass.cs ../tests/MyClass.cs`
- Шаг 3 — выполнить `diff generated/MyClass.java ../tests/MyClass.java`

Результат: различий нет

---

### Case №4
**Проверка компиляции сгенерированного кода на C++**

Входные параметры: файл `generated/MyClass.cpp`

- Шаг 1 — выполнить `g++ -std=c++17 -Wall -Wextra -pedantic generated/MyClass.cpp -o MyClass`
- Шаг 2 — запустить `./MyClass`

Результат: файл компилируется без ошибок и предупреждений, программа завершается с кодом `0`

---

### Case №5
**Проверка компиляции сгенерированного кода на C#**

Входные параметры: файл `generated/MyClass.cs`

- Шаг 1 — создать проект `dotnet new console -o cs_check`
- Шаг 2 — заменить `cs_check/Program.cs` файлом `generated/MyClass.cs`
- Шаг 3 — выполнить `dotnet build cs_check`

Результат: проект собирается без ошибок

Модификатор `private protected` появился в C# 7.2, поэтому нужен .NET SDK; компилятор
`csc.exe` из .NET Framework (C# 5) его не поддерживает.

---

### Case №6
**Проверка компиляции сгенерированного кода на Java**

Входные параметры: файл `generated/MyClass.java`

- Шаг 1 — в папке `generated` выполнить `javac MyClass.java`

Результат: файл компилируется без ошибок (имя файла совпадает с публичным классом `MyClass`)
