#!/usr/bin/env python3

import os
import re
import subprocess


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

    # Format Python
    files = [
        os.path.join(dp, f)
        for dp, dn, fn in os.walk(".")
        for f in fn
        if f.endswith(".py")
    ]
    for f in files:
        subprocess.check_output(["python3", "-m", "black", "-q", f])


if __name__ == "__main__":
    main()
