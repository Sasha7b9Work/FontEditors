# PhuntFontEditor

Инструкция по сборке.

Предустановленное программное обеспечеие.
1. Python 3 - https://www.python.org/downloads/
2. CMake - https://cmake.org/download/
3. Visual Studio 2019 - https://visualstudio.microsoft.com/ru/downloads/

Создайте следующую структуру каталогов:

Editor -+- PhuntFontEditor
        +- ThirdParty
        
Склонируйте проекты с github.com:
PhuntFontEditor - https://github.com/Sasha7b9/PhuntFontEditor
ThirdParty - https://github.com/Sasha7b9/ThirdParty

Сборка сторонних библиотек.
1. Перейдите в каталог Editor/ThirdParty/scripts и запустите файл promt.bat
2. Выполните команду "install_python_libs.bat". Необходимые библиотеки python установлены
3. Со страницы https://www.wxwidgets.org/downloads/ скачайте файл Windows ZIP и распакуйте его в созданный каталог Editor/ThirdParty/wxWidgets
3. Выполните команду "install_libs.bat". Библиотеки, необходимые для сборки редактора, собраны.


Сборка редактора.
1. Перейдите в каталог Editor/PhuntFontEditor/sources/scripts и запустите файл promt.bat
2. Выполните команду "make_build.bat"
3. Выполните команду "run.bat". Это запустит редактор.

Для редактирования исходного кода выполните "edit.bat"
