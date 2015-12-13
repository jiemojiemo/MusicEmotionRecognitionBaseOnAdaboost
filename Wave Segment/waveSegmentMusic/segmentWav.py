#usr/bin/env python
#-*- coding: utf-8 -*-

import win32api
import win32event
import win32process
import os

def main():
    rootdir = os.getcwd();
    exePath = rootdir + "/Wave Segment.exe"
    print(exePath)
    for parent, dirname, filenames in os.walk(rootdir):
        for filename in filenames:
            fullPath = os.path.join(parent, filename);
            if(fullPath.find(".wav") != -1):
                param = '\"' + fullPath +'\"'
                print(param)
                #win32api.ShellExecute(0,
                #                      "open",
                #                      "E:\python_wk\callExeOnWin\WaveSegmenter.exe",
                #                     param,
                #                      '',
                #                      1)

                #exe = "E:\python_wk\callExeOnWin\helloSomthing.exe"
                param = "\""+exePath+"\"" + " " + param
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
                #print(fullPath + " done.\n")

if '__main__' == __name__:
    main()
