# Базовое модульное тестирование (Basic Unit Testing)

Цель лабораторной работы -- познакомиться с принципами модульного (unit) тестирования на C++ без использования сторонних фреймворков: научиться отделять тестируемую логику от точки входа приложения, писать тестовые функции и собирать их в отдельный исполняемый файл-раннер.

Перед выполнением работы необходимо настроить окружение по мануалу [Настройка окружения под Windows/Linux](../../EnvironmentTest/manual/environment.md).

## Подготовка

### Переменная окружения WORKSDK

Корневой CMakeLists.txt проверяет наличие переменной окружения **WORKSDK**. Если она не задана, конфигурация завершится ошибкой:---
------------
```
CMake Error at CMakeLists.txt:11 (message):
  Environment variable WORKSDK is not set.
```

#### Windows

1. В терминале выполнить команду (значение -- путь к рабочей папке, например к папке с репозиторием):

```
setx WORKSDK "D:\Work\aip"
```

2. Перезапустить vscode, чтобы переменная стала видна CMake.

#### Linux

1. Добавить переменную в файл **~/.bashrc**:

```
echo 'export WORKSDK=$HOME/work/aip' >> ~/.bashrc
source ~/.bashrc
```

2. Перезапустить vscode.

### Получение исходников

1. Обновить личный репозиторий (Pull) в Git-клиенте (Git Extensions / Sublime Merge).
2. Убедиться, что в репозитории присутствует папка **BasicUT**.

## Тестовый проект-----------------------------------

Структура проекта:-

```
aip
    BasicUT
        manual
            basic_ut.md
        src
            Converter.h
            Converter.cpp
            main.cpp
        tests
            testMain.cpp
        CMakeLists.txt
    EnvironmentTest
        ...
    CMakeLists.txt
```

### Корневой CMakeLists.txt

Корневой файл объединяет все лабораторные работы в один проект и задает общие настройки (стандарт C++17):

```
cmake_minimum_required(VERSION 3.10)

project(AIP VERSION "1.0")

set(CMAKE_CXX_STANDARD 17)          # Стандарт языка C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Стандарт обязателен

include(CTest) # Подключение модуля тестирования CTest

if(NOT DEFINED ENV{WORKSDK})
    message(FATAL_ERROR "Environment variable WORKSDK is not set.")
endif()

# Подпроекты
add_subdirectory(BasicUT)
add_subdirectory(EnvironmentTest)
```

### Converter.h / Converter.cpp -- тестируемый модуль

Модуль содержит функцию конвертации единиц длины. Именно эта логика является объектом тестирования, поэтому она вынесена в отдельный файл и не зависит от **main()**.

```
#pragma once

namespace NConverter
{

enum class EConversionType
{
    MillimeterToMeter,
    CentimeterToMeter,
    DecimeterToMeter,
    MeterToKilometer,
    UnsupportedConversion = 999
};

double Convert(double in, EConversionType ct);

}
```

```
#include <stdexcept>

#include "Converter.h"

namespace NConverter
{

double Convert(double in, EConversionType ct)
{
    if (in < 0)
        throw std::invalid_argument("Input value must be greater than 0");

    switch (ct)
    {
        case EConversionType::MillimeterToMeter:
            return in / 1000;
        case EConversionType::CentimeterToMeter:
            return in / 100;
        case EConversionType::DecimeterToMeter:
            return in / 10;
        case EConversionType::MeterToKilometer:
            return in / 1000;
        default:
            throw std::invalid_argument("Invalid conversion type");
    }
}
}
```

Контракт функции **Convert**:

- для поддерживаемого типа конвертации возвращает пересчитанное значение;
- для отрицательного входного значения бросает исключение **std::invalid_argument**;
- для неподдерживаемого типа конвертации бросает исключение **std::invalid_argument**.

### main.cpp -- консольное приложение

Приложение принимает входное значение и тип конвертации из аргументов командной строки:

```
BasicUT --in <input> --ct <conversion_type>
```

