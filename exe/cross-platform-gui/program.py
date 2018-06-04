#!/usr/bin/env python3

import sys
import random

random.seed()

data_input = sys.stdin.readline()

print("Python got" + data_input)

sys.stderr.writelines(["Hello: {0}".format(random.randint(1, 20))])
