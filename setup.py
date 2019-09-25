#!/usr/bin/env python

from distutils.core import setup, Extension

import os

srcs = ['_yuv.c', '_yuv2rgb.c', '_py.c']

ext = Extension('PyYuv._yuv', 
            srcs)

setup(name='PyYuv',
      version='0.0',
      description='PyYuv extension',
      author='Tee Jung',
      author_email='naey05@gmail.com',
      url='https://github.com/Tee0125/pyyuv',
      ext_modules=[ext],
      packages=['PyYuv'],
     )
