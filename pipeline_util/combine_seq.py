#!/usr/bin/python3

# **************************************************
# *                  Galen Helfter
# *              ghelfte@g.clemson.edu
# *                  combine_seq.py
# **************************************************/

import os
import sys
import subprocess

def print_usage():
    print("Usage: ./combine_seq.py [sequence-prefix] [result-dir] [pref]"+
           " [ext] {[list-of-shots]}")

def create_result_dir(result_dir):
    subprocess.call(['mkdir', result_dir])

def move_file(original, result):
    subprocess.call(['cp', original, result])

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
    if len(argv) < 6:
        print_usage()
        sys.exit(1)
    seq_pref = argv[1]
    result_dir = argv[2]
    pref = argv[3]
    ext = argv[4]
    shots = argv[5:]

    # create resulting directory
    create_result_dir(result_dir)

    i = 0
    # Go through all the shots
    for shot in shots:
        # Build directory
        if int(shot) < 10:
            shot = '0' + shot
        src_dir = seq_pref + shot

        for f in os.listdir(src_dir):
            if f.endswith(ext):
                res_file = build_name(result_dir + '/' + pref, i, ext)
                # Copy filename to new file
                move_file((src_dir + '/' + f), res_file)
                i += 1


if __name__ == "__main__":
    main(sys.argv)
