#!/usr/bin/python
#===- hsail_gdb_test.py --------------------------------------------------===#
#
#                     The HSA Simulator
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
#===----------------------------------------------------------------------===#
import math
import unittest
import random
import os
import filecmp
import sys
import re
import imp

from hsail_gdb_test_common import *


int_re = r"[-+]?[0-9]+"
flt_re = r"[-+]?[0-9]+\.?[0-9]+"

fib_hsail_filenm = "fib.hsail"
fib_exenm = "fib"

VectorCopy_hsail_filenm = "VectorCopy.hsail"
VectorCopy_exenm = "vectorCopy"

fcos_hsail_filenm = "fcos.hsail"
fcos_exenm = "fcos"


def expect_match(gdb_outputs, ground_truth):
    test_ok = True
    cmp_results = re.match(ground_truth, gdb_outputs)
    if cmp_results == None:
       test_ok = False

    return test_ok



class TestGdbPrintTypesOnEachLine(unittest.TestCase):


    def check_types_on_each_line_pretty_printer(self, exenm, hsail_filenm, linenum_arr, gdb_cmd, gt):
         total_line_number = len(linenum_arr)
         lines = gdb_print_types_on_lines(exenm,hsail_filenm,linenum_arr, gdb_cmd)
         gt_len = len(gt.splitlines())

         lines_iter = iter(lines)
         for a_line in linenum_arr:
             tr = ""
             for x in range(gt_len):
                 line1 = lines_iter.next()
                 tr = tr + line1
             self.assertEqual(gt, \
                              tr, \
                              "\n\n"
                              "   ************************************************************\n"
                              "  |  When printing types under GDB \" \n" + \
                              "  |       " + ''.join(gdb_cmd.splitlines()) + "\"\n"
                              "  |  at \"line " + str(a_line) + "\"\n"
                              "  |        The real output is \n"
                              "  |            { " + ''.join(tr.splitlines()) + " }\n"
                              "  |        The correct answer should be \n"
                              "  |            { " + ''.join(gt.splitlines()) + " }\n"
                              "   ************************************************************\n"
             )


    def test_fib(self):
        linenum_arr =[13,14,15,21,22,23,24,30,31,32,33,35,36,37,38,40,41]
        gdb_cmd = "ptype r\nptype n\nptype hsa$c1\nptype hsa$s1\nptype hsa$s2\nptype hsa$s3\nptype p\nptype nm2\nptype res\n"
        gt = "type = s32 &\n" \
             "type = s32 &\n" \
             "type = b1\n" \
             "type = union sReg {\n" \
             "    b32 b32;\n" \
             "    f32 f32;\n" \
             "}\n" \
             "type = union sReg {\n" \
             "    b32 b32;\n" \
             "    f32 f32;\n" \
             "}\n" \
             "type = union sReg {\n" \
             "    b32 b32;\n" \
             "    f32 f32;\n" \
             "}\n" \
             "type = s32\n" \
             "type = s32\n" \
             "type = s32\n"

        self.check_types_on_each_line_pretty_printer(fib_exenm, fib_hsail_filenm, linenum_arr, gdb_cmd, gt)


    def test_fibKernel(self):
        linenum_arr =[49,50,51,52,53,54]
        gdb_cmd = "ptype r_ptr\nptype n_ptr\nptype r\nptype n\nptype hsa$s0\nptype hsa$s1"
        gt = "type = s32 &\n" \
             "type = s32 &\n" \
             "type = s32\n" \
             "type = s32\n" \
             "type = union sReg {\n" \
             "    b32 b32;\n" \
             "    f32 f32;\n" \
             "}\n" \
             "type = union sReg {\n" \
             "    b32 b32;\n" \
             "    f32 f32;\n" \
             "}\n"

        self.check_types_on_each_line_pretty_printer(fib_exenm, fib_hsail_filenm, linenum_arr, gdb_cmd, gt)

    def test_fcos(self):
        linenum_arr =[24,25,29,30,33,34,37,38,41,43]
        gdb_cmd = "ptype arg_val0\nptype arg_val1\nptype hsa$s0\nptype hsa$s1\nptype hsa$s2\nptype hsa$s3"
        gt = "type = u32 &\n" \
             "type = u32 &\n" \
             "type = union sReg {\n" \
             "    b32 b32;\n" \
             "    f32 f32;\n" \
             "}\n" \
             "type = union sReg {\n" \
             "    b32 b32;\n" \
             "    f32 f32;\n" \
             "}\n" \
             "type = union sReg {\n" \
             "    b32 b32;\n" \
             "    f32 f32;\n" \
             "}\n" \
             "type = union sReg {\n" \
             "    b32 b32;\n" \
             "    f32 f32;\n" \
             "}\n"

        self.check_types_on_each_line_pretty_printer(fcos_exenm, fcos_hsail_filenm, linenum_arr, gdb_cmd, gt)


    def test_VectorCopy(self):
        linenum_arr =[17,18,19,20,21,22,23,25,26,27,28,29,30,31]
        gdb_cmd = "ptype arg_val0\nptype arg_val1\nptype arg_val2\nptype hsa$c0\nptype hsa$s0\nptype hsa$s1\nptype hsa$s2\nptype hsa$s3\n"
        gt = "type = u32 &\n" \
             "type = u32 &\n" \
             "type = u32 &\n" \
             "type = b1\n" \
             "type = union sReg {\n" \
             "    b32 b32;\n" \
             "    f32 f32;\n" \
             "}\n" \
             "type = union sReg {\n" \
             "    b32 b32;\n" \
             "    f32 f32;\n" \
             "}\n" \
             "type = union sReg {\n" \
             "    b32 b32;\n" \
             "    f32 f32;\n" \
             "}\n" \
             "type = union sReg {\n" \
             "    b32 b32;\n" \
             "    f32 f32;\n" \
             "}\n"

        self.check_types_on_each_line_pretty_printer(VectorCopy_exenm, VectorCopy_hsail_filenm, linenum_arr, gdb_cmd, gt)

