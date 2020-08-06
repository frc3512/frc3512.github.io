#!/usr/bin/env python3

import os
import re


def main():
    files = [
        os.path.join(dp, f)
        for dp, dn, fn in os.walk(os.path.expanduser("."))
        if "MathJax" not in dp and "reveal.js" not in dp
        for f in fn
        if f.endswith(".html")
    ]

    # Do not process certain problematic files
    skip_strs = ["archives/angelscript/docs"]
    for skip_str in skip_strs:
        files = [f for f in files if skip_str not in f]

    line_regex = re.compile(
        "(?P<open>\s*<h[2-6])\s+"
        '(?P<id>id="[A-Za-z0-9\-._]*")?'
        "(?P<close>>)"
        "(?P<content>.*?)"
        "(?P<end></h[2-6]>.*)"
    )
    char_regex = re.compile("[^A-Za-z0-9\-._]")
    for name in files:
        lines = ""
        modified = False
        in_tag = False
        with open(name, "r") as input:
            for line in input:
                match = line_regex.search(line)
                if match:
                    edit = match["open"] + ' id="'
                    for char in match["content"]:
                        if char == "<":
                            in_tag = True
                            continue
                        elif char == ">":
                            in_tag = False
                            continue

                        if not in_tag:
                            if char == " ":
                                edit += "_"
                            elif char_regex.match(char) and char != "\n":
                                edit += "." + format(ord(char), "X")
                            else:
                                edit += char
                    edit += (
                        '"' + match["close"] + match["content"] + match["end"] + "\n"
                    )

                    if line != edit:
                        modified = True
                    lines += edit
                else:
                    lines += line
        if modified:
            with open(name, "w") as output:
                output.write(lines)


if __name__ == "__main__":
    main()
