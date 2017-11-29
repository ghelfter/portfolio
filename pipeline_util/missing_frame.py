#!/usr/bin/python3

# **************************************************
# *                  Galen Helfter
# *              ghelfte@g.clemson.edu
# *                missing_frame.py
# **************************************************/

import sys
import subprocess

def print_usage():
    print("Usage: ./missing_frame.py [sequence-name] [start] [end] [ext]")

def print_missing_frame(i):
    print("Missing frame " + str(i))

# This will fix a missing frame by copying the previous frame
def fix_missing_frame(seq_name, frame, ext):
    new_i = 0
    if frame <= 0:
        new_i = 1
    else:
        new_i = frame-1
    new_name = build_name(seq_name, new_i, ext)
    name = build_name(seq_name, frame, ext)
    subprocess.call(['cp', new_name, name])


def build_name(seq_name, frame, ext):
    frame_str = ""
    if frame < 10:
        frame_str = '000' + str(frame)
    elif frame < 100:
        frame_str = '00' + str(frame)
    elif frame < 1000:
        frame_str = '0' +  str(frame)
    else:
        frame_str = str(frame)

    return seq_name + frame_str + '.' + ext

def main(argv):
    if len(argv) < 5:
        print_usage()
    else:
        # Acquire sequence information
        seq_name = argv[1]
        start = int(argv[2])
        end = int(argv[3])
        ext = argv[4]
        missing = []

        # Go through each frame
        for i in range(start, end):
            try:
                name = build_name(seq_name, i, ext)
                fd = open(name, 'r')
                fd.close()
            except IOError:
                print_missing_frame(i)
                missing.append(i)
        for elem in missing:
            fix_missing_frame(seq_name, elem, ext)


if __name__ == "__main__":
    main(sys.argv)
