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

proc build_computer {prefix suffix n} {
    return "${prefix}${n}${suffix}"
}

proc load_json {filename} {
    set fp [open $filename]
    set json_data [read $fp]
    close $fp
    return [json::json2dict $json_data]
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
#set fp [open [lindex $argv 0]]
#set json_data [read $fp]
#close $fp
#unset fp

# Parse the JSON file
set parsed_json [load_json [lindex $argv 0]]
# [json::json2dict $json_data]

# Build the domain names needed
set hosts [dict get $parsed_json hosts]
set clusters [dict get $parsed_json clusters]

foreach cluster $clusters {
    set prefix [dict get $cluster prefix]
    set suffix [dict get $cluster suffix]
    set start [dict get $cluster start]
    set end [dict get $cluster end]

    for {set i $start} {$i <= $end} {incr i} {
        lappend hosts [build_computer $prefix $suffix $i]
    }
}

# Prompt for username and password
puts "Username:"
gets stdin username

puts "Password:"
log_user 0

# Use expect to connect to all the machines


# Parse local JSON file into memory
# set local_json_fname "/home/batch_renderer/batch_render.config"
# set local_json [load_json local_json_fname]

# Set command name using local config file
set cmdpath ""
set cmd "killall ${cmdpath}"

# Close the connection
