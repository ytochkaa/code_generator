# Тестирование

В папке лежат эталонные результаты генерации (`MyClass.cpp`, `MyClass.cs`, `MyClass.java`),
автотест проверки модификаторов (`validation_tests.cpp`) и описание входных данных
([`user_example.md`](user_example.md)).

Все команды ниже выполняются из папки `build`, в которой собран проект.

## Пользовательские тест-кейсы

### Case №1
**Проверка сборки проекта**

Входные параметры: -

- Шаг 1 — выполнить `cmake ..` в папке `build`
- Шаг 2 — выполнить `cmake --build .`

Результат: собираются две программы — `code_generator` и `validation_tests`, ошибок и предупреждений нет

---

### Case №2
**Проверка генерации кода на трёх языках**

Входные параметры: дерево классов из `generateProgram` (см. [`user_example.md`](user_example.md))

- Шаг 1 — запустить приложение `./code_generator`

Результат: в консоль выводится код на C++, C# и Java; в папке `generated` создаются файлы
`MyClass.cpp`, `MyClass.cs`, `MyClass.java`; код возврата программы — `0`

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

- Шаг 1 — в папке `generated` выполнить `csc MyClass.cs`
  (компилятор .NET Framework: `C:\Windows\Microsoft.NET\Framework64\v4.0.30319\csc.exe`)
- Шаг 2 — запустить `MyClass.exe`

Результат: файл компилируется без ошибок, программа завершается с кодом `0`

Вариант с .NET SDK: создать проект `dotnet new console -o cs_check`, заменить `cs_check/Program.cs`
файлом `generated/MyClass.cs` и выполнить `dotnet build cs_check`.

---

### Case №6
**Проверка компиляции сгенерированного кода на Java**

Входные параметры: файл `generated/MyClass.java`

- Шаг 1 — в папке `generated` выполнить `javac MyClass.java`
- Шаг 2 — выполнить `java Program`

Результат: файл компилируется без ошибок (имя файла совпадает с публичным классом `MyClass`),
программа завершается с кодом `0`

---

### Case №7
**Проверка недопустимых сочетаний модификаторов**

Входные параметры: набор заведомо ошибочных и корректных сочетаний из `validation_tests.cpp`

- Шаг 1 — запустить `ctest --output-on-failure` (или `./validation_tests`)

Результат: для каждого ошибочного сочетания выводится `[OK]` с текстом ошибки
(например, `C#: метод method: virtual нельзя сочетать с abstract или override`),
для корректных — `[OK]` без ошибки; в конце — `Все проверки пройдены`, тест `validation_tests` пройден

---

### Case №8
**Проверка реакции генератора на ошибку в модификаторах**

Входные параметры: в `generateProgram` у метода `testFunc2` флаги заменены на
`MethodUnit::STATIC | MethodUnit::VIRTUAL`

- Шаг 1 — удалить папку `generated`
- Шаг 2 — пересобрать проект и запустить `./code_generator`

Результат: для каждого языка выводится сообщение вида
`Ошибка генерации MyClass.cpp: C++: метод testFunc2: static нельзя сочетать с virtual, abstract, override или sealed`;
файлы в `generated` не создаются, код возврата — `1`

---

### Case №9
**Проверка отбрасывания модификаторов, которых нет в языке**

Входные параметры: в `generateProgram` у метода `testFunc1` флаги заменены на `MethodUnit::ASYNC`

- Шаг 1 — пересобрать проект и запустить `./code_generator`

Результат: в C# метод генерируется как `public async void testFunc1()`, в C++ и Java — без `async`;
все три файла компилируются (Case №4–6); компилятор C# выдаёт только предупреждение CS1998
(в async-методе нет `await`)