class TestGdbCheckArgSanityUponEntry(unittest.TestCase):

    def check_arg_sanity_pretty_printer(self, exenm, hsail_filenm, krnlnm, ignore_count, fmt, argList, ground_truth):
        line1 = gdb_check_arg_sanity_upon_entry(exenm,hsail_filenm,krnlnm, ignore_count, fmt, argList)
        self.assertEqual(expect_match(line1,ground_truth), \
                         True, \
                         "\n\n" + \
                         "   ************************************************************\n"
                         "  |  When printing \"" + argList + "\" at \"" + krnlnm + "\"\n" + \
                         "  |        The real output is {" + line1 + "}\n" + \
                         "  |        The correct answer should be {" + ground_truth + "}\n"
                         "   ************************************************************\n"
        )

    def test_fibKernel(self):
        ground_truth = int_re + ",5"
        krnlnm = "fib.hsail:49"
        ignore_count = 4
        fmt = "%d,%d\\n"
        argList = "r_ptr, n_ptr"
        self.check_arg_sanity_pretty_printer(fib_exenm, fib_hsail_filenm, krnlnm, ignore_count, fmt, argList, ground_truth)

    def test_fib(self):
        ground_truth = "^.*,1"
        krnlnm = "fib.hsail:13"
        ignore_count = 0
        fmt = "%d,%d\\n"
        argList = "r, n"
        self.check_arg_sanity_pretty_printer(fib_exenm, fib_hsail_filenm, krnlnm, ignore_count, fmt, argList, ground_truth)

    def test_fcos(self):
        ground_truth = "-3.14159[0-9]+, " + flt_re
        krnlnm = "fcos.hsail:21"
        ignore_count = 0
        fmt = "%f, %f\\n"
        argList = "*(float *)arg_val0, *(float *)arg_val1"
        self.check_arg_sanity_pretty_printer(fcos_exenm, fcos_hsail_filenm, krnlnm, ignore_count, fmt, argList, ground_truth)

    def test_VectorCopy(self):
        ground_truth = "3.14159[0-9]+,0.00000+,16"
        krnlnm = "VectorCopy.hsail:17"
        ignore_count = 0
        fmt = "%f,%f,%d\\n"
        argList = "*(float*)arg_val0, *(float*)arg_val1, arg_val2"
        self.check_arg_sanity_pretty_printer(VectorCopy_exenm, VectorCopy_hsail_filenm, krnlnm, ignore_count, fmt, argList, ground_truth)


