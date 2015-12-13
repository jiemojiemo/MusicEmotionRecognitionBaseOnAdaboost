#usr/bin/env python
#-*- coding: utf-8 -*-

import win32api
import win32process
import win32event
import os

def main():
    rootdir = os.getcwd()
    exePath = rootdir + "\Music Feature Extract.exe"
    #exePath = "\"" +exePath+ "\""
    print(exePath)
    label = "";
    
    for parent, dirname, filenames in os.walk(rootdir):
        for filename in filenames:
            inputPath = os.path.join(parent, filename)
            if(inputPath.find(".wav") != -1):
                param = inputPath
                if(inputPath.find("exic_") != -1):
                    label = "exci"
                if(inputPath.find("plea_") != -1):
                    label = "plea"
                if(inputPath.find("quie_") != -1):
                    label = "quie"
                if(inputPath.find("sad_") != -1):
                    label = "sad"
                param = "\"" +exePath+ "\"" + " " + label + " \"" + param + "\""
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
