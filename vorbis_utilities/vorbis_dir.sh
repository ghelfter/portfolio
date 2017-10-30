#!/bin/sh
# Galen Helfter
# vorbis_dir.sh

# Perl file to execute
readonly vorc_var="/home/ghelfte/Util/vorbis_change.pl"

# Usage printing function
print_usage ()
{
    echo "Usage: ./vorbis_dir.sh [TAG] [VALUE]"
}

# Check if we're given the help command
if [ $# -eq 1 ] && [ $1 = "-h" ]
then
    print_usage
# Check command line arguments
elif [ $# != 2 ]
then
    echo "Improper number of command line arguments"
# If proper, then execute our code
else
    for fn in *.ogg
    do
        echo "Converting filename $fn"
        $vorc_var "$fn" "$1" "$2" >> /dev/null
    done
fi
