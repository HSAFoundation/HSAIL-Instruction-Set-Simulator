#!/usr/bin/python
#===- hsail_gdb_test_common.py -------------------------------------------===#
#
#                     The HSA Simulator
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
#===----------------------------------------------------------------------===#
import os
import math
import re
import tempfile

## Behave similar to the gdb_test function in Expect
def gdb_print_types_on_lines(exenm,filenm,linenum_arr,gdb_cmd):
    test_ok = True
    tmpfn = tempfile.mkstemp()
    gdb_str = "set logging file " + tmpfn[1] + "\n" \
              "set pagination off\n" \
              "set logging overwrite off  \n" \
              "set logging off  \n" \
              "set verbose off  \n" \
              "set logging redirect on \n" \
              "set confirm off \n" \
              "set print thread-events off \n" \
              "set breakpoint pending on \n"


    total_line_number = len(linenum_arr)
    for a_line in linenum_arr:
        gdb_str = gdb_str + \
                  "break "+ filenm + ":" + str(a_line) + "\n"

    gdb_str = gdb_str + "r\n"

    bpid = 0
    for a_line in linenum_arr:
        bpid = bpid + 1
        gdb_str = gdb_str + "set logging on\n"
        gdb_str = gdb_str + gdb_cmd + "\n"
        gdb_str = gdb_str + "set logging off\n"
        gdb_str = gdb_str + "c\n"

    gdb_str = gdb_str + "quit\n"

    scriptfn = tempfile.mkstemp()
    f = open(scriptfn[1],'w')
    f.write(gdb_str);
    f.close()

    exec_gdb_cmd = "gdb -q " + exenm + " -x " + scriptfn[1] + " > /dev/null 2>&1";
    os.system(exec_gdb_cmd);

    os.remove(scriptfn[1])

    g = open(tmpfn[1],'r')
    lines = g.read()
    g.close()
    os.remove(tmpfn[1])

    return lines.splitlines(lines.count('\n'))



## Behave similar to the gdb_test function in Expect
def gdb_script_and_test(exenm,filenm,line_num,ignore_count,gdb_cmd):
    test_ok = True
    tmpfn = tempfile.mkstemp()
    gdb_str = "set logging file " + tmpfn[1] + "\n" \
              "set pagination off\n" \
              "set logging overwrite off \n" \
              "set logging off \n" \
              "set verbose off \n" \
              "set logging redirect on \n" \
              "set confirm off \n" \
              "set print thread-events off \n" \
              "set breakpoint pending on \n"

    gdb_str = gdb_str + \
              "define hook-stop \n" \
              "  set logging on \n" + \
              "  " + gdb_cmd + "\n" \
              "  set logging off \n" \
              "  quit \n" \
              "end\n"

    gdb_str = gdb_str + \
              "break " + filenm  + ":" + str(line_num) + "\n" + \
              "ignore 1 " + str(ignore_count) + "\n";
    gdb_str = gdb_str + \
              "run\n" + \
              "quit \n"

    scriptfn = tempfile.mkstemp()
    f = open(scriptfn[1],'w')
    f.write(gdb_str);
    f.close()

    exec_gdb_cmd = "gdb -q " + exenm + " -x " + scriptfn[1] + " > /dev/null 2>&1";
    os.system(exec_gdb_cmd);

    os.remove(scriptfn[1])

    g = open(tmpfn[1],'r');
    gdb_outputs = g.readline()
    g.close();
    os.remove(tmpfn[1])

    return gdb_outputs.replace("\n","")



# Tests, upon entering a function, whether GDB can recognize
# the arguments correctly.
def gdb_check_arg_sanity_upon_entry(exenm, filenm, krnlnm, ignore_count, fmt, argList):
    test_ok = True
    tmpfn = tempfile.mkstemp()
    gdb_str = "set logging file " + tmpfn[1] + "\n" \
              "set pagination off\n" \
              "set logging overwrite off\n" \
              "set logging off\n" \
              "set verbose off\n" \
              "set logging redirect on\n" \
              "set confirm off\n" \
              "set print thread-events off \n" \
              "set breakpoint pending on\n"

    gdb_str = gdb_str + \
              "define hook-stop \n" \
              "  set logging on \n" \
              "  printf \"" + fmt + "\", " + argList + "\n" \
              "  set logging off \n" \
              "  quit \n" \
              "end\n"

    gdb_str = gdb_str + \
              "break "+ krnlnm + "\n" + \
              "ignore 1 " + str(ignore_count) + "\n";

    gdb_str = gdb_str + "r\n"

    gdb_str = gdb_str + "quit\n"

    scriptfn = tempfile.mkstemp()
    f = open(scriptfn[1],'w')
    f.write(gdb_str);
    f.close()

    exec_gdb_cmd = "gdb -q " + exenm + " -x " + scriptfn[1] + " > /dev/null 2>&1";
    os.system(exec_gdb_cmd);

    os.remove(scriptfn[1])

    g = open(tmpfn[1],'r');
    lines = g.read().splitlines()
    line1 = lines[0];
    g.close()
    os.remove(tmpfn[1])

    return line1



# Tests, for a given kernel, whether GDB can break each line
# within the kernel correctly.
def gdb_check_set_breakpoints(exenm,filenm,linenum_arr):
    tmpfn = tempfile.mkstemp()
    gdb_str = "set logging file " + tmpfn[1] + "\n" \
              "set pagination off\n"\
              "set logging overwrite off  \n"\
              "set logging off  \n"\
              "set verbose off  \n"\
              "set logging redirect on \n"\
              "set confirm off \n"\
              "set print thread-events off \n" \
              "set breakpoint pending on \n"

    gdb_str = gdb_str + \
              "break "+ filenm + ":" + str(linenum_arr[0]) + "\n"
    gdb_str = gdb_str + "r\n"
    gdb_str = gdb_str + "set logging on\n"
    gdb_str = gdb_str + "info line\n"
    gdb_str = gdb_str + "set logging off\n"
    gdb_str = gdb_str + "d\n"

    total_line_number = len(linenum_arr)
    bpid = 0
    for i in range(1,total_line_number):
        bpid = bpid + 1
        gdb_str = gdb_str + \
                  "break "+ filenm + ":" + str(linenum_arr[i]) + "\n"
        gdb_str = gdb_str + "c\n"
        gdb_str = gdb_str + "set logging on\n"
        gdb_str = gdb_str + "info line\n"
        gdb_str = gdb_str + "set logging off\n"
        gdb_str = gdb_str + "d\n"


    gdb_str = gdb_str + "quit\n"

    scriptfn = tempfile.mkstemp()
    f = open(scriptfn[1],'w')
    f.write(gdb_str);
    f.close()

    exec_gdb_cmd = "gdb -q " + exenm + " -x " + scriptfn[1] + " > /dev/null 2>&1";
    os.system(exec_gdb_cmd);

    os.remove(scriptfn[1])

    g = open(tmpfn[1],'r');
    lines = g.read()
    g.close()
    os.remove(tmpfn[1])

    return lines.splitlines(lines.count('\n'))
