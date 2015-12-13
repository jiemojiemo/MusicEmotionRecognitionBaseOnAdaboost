#usr/bin/env python
#-*- coding: utf-8 -*-

import win32api
import win32event
import win32process
import os

def main():
    rootdir = os.getcwd();
    exePath = rootdir + "/ffmpeg.exe"
    print(exePath)
    for parent, dirname, filenames in os.walk(rootdir):
        for filename in filenames:
            inputPath = os.path.join(parent, filename);
            if(inputPath.find(".mp3") != -1):
                outputPath = filename.replace("mp3","wav")
                if(inputPath.find("excited") != -1):
                    outputPath = "exci_" + outputPath
                if(inputPath.find("pleasant") != -1):
                    outputPath = "plea_" + outputPath
                if(inputPath.find("quiet") != -1):
                    outputPath = "quie_" + outputPath
                if(inputPath.find("sad") != -1):
                    outputPath = "sad_" + outputPath
                param = " -i " + "\""+inputPath+"\"" + " -ac 1 -sample_fmt s16 " + '\"'+outputPath+'\"'
                param = "\""+exePath+"\"" + param
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
                ret = win32event.WaitForSingleObject(handle[0],-1)
if '__main__' == __name__:
    main()
