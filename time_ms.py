#!/usr/bin/python
# coding:utf-8

import datetime
import time
import sys

def get_ms():
    t1 = datetime.datetime.now().microsecond
    t2 = time.mktime(datetime.datetime.now().timetuple())
    return t2 * 1000 + t1 / 1000


if __name__ == '__main__':
        n = 1
        num = 0

        t1 = get_ms()
        for i in range(10000000):
                if n == 0:
                        num += 1
                else:
                        num += 2
        print "int compare:" + str(num)
        num = 0
        t2 = get_ms()
        strTime = 'time use:%dms' % (t2 - t1)
        print(__file__,sys._getframe().f_lineno)
        print strTime