class TestCheckLinesBreakProperly(unittest.TestCase):

    def check_lines_break_properly_pretty_printer(self, exenm, hsail_filenm, linenum_arr):
        total_line_number = len(linenum_arr)
        lines = gdb_check_set_breakpoints(exenm,hsail_filenm,linenum_arr)
        lines_iter = iter(lines)

        for a_line in linenum_arr:
            line1 = lines_iter.next()
            s1 = (line1.rstrip()).split()
            lineno = int(s1[1],0)

            self.assertEqual(lineno, a_line, \
                             "\n\n" + \
                             "   ************************************************************\n"
                             "  |  When check info line at \"" + hsail_filenm + ":" + str(a_line) + "\"\n" + \
                             "  |        The real output is {" + str(lineno) + "}\n" + \
                             "  |        The correct answer should be {" + str(a_line) + "}\n"
                             "   ************************************************************\n"
            )

    def test_fibKernel(self):
        linenum_arr = [49,50,51,52,53,54]
        self.check_lines_break_properly_pretty_printer(fib_exenm, fib_hsail_filenm, linenum_arr)

    def test_fcos(self):
        linenum_arr = [24,25,29,30,33,34,37,38,41]
        self.check_lines_break_properly_pretty_printer(fcos_exenm, fcos_hsail_filenm, linenum_arr)

    def test_fib(self):
        linenum_arr = [13,14,15,21,22,23,24,30,31,32,33,35,36,37,38,40,41]
        self.check_lines_break_properly_pretty_printer(fib_exenm, fib_hsail_filenm, linenum_arr)

    def test_VectorCopy(self):
        linenum_arr =[18,19,20,21,22,23,25,26,27,28,29,31]
        self.check_lines_break_properly_pretty_printer(VectorCopy_exenm, VectorCopy_hsail_filenm, linenum_arr)



class TestGdbStepping(unittest.TestCase):

    def check_gdb_stepping_pretty_printer(self, exenm,filenm,line_num,ignore_count,gdb_cmd,ground_truth):
        line1 = gdb_script_and_test(exenm, filenm, line_num, ignore_count, gdb_cmd)
        self.assertEqual(expect_match(line1, ground_truth), \
                         True, \
                         "\n\n" + \
                         "   ************************************************************\n"
                         "   |   GDB Stepping failed at " + filenm + ":" + str(line_num) + "\n"
                         "   ************************************************************\n"
        )


    def test_fib(self):
        self.check_gdb_stepping_pretty_printer(fib_exenm, fib_hsail_filenm, 23, 0, "s", r"fib\s+")
        self.check_gdb_stepping_pretty_printer(fib_exenm, fib_hsail_filenm, 32, 0, "s", r"fib\s+")
        self.check_gdb_stepping_pretty_printer(fib_exenm, fib_hsail_filenm, 51, 0, "s", r"fib\s+")


