#!/usr/bin/env sh

pandoc --bibliography ./bibliography.bib -s docs.md -o docs.pdf -V lang=ro