Типы конвертации: 0 = MillimeterToMeter, 1 = CentimeterToMeter, 2 = DecimeterToMeter, 3 = MeterToKilometer.

### testMain.cpp -- тестовый раннер

Тестовый раннер -- отдельное приложение, которое собирается из тестов и тестируемого модуля (**Converter.cpp**), но без **main.cpp** приложения. Основные элементы:

**CFixture** -- набор тестов. Хранит список тестовых функций, запускает их по очереди и подсчитывает результат.

```
class CFixture
{
public:
    void AddTest(std::function<int()> test); // Регистрация теста
    void Run();                              // Запуск всех тестов и вывод результата
    ...
};
```

**Тестовая функция** -- функция без аргументов, возвращающая **int**. Соглашение: **0** -- тест пройден, любое другое значение -- тест провален. Исключение, вылетевшее из теста, также считается провалом.

```
int TestNegativeInput()
{
    try
    {
        NConverter::Convert(-1.0, NConverter::EConversionType::MillimeterToMeter);
    }
    catch (const std::invalid_argument&)
    {
        return 0; // Ожидаемое исключение -- тест пройден
    }

    return 1; // Исключения не было -- тест провален
}
```

Регистрация и запуск тестов:

```
int main()
{
    CFixture f;

    f.AddTest(TestNegativeInput);
    f.AddTest(TestUnsupportedConversionType);

    f.Run();

    return 0;
}
```

### BasicUT/CMakeLists.txt

Проект описывает две цели сборки: приложение и тестовый раннер. Обратите внимание, что **Converter.cpp** входит в обе цели.

```
project(BasicUT)

# Исходники приложения
set(BasicUT_SRCS
    src/main.cpp
    src/Converter.cpp
    src/Converter.h
)

add_executable(BasicUT
	${BasicUT_SRCS}
)

# Исходники тестового раннера: тесты + тестируемый модуль
set(BasicUT_TESTS_SRCS
    tests/testMain.cpp
    src/Converter.cpp
)

add_executable(BasicUTRunner
    ${BasicUT_TESTS_SRCS}
)
```

## Конфигурация, сборка и запуск

1. Открыть в vscode **корневую** папку репозитория (aip), а не папку BasicUT.
![CMake](../../EnvironmentTest/manual/images/folder.png)
2. Выполнить конфигурацию с помощью CMake.
![CMake](../../EnvironmentTest/manual/images/cmake_configure.png)
Конфигурация должна завершиться строками:

```
[cmake] -- Configuring done
[cmake] -- Generating done
[cmake] -- Build files have been written to: .../aip/build
```

3. Выполнить сборку кнопкой **Build**. Будут собраны все цели, в том числе **BasicUT** и **BasicUTRunner**. Для сборки отдельной цели ее можно выбрать в панели CMake (Build target).
![CMake](../../EnvironmentTest/manual/images/cmake_build.png)

Расположение исполняемых файлов:

| ОС | Приложение | Тестовый раннер |
|---|---|---|
| Windows (Visual Studio) | build\BasicUT\Debug\BasicUT.exe | build\BasicUT\Debug\BasicUTRunner.exe |
| Linux (Makefiles/Ninja) | build/BasicUT/BasicUT | build/BasicUT/BasicUTRunner |

### Запуск приложения

Windows:

```
.\build\BasicUT\Debug\BasicUT.exe --in 1500 --ct 0
```

Linux:

```
./build/BasicUT/BasicUT --in 1500 --ct 0
```

Ожидаемый вывод:

```
1500 MillimeterToMeter equals 1.5
```

Запуск без аргументов выводит подсказку по использованию:

```
Usage: --in <input> --ct <conversion_type>
Conversion types: 0=MillimeterToMeter, 1=CentimeterToMeter, 2=DecimeterToMeter, 3=MeterToKilometer
```

### Запуск тестов

Windows:

```
.\build\BasicUT\Debug\BasicUTRunner.exe
```

Linux:

```
./build/BasicUT/BasicUTRunner
```

Ожидаемый вывод:

