# lab-framegrabber-macosx
A program written to grab frames from an input video using an input .csv file.
This is the Mac conversion for original framegrabber project. (Images below showcase Windows version)


Input:
Upon start up, FrameGrabber will prompt the user to input the name of the video file and the corresponding .csv file. These files should be placed within a folder named input inside the program binaries' root directory. 
It will then scan through the .csv file and grab intervals named "Air Active", "Air Passive", and "Interim". 
![Input](http://imgur.com/4Rtev4d.png)


Filter:
The user will be asked whether or not they want to filter out the intervals by length. If so, an input will be taken and all intervals less than this number in seconds will be pruned. 
Note: This program will not modify any of the videos or .csv files in any way, so the original data will be safe!
![Filter](http://i.imgur.com/zRoogMt.png)

Active or Passive:
The user will be asked if they are grabbing frames for the active or passive rat. This is purely for file naming schema. 


Iteration:
FrameGrabber will go through all of the intervals and generate a random frame for the current one. The user will be given the option to either accept, reject, or skip. The controls for each are a, r, and S, respectively. User input is only read when the frame window is active.
![Iteration1](http://i.imgur.com/oH9mLu0.png)
![Iteration2](http://i.imgur.com/sN7rN89.png)

  Accept: This saves the frame to the output folder in a subdirectory named after the video. By default, the program looks for three accepted frames per interval. 
Files saved will be in the format similar to: 
CSD2122sub-111615-A00_04_9, or 
videoname-A(P if passive)minute_second_1000msinterval.jpg.

  Reject: This rejects the current frame and has the program randomly generate another one within the interval.

  Skip: This skips the current interval entirely and moves onto the next one.


This repeats until all intervals are done.
