#!/usr/bin/expect

# ****************************************
#              Galen Helfter
#           ghelfter@gmail.com
#            batch_killall.tcl
# ****************************************

# Script to kill all of the rendering processes on a given render queue,
# defined by the JSON file provided. Uses expect to SSH into all of the
# machines, load the local JSON files, then kill the processes.

package require json

proc print_usage {} {
    puts "Usage: ./batch_killall.tcl {config_file}"
}

# Check arguments
if {$argc < 1} {
    print_usage
    exit 1
}

# Assert json file exists
if {![file exists [lindex $argv 0]]} {
    puts "JSON file not found."
    exit 1
}

# Load JSON into memory
set fp [open [lindex $argv 0]]
set json_data [read $fp]
close $fp

# Parse the JSON file
