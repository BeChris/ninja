#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
from setuptools import setup, Extension

common_sources=[
'src/build.cc',
'src/build_log.cc',
'src/clean.cc',
'src/clparser.cc',
'src/debug_flags.cc',
'src/depfile_parser.cc',
'src/deps_log.cc',
'src/disk_interface.cc',
'src/edit_distance.cc',
'src/eval_env.cc',
'src/graph.cc',
'src/graphviz.cc',
'src/lexer.cc',
'src/line_printer.cc',
'src/manifest_parser.cc',
'src/metrics.cc',
'src/ninja.cc',
'src/ninjacore.cc',
'src/state.cc',
'src/string_piece_util.cc',
'src/util.cc',
'src/version.cc',
]

common_test_sources=[
'src/build_log_test.cc',
'src/build_test.cc',
'src/clean_test.cc',
'src/clparser_test.cc',
'src/depfile_parser_test.cc',
'src/deps_log_test.cc',
'src/disk_interface_test.cc',
'src/edit_distance_test.cc',
'src/graph_test.cc',
'src/lexer_test.cc',
'src/manifest_parser_test.cc',
'src/ninja_test.cc',
'src/state_test.cc',
'src/string_piece_util_test.cc',
'src/subprocess_test.cc',
'src/test.cc',
'src/util_test.cc',
]

unix_sources=[
'src/subprocess-posix.cc',
]

windows_sources=[
'src/subprocess-win32.cc',
'src/includes_normalize-win32.cc',
'src/msvc_helper-win32.cc',
'src/msvc_helper_main-win32.cc',
'src/minidump-win32.cc',
'src/getopt.c'
]

sources = common_sources + common_test_sources

if sys.platform == 'win32':
    sources = sources + windows_sources
    compile_flags = '/nologo /GR- /wd4267 /wd4275 /wd4800 /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /D_HAS_EXCEPTIONS=0 /Ox /DNDEBUG /GL-'
else:
    sources = sources + unix_sources
    compile_flags = '-Wno-deprecated -fno-rtti -fno-exceptions'

module = Extension('ninjacore',
            sources=sources,
            include_dirs = ['.'],
            extra_compile_args = compile_flags.split(' ')
        )

setup(name='ninjacore', ext_modules = [module])
