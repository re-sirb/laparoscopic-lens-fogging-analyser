# Laparoscopic-lens-fogging-analyser

Laparoscopic-lens-fogging-analyser is a simple command line tool to quantify the amount of lens-fogging in a video file, performing frame-by-frame analysis in the frequency domain.

The program depends on OpenCV 3.x.

## Usage:
```console
foo@bar:~$ ./laparoscopic-lens-fogging-analyser [file]
```

When executed successfully it writes a quantity of dimension one to stdout, describing the amount of lens-fogging. The smaller the number, the more lens-fogging was detected in the video.


