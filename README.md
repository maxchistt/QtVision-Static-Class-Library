# QtVision

C3D QtVision static Qt class library for usage C3D Vision in Qt projects without CMake

To build the project, first place the directories of the C3D Toolkit distribution "Kernel" and "Vision" in the "C3D" folder.
To export library into .zip file run QtVision.Export.ps1 by right click -> Run with PowerShell

## Инструкция по использованию

Это проект статитеской библиотеки классов Qt "QtVision", адаптирующий одноименный проект из дистрибутива C3D Toolkit для использования C3D Vision в проектах на C++ и Qt без CMake. Добавлена возможность собрать также и Dll.

### Сборка проекта

#### Cтатическая библиотека

1. Если VS не настроена для работы с Qt, смотрим материал по [ссылке](http://wiki.mpu-cloud.ru/c3d-vision-getting-started)
1. Копируем папки "Kernel" и "Vision" из дистрибутива ядра в папку "C3D" этого проекта
1. Открываем `QtVision.sln`, назначаем Example в качестве запускаемого проекта по умолчанию. Теперь мы можем попробовать запустить тестовое приложение
1. Чтобы собрать библиотеку в архив для подключения в других проектах, отрываем папку решения и запускаем `QtVision.Export.ps1` с помошью `ПКМ -> Выполнить с помощью PowerShell`. Если требуется установить пакет при выполнении, вводим "Y". Если скрипт завис на "Build Debug", перезапускаем его. По итогу работы скрипт открывает архив (..\QtVision.zip) с построенной библиотекой и ядром
1. Если скрипт не работает, собираем вручную: билдим версии Debug и Release, создаем внешнюю папку, копируем `QtVision.lib` из папок "x64/Debug" и "x64/Release" в во внешнюю папку в подпапки "Debug" и "Release" соответственно, копируем содержимое "QtVision/Include", то есть все заголовочные файлы библиотеки, туда же, в подпапку "Include"

#### Динамическая библиотека

1. Настраиваем VS для работы с Qt, ссылка на инструкцию в предидущем разделе
1. Копируем папки "Kernel" и "Vision" из дистрибутива ядра в папку "C3D" этого проекта
1. Открываем каталог решения как проект CMake, есть два способа это сделать:
   - 1. Открываем через СMake Gui, запуская программу 'СMake (cmake-gui)'
     1. Указываем папку проекта, в качестве папки сборки указываем тот же путь, добавляя "/Build"
     1. Жмем Configure 2 раза, Generate, Open
   - 1. Открываем через Visual Studio, кликнув по папке решения `ПКМ -> Открыть с помощью Visual Studio`
     1. Жмем `Переключиться между решениями и доступными представлениями -> Просмотр целевых объектов CMake`
1. Переключаемся на конфигурацию Debug и собираем целевой обьект "QtVision" (`ПКМ -> Сборка`)
1. Переключаемся на конфигурацию Release и собираем целевой обьект "QtVision"
1. Оставаясь в конфигурации Release, собираем целевой обьект "ExportLib". Теперь собранная библиотека в подпапке решения "ExportLibDll"

### Подключение и использование

#### Cтатическая библиотека

##### Можно использовать собранную библиотеку

Можно собрать самому или скачать по [ссылке](https://clck.ru/YsCKE) из документа нашей команды. Это инструкция для Visual Studio, в котором мы работаем. При желании похожим образом можно подключить ее в проекте в QtCreator (Гугли как подключать библиотеки в QtCreator)

1. Создаем проект Qt Widgets в Visual Studio
1. Копируем в папку решения папки библиотек ядра "Kernel" и "Vision"
1. Копируем в папку решения папку стаической библиотеки классов Qt "QtVision" из собранного или скачанного архива
1. В свойствах "Qt Project Settings -> Qt modules" задаем "core;opengl;gui;widgets" (Добавляем opengl)
1. В свойствах "C/C++ -> Общие -> Дополнительные каталоги включаемых файлов" добавляем пути к заголовочным файлам библиотек "`$(SolutionDir)Vision\Include;$(SolutionDir)Kernel\Include;$(SolutionDir)QtVision\Include;`". Делаем это построчно, через "Изменить", не удаляя то что уже есть
1. В свойствах "Компоновщик -> Общие -> Дополнительные каталоги библиотек" добавляем пути к заголовочным файлам библиотек "`$(SolutionDir)Vision\$(Configuration);$(SolutionDir)Kernel\$(Configuration);$(SolutionDir)QtVision\$(Configuration);`".
1. В свойствах "Компоновщик -> Ввод -> Дополнительные зависимости" добавляем имена библиотек импорта для динамических библиотек ядра и имя нашей статической библиотеки классов"`c3d.lib;c3dvision.lib;qtvision.lib;`"
1. Копируем Dll-ки ядра в выходной каталог, или в "события сборки -> событие перед соркой" добавляем

```cmd
xcopy /y "$(SolutionDir)Vision\$(Configuration)\c3dvision.dll" "$(SolutionDir)$(Platform)\$(Configuration)"
xcopy /y "$(SolutionDir)Kernel\$(Configuration)\c3d.dll" "$(SolutionDir)$(Platform)\$(Configuration)"
```

##### Можно использовать этот проект

1. Создаем проект Qt Widgets в Visual Studio
1. Копируем в папку решения папки библиотек ядра "Kernel" и "Vision" в подпапку "C3D"
1. Копируем папку с проектом "QtVision" из папки этого решения в папку решения созданного приложения
1. В контекстном меню решения выбираем "Добавить -> Существующий проект" и выбираем "QtVision.vsxproj"
1. В проекте приложения добавляем ссылку на проект "QtVsion", жмем "Ссылки -> Добавить ссылку -> Проект -> QtVsion"
1. В свойствах проекта приложения "Qt Project Settings -> Qt modules" задаем "core;opengl;gui;widgets" (Добавляем opengl)
1. В свойствах проекта приложения "C/C++ -> Общие -> Дополнительные каталоги включаемых файлов" добавляем пути к заголовочным файлам библиотек "`$(SolutionDir)C3D\Vision\Include;$(SolutionDir)C3D\Kernel\Include;$(SolutionDir)QtVision\Include;`". Делаем это построчно, через "Изменить", не удаляя то что уже есть
1. В свойствах проекта приложения "Компоновщик -> Общие -> Дополнительные каталоги библиотек" добавляем пути к заголовочным файлам библиотек ядра "`$(SolutionDir)C3D\Vision\$(Configuration);$(SolutionDir)C3D\Kernel\$(Configuration);`"
1. В свойствах проекта приложения "Компоновщик -> Ввод -> Дополнительные зависимости" добавляем имена библиотек импорта для динамических библиотек ядра "`c3d.lib;c3dvision.lib;`"
1. Копируем Dll-ки ядра в выходной каталог, или в "события сборки -> событие перед соркой" добавляем

```cmd
xcopy /y "$(SolutionDir)C3D\Vision\$(Configuration)\c3dvision.dll" "$(SolutionDir)$(Platform)\$(Configuration)"
xcopy /y "$(SolutionDir)C3D\Kernel\$(Configuration)\c3d.dll" "$(SolutionDir)$(Platform)\$(Configuration)"
```

#### Динамическая библиотека

1. Создаем проект Qt Widgets в Visual Studio
1. Копируем в папку решения папки библиотек ядра "Kernel" и "Vision"
1. Копируем в папку решения папку "QtVision" из собранной папки "ExportLibDll"
1. В свойствах "Qt Project Settings -> Qt modules" задаем "core;opengl;gui;widgets" (Добавляем opengl)
1. В свойствах "C/C++ -> Общие -> Дополнительные каталоги включаемых файлов" добавляем пути к заголовочным файлам библиотек "`$(SolutionDir)Vision\Include;$(SolutionDir)Kernel\Include;$(SolutionDir)QtVision\Include;`". Делаем это построчно, через "Изменить", не удаляя то что уже есть
1. В свойствах "Компоновщик -> Общие -> Дополнительные каталоги библиотек" добавляем пути к заголовочным файлам библиотек "`$(SolutionDir)Vision\$(Configuration);$(SolutionDir)Kernel\$(Configuration);$(SolutionDir)QtVision\$(Configuration);`".
1. В свойствах "Компоновщик -> Ввод -> Дополнительные зависимости" добавляем имена библиотек импорта для динамических библиотек ядра и имя нашей статической библиотеки классов"`c3d.lib;c3dvision.lib;qtvision.lib;`"
1. Копируем Dll-ки ядра и QtVision в выходной каталог, или в "события сборки -> событие перед соркой" добавляем

```cmd
xcopy /y "$(SolutionDir)Vision\$(Configuration)\c3dvision.dll" "$(SolutionDir)$(Platform)\$(Configuration)"
xcopy /y "$(SolutionDir)Kernel\$(Configuration)\c3d.dll" "$(SolutionDir)$(Platform)\$(Configuration)"
xcopy /y "$(SolutionDir)QtVision\$(Configuration)\QtVision.dll" "$(SolutionDir)$(Platform)\$(Configuration)"
```

### Лицензия

При работе приложений с ядром необходимо активировать лицензию. Для этого в приложениях надо вызывать метод `QtVision::ActivateLicense`. При активации потребуется ввести ключ и сигнатуру, активация может быть сохранена.
Также, если вы не собиратесь добавлять этот проект (не собранную библиотеку) в решение с открытым исходным кодом, можно захардкодить лицензию, чтобы пользователям готового приложения не потребовалось вводить ее - заменить в файле `qt_licensewnd.cpp` следующие директивы:

```cpp
#define C3DKEY  "<C3DLicenseKey>"
#define C3DSIGN "<C3DSignature>"
```