class TestGdbCheckRegisterContents(unittest.TestCase):

    def check_gdb_register_contents_pretty_printer(self, exenm,hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth):
        line1 = gdb_script_and_test(exenm, hsail_filenm, line_num, ignore_count, gdb_cmd)
        self.assertEqual(expect_match(line1, ground_truth), \
                     True, \
                     "\n\n" + \
                     "   ************************************************************\n"
                     "  |  When checking reg contents at \"" + hsail_filenm + ":" + str(line_num) + "\"\n" + \
                     "  |        GDB command used: " + gdb_cmd + "\n" + \
                     "  |        The real output is " + line1 + "\n" + \
                     "  |        The correct answer should be " + ground_truth + "\n"
                     "   ************************************************************\n"
        )

    def test_fib(self):
        gdb_cmd = "printf \"%d,%d,%d,%d\\n\",res,nm2,hsa$s1.b32,hsa$s2.b32"

        line_num = 33
        ground_truth = "1,1,3,2"
        ignore_count = 0
        self.check_gdb_register_contents_pretty_printer(fib_exenm, fib_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

        ground_truth = "1,1,3,2"
        ignore_count = 1
        self.check_gdb_register_contents_pretty_printer(fib_exenm, fib_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

        ground_truth = "1,2,4,3"
        ignore_count = 2
        self.check_gdb_register_contents_pretty_printer(fib_exenm, fib_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

        ground_truth = "1,1,3,2"
        ignore_count = 3
        self.check_gdb_register_contents_pretty_printer(fib_exenm, fib_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)


        line_num = 24
        ground_truth = "1,1,3,1"
        ignore_count = 0
        self.check_gdb_register_contents_pretty_printer(fib_exenm, fib_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

        ground_truth = "1,2,4,2"
        ignore_count = 1
        self.check_gdb_register_contents_pretty_printer(fib_exenm, fib_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

        ground_truth = "1,1,3,1"
        ignore_count = 2
        self.check_gdb_register_contents_pretty_printer(fib_exenm, fib_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

        ground_truth = "1,1,3,1"
        ignore_count = 3
        self.check_gdb_register_contents_pretty_printer(fib_exenm, fib_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

    def test_fibKernel(self):
        gdb_cmd = "printf \"%d,%d,%d\\n\", r_ptr, n_ptr, hsa$s0.b32"
        line_num = 49
        ground_truth = int_re + "," + "1," + int_re
        ignore_count = 0
        self.check_gdb_register_contents_pretty_printer(fib_exenm, fib_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

        gdb_cmd = "printf \"%d,%d,%d\\n\",r,n,hsa$s0.b32"
        line_num = 53
        ground_truth = "1,1,1"
        ignore_count = 0
        self.check_gdb_register_contents_pretty_printer(fib_exenm, fib_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

        line_num = 51
        ground_truth = int_re + "," + "1," + "1"
        ignore_count = 0
        self.check_gdb_register_contents_pretty_printer(fib_exenm, fib_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

    def test_fcos(self):
        gdb_cmd = "printf \"%f,%f,%f,%f\\n\",hsa$s0,hsa$s1,hsa$s2,hsa$s3"
        line_num = 38
        ground_truth = "-3.14159[0-9]+," + flt_re + "," + flt_re + "," + flt_re
        ignore_count = 0
        self.check_gdb_register_contents_pretty_printer(fcos_exenm, fcos_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

        line_num = 41
        ground_truth = "-3.14159[0-9]+," + flt_re + "," + "-1.000000" + "," + flt_re
        ignore_count = 0
        self.check_gdb_register_contents_pretty_printer(fcos_exenm, fcos_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

        line_num = 43
        ground_truth = "-3.14159[0-9]+," + flt_re + "," + "-1.000000" + "," + flt_re
        ignore_count = 0
        self.check_gdb_register_contents_pretty_printer(fcos_exenm, fcos_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

    def test_VectorCopy(self):
        gdb_cmd = "printf \"%d\\n\",(hsa$s1.b32 > hsa$s0.b32)"
        line_num = 20
        ground_truth = "0"
        ignore_count = 0
        self.check_gdb_register_contents_pretty_printer(VectorCopy_exenm, VectorCopy_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

        gdb_cmd = "printf \"%d,%f,%f,%f\\n\",hsa$c0,hsa$s0,hsa$s1,hsa$s2"
        line_num = 25
        ground_truth = "0," + flt_re + "," + flt_re + "," + flt_re
        ignore_count = 0
        self.check_gdb_register_contents_pretty_printer(VectorCopy_exenm, VectorCopy_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

        line_num = 29
        ground_truth = "0," + flt_re + ",3.14159[0-9]+," + flt_re
        ignore_count = 0
        self.check_gdb_register_contents_pretty_printer(VectorCopy_exenm, VectorCopy_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)

        gdb_cmd = "printf \"%f,%f,%d\\n\",arg_val0, arg_val1, arg_val2"
        line_num = 18
        ground_truth = "0.00000+,0.00000+,16"
        ignore_count = 0
        self.check_gdb_register_contents_pretty_printer(VectorCopy_exenm, VectorCopy_hsail_filenm,line_num,ignore_count,gdb_cmd,ground_truth)


suite0 = unittest.TestLoader().loadTestsFromTestCase(TestGdbStepping)
suite1 = unittest.TestLoader().loadTestsFromTestCase(TestGdbCheckRegisterContents)
suite2 = unittest.TestLoader().loadTestsFromTestCase(TestCheckLinesBreakProperly)
suite3 = unittest.TestLoader().loadTestsFromTestCase(TestGdbCheckArgSanityUponEntry)
suite4 = unittest.TestLoader().loadTestsFromTestCase(TestGdbPrintTypesOnEachLine)
suitelist = [suite0, suite1, suite2, suite3, suite4]

os.environ["SIMNOOPT"] = "1"


try:
    imp.find_module('argparse')
    found_argparse = True
    import argparse
except ImportError:
    found_argparse = False
    import getopt

if found_argparse:
    parser = argparse.ArgumentParser()
    parser.add_argument("-t", "--testselect", help="Select an individual test to run.", dest='case_no', action="store")
    parser.add_argument("-s", "--stoponfail", help="Stop the tests on the first failure.", action="store_true")
    args = parser.parse_args()

    if args.case_no:
        print("testselect turned on with case no = %s" % args.case_no)
        alltests = unittest.TestSuite([suitelist[int(args.case_no)]])
    else:
        alltests = unittest.TestSuite(suitelist)

    if args.stoponfail:
        print("stoponfail turned on: testing will stop on the first failure!")
        unittest.TextTestRunner(verbosity=2,failfast=True).run(alltests)
    else:
        unittest.TextTestRunner(verbosity=2,failfast=False).run(alltests)
else:
    case_no = -1
    stoponfail = False
    try:
        opts, args = getopt.getopt(sys.argv[1:],"hst:",["help","stoponfail","testselect="])
    except getopt.GetoptError:
        print "usage: hsail_gdb_test.py [-h] [-t CASE_NO] [-s]\n" + \
              " optional arguments:\n" + \
              "  -h, --help            Show this help message and exit\n" + \
              "  -t CASE_NO, --testselect CASE_NO\n" + \
              "                        Select an individual test to run.\n" + \
              "  -s, --stoponfail      Stop the tests on the first failure.\n"
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-h","--help"):
            print "usage: hsail_gdb_test.py [-h] [-t CASE_NO] [-s]\n" + \
                  " optional arguments:\n" + \
                  "  -h, --help            Show this help message and exit\n" + \
                  "  -t CASE_NO, --testselect CASE_NO\n" + \
                  "                        Select an individual test to run.\n" + \
                  "  -s, --stoponfail      Stop the tests on the first failure.\n"
            sys.exit()
        elif opt in ("-t", "--testselect"):
            case_no = arg
        elif opt in ("-s", "--stoponfail"):
            stoponfail = True

    if case_no > 0:
        print("testselect turned on with case no = %s" % case_no)
        alltests = unittest.TestSuite([suitelist[int(case_no)]])
    else:
        alltests = unittest.TestSuite(suitelist)

    if stoponfail == True:
        print("stoponfail turned on: testing will stop on the first failure!")
        unittest.TextTestRunner(verbosity=2,failfast=True).run(alltests)
    else:
        unittest.TextTestRunner(verbosity=2,failfast=False).run(alltests)
