#!/bin/sh

# convert unicode to html unbreakable entities
for CATALOG in *.po ; do {
  sed -i".sav" "s/ /\&nbsp;/g" $CATALOG
} ; done

