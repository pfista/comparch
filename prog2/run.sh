#!/bin/sh
#for i in yb/*.yb ; do ./a.out $i; done > output
for i in yb/*.yb ; do a.out $i; done > output
#for i in `\ls yb`; do ./a.out yb/$i >> output; done
