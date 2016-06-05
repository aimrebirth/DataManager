#!/usr/bin/python
# -*- coding: utf-8 -*-

import argparse
import re
import sqlite3

regexp_templates = [
    ['\n+', '\n'],
    ['  +', ' '],
    ['\t+', ' '],

    ['й', 'й'] # double-symboled russian 'й'
]

replacements = []

def main():
    parser = argparse.ArgumentParser(description='generate test')
    parser.add_argument('database', help='database file to load')
    parser.add_argument('-q', '--quiet', action='store_true', default=False, help='quiet mode')
    pargs = parser.parse_args()

    conn = sqlite3.connect(pargs.database)
    c = conn.cursor()

    conn.create_function("format", 1, format)

    c.execute("update strings set ru = format(ru)")

    if len(replacements) > 0 and pargs.quiet == False:
        f = open('replacements.txt', 'w', encoding = 'utf-8')
        for s in replacements:
            f.write(s[0])
            f.write('\n')
            f.write(s[1])
            f.write('\n\n')
    print('replacements done:', len(replacements))

    conn.commit()
    conn.close()

def format(string):
    old = string
    while 1:
        s = string
        for tpl in regexp_templates:
            string = re.sub(tpl[0], tpl[1], string)
        if s == string:
            break
    if old != string:
        arr = []
        arr.append(old)
        arr.append(string)
        replacements.append(arr)
    return string

if __name__ == '__main__':
    main()
