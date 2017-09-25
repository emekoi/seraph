#/usr/bin/bash

python gl3w_gen.py --ext --root src
echo
python build.py $@