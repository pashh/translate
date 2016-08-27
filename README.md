translate - the translation of the text in the console using dictionaries of stardict

In a house directory there shall be a file with the .translate.conf settings
In this file it is possible to specify only two options.

jobdir = in this option can write folders where there will be dictionaries, it is possible to write several ways of folders through a comma, then search will be run in the listed folders. Search of dictionaries happens the recursive method, then it is possible to store several folders in the folder, without dumping all dictionaries in one folder.

dicts = if this opition is commented out, means will be all dictionaries from the option jobdir will be checked. If the option dicts is included, then only the dictionaries listed in this option will be selected, it is possible to specify a little through a comma. There is an example.

jobdir=/home/user/dicts
dicts=stardict.dictch

When settings are complete, it is possible to use translate.
translate word
