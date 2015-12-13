#usr/bin/env python
#-*- coding: utf-8 -*-

import win32api
import win32process
import win32event
import os

def main():
    rootdir = os.getcwd()
    exePath = rootdir + "\Adaboost Training.exe"

    samples = {"exci.txt", "plea.txt", "quie.txt", "sad.txt"}
    
    for sample in samples:
        param = " -r 500 -v "
        param = param + sample
        param = "\"" + exePath + "\"" + param
        print(param)
        handle = win32process.CreateProcess(exePath,
                                            param,
                                            None,
                                            None,
                                            0,
                                            win32process.CREATE_NEW_CONSOLE,
                                            None,
                                            None,
                                            win32process.STARTUPINFO())
        win32event.WaitForSingleObject(handle[0],-1)
    
    
if '__main__' == __name__:
    main()
