"""This module outputs strings missing from other .ts files"""
EN_FILE = open("en.ts", "r")
FR_FILE = open("fr.ts", "r")
EN_STRINGS, FR_STRINGS = [], []

for en_string in EN_FILE:
    en_string = en_string.lstrip()
    if en_string.startswith("<source>"):
        EN_STRINGS.append(en_string[8:-10])

for fr_string in FR_FILE:
    fr_string = fr_string.lstrip()

    if fr_string.startswith("<source>"):
        fr_string = fr_string[8:-10]
        FR_STRINGS.append(fr_string)

for string in EN_STRINGS:
    if string not in FR_STRINGS:
        print string
