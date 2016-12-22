#!/usr/bin/env python3

import os


def main():
    with open("sitemap.xml", "w") as file:
        file.write(
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<urlset xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\"\n"
            "    xmlns:image=\"http://www.google.com/schemas/sitemap-image/1.1\"\n"
            "    xmlns:video=\"http://www.google.com/schemas/sitemap-video/1.1\">\n"
        )
        files = [
            os.path.join(dp, f)
            for dp, dn, fn in os.walk(os.path.expanduser(".")) for f in fn
            if f.endswith(".html") and "MathJax" not in f
        ]
        for name in [
                os.path.join(dp, f)
                for dp, dn, fn in os.walk(os.path.expanduser(".")) for f in fn
        ]:
            if name.endswith(".html") and "MathJax" not in name:
                file.write("    <url>\n"
                           "        <loc>https://csweb.frc3512.com/" + name[2:]
                           + "</loc>\n"
                           "    </url>\n")
        file.write("</urlset>\n")


if __name__ == "__main__":
    main()
