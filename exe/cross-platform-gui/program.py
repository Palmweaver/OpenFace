#!/usr/bin/env python3

import sys
import random

random.seed()

sys.stderr.writelines(["Hello: {0}".format(random.randint(1, 20))])
