#!/bin/bash
if tidy -v &> /dev/null; then
  find . -type f -name \*\.html ! -path "./MathJax/*" ! -path "./reveal.js/*" ! -path "./archives/angelscript/docs/*" ! -name "google*.html" -exec tidy -config html-tidy.conf -modify -quiet --tidy-mark false {} \;
else
  echo ${0##*/}: tidy-html5 not found
fi
