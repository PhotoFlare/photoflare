"""`
This module outputs strings missing from other .ts files
:param [lang]
example run: python3 diffing_tool.py de
python3 diffing_tool.py de nl fr -> produces de.txt, nl.txt, fr.txt
"""
import sys
EN_FILE = open("en.ts", "r")
EN_STRINGS = []

for en_string in EN_FILE:
    en_string = en_string.lstrip().capitalize()
    if en_string.startswith("<source>"):
        EN_STRINGS.append(en_string[8:-10])

for lang_short in sys.argv[1:]:
    LANG_FILE = open(lang_short + ".ts", "r")
    DIFF_FILE_OUTPUT = open(lang_short + ".txt", "w")
    LANG_STRING_LIST, DIFF_LIST = [], []

    for lang_string in LANG_FILE:
        lang_string = lang_string.lstrip().capitalize()
        if lang_string.startswith("<source>"):
            LANG_STRING_LIST.append(lang_string[8:-10])

    for string in EN_STRINGS:
        if string not in LANG_STRING_LIST:
            DIFF_LIST.append(string)

    DIFF_LIST = [DIFF_FILE_OUTPUT.write(line + "\n") for line in DIFF_LIST]