```
Running 2 tests...
[PASS] Test 1
[PASS] Test 2
Results: 2 passed, 0 failed
```

Окружение воспроизведено! Можно приступать к выполнению заданий.

## Рекомендации по написанию тестов

1. Один тест проверяет одно поведение. Имя теста отражает, что проверяется: **TestMillimeterToMeter**, **TestNegativeInput**.
2. Тест строится по схеме **Arrange -- Act -- Assert**: подготовить входные данные, вызвать тестируемую функцию, проверить результат.
3. Числа с плавающей точкой нельзя сравнивать через **==**. Используйте сравнение с допуском:

```
#include <cmath>

bool IsEqual(double a, double b, double eps = 1e-9)
{
    return std::fabs(a - b) < eps;
}

int TestMillimeterToMeter()
{
    const double result = NConverter::Convert(1500., NConverter::EConversionType::MillimeterToMeter);

    return IsEqual(result, 1.5) ? 0 : 1;
}
```

4. Проверяйте не только «нормальные» значения, но и граничные: **0**, очень маленькие и очень большие числа, некорректный ввод.
5. Тест для исправленной ошибки пишется **до** исправления: сначала убедитесь, что тест падает, затем исправьте код и убедитесь, что тест проходит.

## Задания

### Общая часть (выполняется во всех вариантах)

1. Воспроизвести окружение по мануалу, собрать и запустить **BasicUT** и **BasicUTRunner**. Приложить к отчету вывод обоих приложений.
2. Покрыть тестами все существующие типы конвертации **Convert** (MillimeterToMeter, CentimeterToMeter, DecimeterToMeter, MeterToKilometer), в том числе для входного значения **0**.
3. Доработать **CFixture** так, чтобы при провале теста выводилось его **имя**, а не только номер (например, `AddTest("TestNegativeInput", TestNegativeInput)`).
4. Изменить **main()** тестового раннера так, чтобы он возвращал **1**, если хотя бы один тест провален, и **0** в противном случае.
5. Зарегистрировать раннер в CTest, добавив в **BasicUT/CMakeLists.txt**:

```
add_test(NAME BasicUTRunner COMMAND BasicUTRunner)
```

   и проверить запуск командой (из папки build):

```
ctest -C Debug --output-on-failure
```

6. Зафиксировать изменения в личном репозитории (commit + push) с осмысленным сообщением коммита.

### Вариант 1. Конвертер массы

1. Создать модуль **MassConverter.h / MassConverter.cpp** в пространстве имен **NMassConverter** с функцией `double Convert(double in, EMassConversionType ct)`.
2. Поддержать типы конвертации: **MilligramToGram**, **GramToKilogram**, **KilogramToTon**, **TonToKilogram**, **PoundToKilogram** (1 lb = 0.45359237 kg).
3. Отрицательное значение и неподдерживаемый тип -- исключение **std::invalid_argument**.
4. Добавить модуль в обе цели сборки (BasicUT и BasicUTRunner) и в консольное приложение (новый аргумент `--mode mass`).
5. Написать не менее **8** тестов: на каждый тип конвертации, на отрицательный ввод, на неподдерживаемый тип, на значение 0.

### Вариант 2. Конвертер температуры

1. Создать модуль **TemperatureConverter.h / TemperatureConverter.cpp** в пространстве имен **NTemperatureConverter**.
2. Поддержать типы конвертации: **CelsiusToFahrenheit**, **FahrenheitToCelsius**, **CelsiusToKelvin**, **KelvinToCelsius**, **FahrenheitToKelvin**.
3. В отличие от исходного конвертера, отрицательные значения температуры допустимы. Исключение **std::out_of_range** бросается, если входное значение ниже абсолютного нуля в исходной шкале (-273.15 °C, -459.67 °F, 0 K).
4. Написать не менее **10** тестов, в том числе:
   - известные точки: 0 °C = 32 °F, 100 °C = 212 °F, -40 °C = -40 °F;
   - значение ровно на абсолютном нуле (исключения нет) и чуть ниже (исключение есть);
   - обратимость: `FahrenheitToCelsius(CelsiusToFahrenheit(x)) == x` с допуском.

