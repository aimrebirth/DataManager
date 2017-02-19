#!/usr/bin/python
# -*- coding: utf-8 -*-

import argparse
import re
import sqlite3

regexp_templates = [
    ['\n+', '\n'],
    ['  +', ' '],
    ['\t+', ' '],

    ['й', 'й'], # double-symboled russian 'й'

    ['B_L1_TUNNEL', 'BLD_TUNNEL'],
]

replacements = []
n_replacements_ref = 0
n_replacements_link = 0

def main():
    parser = argparse.ArgumentParser(description='generate test')
    parser.add_argument('database', help='database file to load')
    parser.add_argument('-q', '--quiet', action='store_true', default=False, help='quiet mode')
    pargs = parser.parse_args()

    conn = sqlite3.connect(pargs.database)
    c = conn.cursor()

    conn.create_function("format", 1, format)
    conn.create_function("replace_references", 2, replace_references)
    conn.create_function("replace_links", 2, replace_links)

    c.execute("update strings set ru = format(ru)")
    c.execute("update strings set ru = replace_references(ru, id)")
    c.execute("update strings set ru = replace_links(ru, id)")

    if len(replacements) > 0 and pargs.quiet == False:
        f = open('replacements.txt', 'w', encoding = 'utf-8')
        for s in replacements:
            f.write(s[0])
            f.write('\n')
            f.write(s[1])
            f.write('\n\n')
    print('replacements done:', len(replacements))
    print('replacements ref done:', str(n_replacements_ref))
    print('replacements links done:', str(n_replacements_link))

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

def replace_references(string, id):
    global n_replacements_ref

    r = '<reference:([^=]+)=([^>]+)>'

    while True:
        m = re.search(r, string)
        if not m:
            break

        text = m.group(1).strip()
        ref = m.group(2).strip()

        # move punctuation out of link
        mp = re.search('([!"\\#%&\'()*+,-./:;<=>?@\[\]^_`{|}~]+)$', text)
        p = ''
        if mp:
            p = mp.group(1)
            text = replace_last(text, p, '')

        # do a replacement
        string = string.replace(m.group(0), '<a id="object" textstyle="p4blue" name="' + ref + '">' + text + '</>' + p)

        n_replacements_ref = n_replacements_ref + 1

    return string

def replace_links(string, id):
    global n_replacements_link

    r = '<link:([^=]+)=([^>]+)>'

    while True:
        m = re.search(r, string)
        if not m:
            break

        print(str(id))
        print(m)

        text = m.group(1).strip()
        ref = m.group(2).strip()

        # do a replacement
        string = string.replace(m.group(0), '<a id="script" textstyle="p4red" name="' + ref + '">' + text + '</>')

        n_replacements_link = n_replacements_link + 1

    return string

def replace_last(source_string, replace_what, replace_with):
    head, sep, tail = source_string.rpartition(replace_what)
    return head + replace_with + tail

if __name__ == '__main__':
    main()
