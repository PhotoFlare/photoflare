"""This module outputs strings missing from other .ts files"""
import sys

EN_FILE = open("en.ts", "r")
FR_FILE = open("fr.ts", "r")
EN_STRINGS, LANG_STRINGS = [], []

for en_string in EN_FILE:
    en_string = en_string.lstrip()
    if en_string.startswith("<source>"):
        EN_STRINGS.append(en_string[8:-10])

for arg in sys.argv[1:]:
    LANG_FILE = arg + ".ts"
    LANG_STRINGS = []

    for lang_string in LANG_FILE:
        lang_string = lang_string.lstrip()

        if lang_string.startswith("<source>"):
            lang_string = lang_string[8:-10]
            LANG_STRINGS.append(lang_string)

        for string in EN_STRINGS:
            if string not in LANG_STRINGS:
                print (string)
