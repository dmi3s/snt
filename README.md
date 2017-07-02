# snt
## Тестовое задание от Segnetics.

* Проект собирается cmake-ом версии 3.8+.
* Требуется boost 1.64.0+. Из библиотек используется только filesystem (уже есть в 17м стандарте, но я старался писать под 14й). 
Из headers-only библиотек используются signals2, process, range.
* Проверял под vs2017/win64.
* процесс сборки:
```
mkdir mbuild && cd mbuild
cmake -G "Visual Studio 15 2017 Win64" ..
cmake --build .
```
* Два испольняемых файла будут лежать в соответствующем конфигурации каталоге (mbuild/Debug, bmuild/Release, etc). 
* producer можно запускать без consumer-а. он считывает со стандратного ввода микросекунды, и выдает на стандартный вывод последние семплы за заданное время. 0 для выхода.
* consumer запускает producer в фоне и вычитывает ответ через pipe, конструируя vector<sample>.
* producer не стирает созданные файлы chunk-ов. Каталог запуска заполнится каталогами вида "store [50] - 1579XXXXXXXXXXXXX". Не стал удалять в коде для наглядности происходящего 
и разборов полетов.