### Вариант 3. Конвертер времени и форматирование

1. Создать модуль **TimeConverter.h / TimeConverter.cpp** в пространстве имен **NTimeConverter**.
2. Реализовать функцию `double Convert(double in, ETimeConversionType ct)` с типами: **SecondToMinute**, **MinuteToHour**, **HourToDay**, **DayToWeek**, **SecondToHour**.
3. Реализовать функцию `std::string FormatDuration(long long seconds)`, возвращающую строку вида `"1d 02:03:04"` (дни, часы, минуты, секунды). Если дней 0, префикс `"0d "` не выводится: `"02:03:04"`. Отрицательное значение -- **std::invalid_argument**.
4. Написать не менее **10** тестов, в том числе для **FormatDuration**: 0, 59, 60, 3599, 3600, 86399, 86400, 90061 секунд и отрицательного значения.

### Вариант 4. Обратные преобразования и обработка ошибок

1. Расширить **EConversionType** обратными преобразованиями: **MeterToMillimeter**, **MeterToCentimeter**, **MeterToDecimeter**, **KilometerToMeter**.
2. Добавить в консольное приложение поддержку новых типов (`--ct 4..7`) и обновить текст подсказки.
3. Исправить сообщение исключения для отрицательного ввода: значение **0** допустимо, поэтому текст `"must be greater than 0"` некорректен. Предложить корректный текст и написать тест, проверяющий текст сообщения (`e.what()`).
4. Написать тесты на обратимость для каждой пары преобразований: `Convert(Convert(x, A), B) == x` для значений 0, 1, 0.001, 123456.789.
5. Написать тест, проверяющий, что значение **NaN** (`std::nan("")`) и бесконечность (`std::numeric_limits<double>::infinity()`) обрабатываются предсказуемо. Определить желаемое поведение (исключение или иное), реализовать его в **Convert** и зафиксировать тестом.

### Вариант 5. Улучшение тестового фреймворка

1. Вынести **CFixture** из **testMain.cpp** в отдельные файлы **tests/Fixture.h / tests/Fixture.cpp** и подключить их в цель **BasicUTRunner**.
2. Добавить в фреймворк вспомогательные функции проверки, которые бросают исключение с понятным сообщением при несоответствии:
   - `AssertEqual(double expected, double actual, double eps)`;
   - `AssertTrue(bool condition, const std::string& message)`;
   - шаблонную `AssertThrows<TException>(std::function<void()> f)`.
3. Переписать существующие тесты с использованием новых проверок (тестовая функция теперь может возвращать **void**).
4. Добавить вывод времени выполнения каждого теста и общего времени (использовать `std::chrono`).
5. Добавить фильтр запуска тестов по имени через аргумент командной строки: `BasicUTRunner --filter Negative` запускает только тесты, имя которых содержит подстроку `Negative`.
6. Написать не менее **6** тестов на модуль **Converter** с использованием новых проверок и не менее **3** тестов на сам фреймворк (например, что **AssertThrows** действительно сообщает о провале, если исключения не было).

## Требования к отчету

1. Номер варианта и ссылка на коммит в личном репозитории.
2. Вывод консольного приложения для нескольких входных значений.
3. Вывод тестового раннера и **ctest**, где все тесты проходят.
4. Пример провала теста: внести в код намеренную ошибку, показать вывод раннера, затем вернуть исправление.
5. Ответы на контрольные вопросы.

## Контрольные вопросы

1. Почему тестируемая логика вынесена в **Converter.cpp**, а не находится в **main.cpp**?
2. Почему **Converter.cpp** указан в двух целях сборки? Как можно избежать двойной компиляции (подсказка: `add_library`)?
3. Почему числа с плавающей точкой нельзя сравнивать оператором **==**?
4. Зачем тестовому раннеру возвращать ненулевой код завершения при провале тестов?
5. Чем модульный тест отличается от ручной проверки запуском приложения?
