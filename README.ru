translate - перевод текста в консоли используя словари stardict
Файл настроек должен лежать в $HOME/.config/translate/translate.conf
В этом файле можно указать всего две опции.

jobdir = в эту опцию можно записать папки где будет словари, можно записать через запятую несколько путей папок, тогда поиск будет произведён в перечисленных папках. Поиск словарей происходит рекурсивным способом, тогда можно в папке хранить несколько папок, не сваливая все словари в одну папку.
dicts = если эта опиция закоментирована, значит будут проверятся все словари из опции jobdir. Если опция dicts включена, то будут выбираться только перечисленные в этой опции словари, можно через запятую несколько указать. Вот пример.

jobdir=/home/user/dicts
dicts=stardict.dict

Когда настройки завершены, можно использовать translate.
translate слово
