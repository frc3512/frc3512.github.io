#!/bin/bash
latexmk -pdf state-diagram
pdftoppm -png -r 300 state-diagram.pdf > state-diagram.png
convert -transparent white state-diagram.png state-diagram.png
