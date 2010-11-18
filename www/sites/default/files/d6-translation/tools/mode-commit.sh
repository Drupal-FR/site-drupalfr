#!/bin/bash

# convert unbreakable entities to unicode and normalize
for CATALOG in *.po ; do {
  sed -i".sav" "s/&nbsp;/ /g" $CATALOG 
  ( msgcat "$CATALOG" > $CATALOG.tmp ) && (mv "$CATALOG.tmp" "$CATALOG" ; echo -n "+$CATALOG ") || (echo -n "!!!$CATALOG!!! ")
} ; done
echo
