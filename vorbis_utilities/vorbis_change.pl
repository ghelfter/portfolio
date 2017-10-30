#!/usr/bin/perl -w

# Galen Helfter
# vorbis_change.pl

# To use the script:
#  Call the script either by setting an alias in your shell configuration file,
#  or by executing it as an executable (chmod +x
#  [path_to_script]/vorbis_change.pl), or by running perl
#  [path_to_script]/vorbis_change.pl. It takes three arguments: the filepath,
#  the tag to be changed, and the value to set the tag to.

use strict;

# Subroutine to print the usage information, takes no arguments
sub print_usage
{
    print "Usage: ./vorbis_change.pl \[filepath\] \[tag\] \{NewValue\}\n";
    print "Optional tags:\n";
    print "    -h (help) will print help\n";
    print "    -c (case) will change the tag to standard case\n";
}

# Make sure we have the proper amount of arguments before iterating
# through them and attempting to acquire the information out of them
if((scalar (@ARGV) < 1))
{
    print "Improper usage.\n";
    exit(1);
}
elsif((scalar (@ARGV) < 2) && !($ARGV[0] eq "-h"))
{
    print "Improper usage.\n";
    exit(1);
}

# Check argument length
# Set up variables
my $end_tag = 0;
my $make_tag = 0;
my $fp = "";
my $tag = "";
my $val = "";
my $c_arg = 0;

# Iterate through and check for -h flag
foreach my $a (@ARGV)
{
    if($a eq "-h")
    {
        print_usage();
        $end_tag = 1;
    }
    elsif($a eq "-c" and !$end_tag)
    {
        $make_tag = 1;
        print "Will now use default generation.\n";
    }
    else
    {
        if($c_arg == 0)
        {
            $fp = $a;
            $c_arg += 1;
        }
        elsif($c_arg == 1)
        {
            $tag = $a;
            $c_arg += 1;
        }
        elsif($c_arg == 2 and !$make_tag)
        {
            $val = $a;
        }
    }
}

# Only do the operation if we aren't to terminate due to the given flags,
# such as the -h help flag.
if(!$end_tag)
{
    print "Now acquiring file.\n";

    # Wrap the fp in quotations to avoid problems with parentheses and
    # other special characters when executing the shell commands later
    my $quofp = "\"" . $fp . "\"";

    # Do our real processing here
    my $query = `vorbiscomment $quofp`;

    # Go through our elements
    my @elems = split('\n', $query);
    my @nelems = ();
    my $ref;
    my $ind = 0;
    my $found_elem = 0;

    # Could probably make this a bit more elegant
    foreach my $elem (@elems)
    {
        if($elem =~ m/^$tag/)
        {
            print "Acquired tag.\n";
            print $elem . "\n";
            # Push a temporary value onto the list, could use the actual
            # element instead, as a reference is maintained
            push(@nelems, "TEMP");
            $ref = \$nelems[$ind];
            $found_elem = 1;
            $ind++;
        }
        else
        {
            push(@nelems, $elem);
            $ind++;
        }
    }

    # Now to go ahead and reconstruct our new tag
    my $newtag = $tag . "=" . $val;

    # Add logic to check whether or not we found the tag; if we have
    # not found the tag, then we can just add it.
    if($found_elem)
    {
        # Update the reference
        $$ref = $newtag;
    }
    else
    {
        # Append the new tag if it did not exist beforehand
        push(@nelems, $newtag);
    }

    # Craft the final command.
    print "\nUpdated Elements:\n";
    my $cmd_str = "-w";
    foreach my $elem (@nelems)
    {
        print $elem . "\n";
        # Add quotations around the element to avoid shell string issues
        $cmd_str = $cmd_str . " -t " . "\"" . $elem . "\"";
    }

    # Call vorbiscomment
    `vorbiscomment $cmd_str $quofp`;

    # End the program
}
