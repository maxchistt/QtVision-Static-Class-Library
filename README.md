# QtVision

C3D QtVision static Qt class library for usage C3D Vision in Qt projects without CMake

To build the project, first place the directories of the C3D Toolkit distribution "Kernel" and "Vision" in the "C3D" folder.
To export library into .zip file run QtVision.Export.ps1 by right click -> Run with PowerShell

## Инструкция по использованию

Это проект статитеской библиотеки классов Qt "QtVision" для использования C3D в проектах на C++ и Qt без CMake

### Сборка проекта

1. Если VS не настроена для работы с Qt, смотрим материал по [ссылке](http://wiki.mpu-cloud.ru/c3d-vision-getting-started)
1. Копируем папки "Kernel" и "Vision" из дистрибутива ядра в папку "C3D" этого проекта
1. Открываем `QtVision.sln`, назначаем Example в качестве запускаемого проекта по умолчанию. Теперь мы можем попробовать запустить тестовое приложение
1. Чтобы собрать библиотеку в архив для подключения в других проектах, отрываем папку решения и запускаем `QtVision.Export.ps1` с помошью `ПКМ -> Выполнить с помощью PowerShell`. Если требуется установить пакет при выполнении, вводим "Y". Если скрипт завис на "Build Debug", перезапускаем его. По итогу работы скрипт открывает архив с построенной библиотекой и ядром
1. Если скрипт не работает, собираем вручную: билдим версии Debug и Release, создаем внешнюю папку, копируем `QtVision.lib` из папок "x64/Debug" и "x64/Release" в во внешнюю папку в подпапки "Debug" и "Release" соответственно, копируем содержимое "QtVision/Include", то есть все заголовочные файлы библиотеки, туда же, в подпапку "Include"

### Подключение и использование

Можно использовать собранную библиотеку (можно собрать самому или скачать по [ссылке](https://clck.ru/YsCKE) из документа нашей команды). Это медод для Visual Studio, в котором мы работаем. При желании похожим образом можно подключить ее в проекте в QtCreator (Гугли как подключать библиотеки в QtCreator)

1. Создаем проект Qt Widgets в Visual Studio
1. Копируем в папку решения папки библиотек "Kernel", "Vision", "QtVision"
1. В свойствах "Qt Project Settings -> Qt modules" задаем "core;opengl;gui;widgets" (Добавляем opengl)
1. В свойствах "C/C++ -> Общие -> Дополнительные каталоги включаемых файлов" добавляем пути к заголовочным файлам библиотек "`$(SolutionDir)Vision\Include;$(SolutionDir)Kernel\Include;$(SolutionDir)QtVision\Include;`". Делаем это построчно, через "Изменить", не удаляя то что уже есть
1. В свойствах "Компоновщик -> Общие -> Дополнительные каталоги библиотек" добавляем пути к заголовочным файлам библиотек "`$(SolutionDir)Vision\$(Configuration);$(SolutionDir)Kernel\$(Configuration);$(SolutionDir)QtVision\$(Configuration);`".
1. В свойствах "Компоновщик -> Ввод -> Дополнительные зависимости" добавляем имена библиотек импорта для динамических библиотек ядра "`c3d.lib;c3dvision.lib;`"
1. Копируем Dll-ки ядра в выходной каталог, или в "события сборки -> событие перед соркой" добавляем

```cmd
xcopy /y "$(SolutionDir)Vision\$(Configuration)\c3dvision.dll" "$(SolutionDir)$(Platform)\$(Configuration)"
xcopy /y "$(SolutionDir)Kernel\$(Configuration)\c3d.dll" "$(SolutionDir)$(Platform)\$(Configuration)"
```

Можно использовать этот проект

1. Создаем проект Qt Widgets в Visual Studio
1. Копируем в папку решения папки библиотек ядра "Kernel" и "Vision" в подпапку "C3D"
1. Копируем в папку проекта "QtVision" из папки этого решения в папку решения созданного приложения
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

### Лицензия

При работе приложений с ядром необходимо активировать лицензию. Для этого в приложениях надо вызывать метод QtVision::ActivateLicense. Перед этим надо задать ключ и сигнатуру. Есть три способа это сделать:

1. В проекте QtVison (или в папке Include собранной библиотеки) в файле "lisencekey.h" задаем правильные ключ и сигнатуру, в проекте приложения вызываем метод `QtVision::ActivateLicense`
1. В проекте приложения вызываем метод `QtVision::checkLicenseWithInput` (дополнительный метод, содержит вызов QtVision::ActivateLicense. позволяет ввести ключ и сигнатуру)
1. В проекте приложения вызываем метод `QtVision::setLicenseKeyValues` (дополнительный метод, содержит вызов QtVision::ActivateLicense. позволяет передать ключ и сигнатуру параметрами)
