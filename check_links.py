#!/usr/bin/env python3

"""Ensure URLs in every .html file are reachable"""

import multiprocessing as mp
import os
import re
import requests
import sys


def lint_links(link):
    """Runs the URL in the regex Match object through the link linter.

    Keyword arguments:
    link -- a tuple containing the filename, line number of URL, and URL
    """
    filename, line, url = link
    return verify_url(filename, line, url)


def verify_url(filename, line_number, url):
    """Verifies URL is reachable and returns 200 status code.

    Keyword arguments:
    filename -- name of file containing URL
    line_number -- line number of URL
    url -- the URL to verify

    Returns:
    True if verification succeeded or False otherwise
    """
    if url.startswith("http://") or url.startswith("https://"):
        # Ignore .ms links since they only return success on Windows
        m = re.search(r"^https?://([^/]+)/.*?$", url)
        if m.group(1).endswith(".ms"):
            return True

        try:
            r = requests.head(url, headers={"User-Agent": "Python Requests"})
            if r.status_code != 200:
                print(f"[{filename}:{line_number}]\n    {url}\n    {r.status_code}")
                return False
        except requests.ConnectionError as ex:
            print(f"[{filename}:{line_number}]\n    {url}\n    {str(ex)}")
            return False
    else:
        if url.find("#") != -1:
            # Strip trailing page ID
            relative_dest = url[: url.find("#")]
            url_id = url[url.find("#") + 1 :]
        else:
            relative_dest = url
            url_id = ""

        # If URL is empty, it refers to the current file
        if relative_dest == "":
            relative_dest = os.path.basename(filename)

        absolute_dest = os.path.join(os.path.dirname(filename), relative_dest)

        if not os.path.exists(absolute_dest):
            print(
                f"[{filename}:{line_number}]\n    file://{relative_dest}\n    No such file or directory."
            )
            return False

        # If filename wasn't provided in link, assume index.html
        if os.path.isdir(absolute_dest):
            absolute_dest = os.path.join(absolute_dest, "index.html")

        # If destination file exists and the URL has an ID appended, verify
        # the ID exists in the destination file.
        if len(url_id) > 0:
            id_rgx = re.compile(r'\s+ id= \s* "(?P<id>[^"]+)"', re.X)

            with open(absolute_dest) as file:
                content = file.read()
            for match in id_rgx.finditer(content):
                if match.group("id") == url_id:
                    return True

            print(
                f"[{filename}:{line_number}]\n    file://{absolute_dest}\n    tag '{url_id}' does not exist in {absolute_dest}"
            )
            return False
    return True


files = [
    os.path.join(dp, f)[2:]
    for dp, dn, fn in os.walk(".")
    for f in fn
    if f.endswith(".html") and "reveal.js" not in dp
]

url_rgx = re.compile(r'a \s+ href= \s* "(?P<url>[^"]+)"', re.X)

# link tuples contain:
#   filename -- filename
#   contents -- file contents
#   match -- regex Match object
links = []
for filename in files:
    # Get file contents
    with open(filename, "r") as f:
        contents = f.read()

    for match in list(url_rgx.finditer(contents)):
        # Get line regex match was on
        linecount = 1
        for i in range(match.start()):
            if contents[i] == os.linesep:
                linecount += 1

        links.append((filename, linecount, match.group("url")))

with mp.Pool(mp.cpu_count()) as pool:
    results = pool.map(lint_links, links)

if all(results):
    sys.exit(0)
else:
    sys.exit(1)
