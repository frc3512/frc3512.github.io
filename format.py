#!/usr/bin/env python3

import os
import re
import subprocess


def get_linesep(lines):
    """Returns string containing autodetected line separator for file.

    Keyword arguments:
    lines -- file contents string
    """
    # Find potential line separator
    pos = lines.find("\n")

    # If a newline character was found and the character preceding it is a
    # carriage return, assume CRLF line endings. LF line endings are assumed
    # for empty files.
    if pos > 0 and lines[pos - 1] == "\r":
        return "\r\n"
    else:
        return "\n"


def strip_trailing_whitespace(filename):
    """Removes trailing whitespace from the file.

    Keyword arguments:
    filename -- name of file to strip
    """
    with open(filename) as input:
        lines = input.read()
    linesep = get_linesep(lines)

    output = ""
    for line in lines.splitlines():
        line = line[: len(line)].rstrip()
        output += line + linesep

    if lines != output:
        with open(filename, "wb") as file:
            file.write(output.encode())


def main():
    # Format HTML
    files = [
        os.path.join(dp, f)
        for dp, dn, fn in os.walk(".")
        for f in fn
        if f.endswith(".html")
    ]
    files = [
        f
        for f in files
        if not f.startswith("./MathJax/")
        and not f.startswith("./reveal.js/")
        and not f.startswith("./archives/angelscript/docs/")
        and not re.search(r"google.*?\.html$", f)
    ]
    for f in files:
        subprocess.check_output(
            [
                "tidy",
                "-config",
                "html-tidy.conf",
                "-modify",
                "-quiet",
                "--tidy-mark",
                "false",
                f,
            ]
        )
        strip_trailing_whitespace(f)

    # Format Python
    files = [
        os.path.join(dp, f)
        for dp, dn, fn in os.walk(".")
        for f in fn
        if f.endswith(".py")
    ]
    for f in files:
        subprocess.check_output(["python3", "-m", "black", "-q", f])
        strip_trailing_whitespace(f)


if __name__ == "__main__":
    main()
