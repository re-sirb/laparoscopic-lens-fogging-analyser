# Laparoscopic-lens-fogging-analyser

Laparoscopic-lens-fogging-analyser is a simple command line tool to quantify the amount of lens-fogging in a video file, performing frame-by-frame analysis in the frequency domain.

**This is a work in progress! The Version published here is not yet optimized and does miss a feature for baseline calibration.**

We are currently working on a paper comparing the performance of this tool to other methods used to quantify laparoscopic-lens-fogging described in literature. 
Upon acceptance we will insert a link, so you can review if this tool is suitable for your research.
If you have used this tool for your refearch, we would love to hear your feedback.


The program depends on OpenCV 3.x.

## Usage:
```console
foo@bar:~$ ./laparoscopic-lens-fogging-analyser [video_file]
```

When executed successfully it writes a quantity of dimension one to stdout, describing the amount of lens-fogging. The smaller the number, the more lens-fogging was detected in the video.


Have fun!


