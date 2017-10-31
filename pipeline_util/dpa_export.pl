#!/usr/bin/perl -w
# Galen Helfter
# ghelfter@gmail.com
#
# To make this executable, run chmod +x [filename]
# It already has the appropriate thing at the top for the system to
# find the proper interpreter.
#
# To add this to your path, you can put
#       alias [alias name]="[path to this file]"
# in your .bashrc file

use strict;

sub print_help
{
    print "Available commands:\n";
    print "  -h : Prints out help\n";
    print "  -e \[filepath\] \[name\] \[version\]: exports given file\n";
    print "  -c \[filepath\] exports using stored hidden files\n";
}

sub acquire_file_info ($$$)
{
    open FPATH, $_[0];
    while(<FPATH>)
    {
        my @line = split /:/;
        # Check to make sure we have two items in the split
        if(not (scalar(@line) > 1))
        {
            # Do nothing
        }
        elsif(lc($line[0]) eq "version")
        {
            $_[2] = int($line[1]);
        }
        elsif(lc($line[0]) eq "name")
        {
            $_[1] = $line[1];
            chomp $_[1];
        }
    }
}

sub write_file_info($$$$)
{
    open OFILE, '>', $_[0];

    print OFILE "NAME:" . $_[1] . "\n";
    print OFILE "VERSION:" . $_[2] . "\n";
    print OFILE "FPATH:" . $_[3] . "\n";

    close OFILE;
}

sub generate_cfg_loc($)
{
    my $working = shift;
    # Here, test for the string we want
    if($working =~ m/\.\/.*/)
    {
        $working =~ s/\.\///;
    }

    $working = "./." . $working . ".cfg";

    return $working;
}

sub dpa_update($$$)
{
    system("dpa", "version", "work");
    system("dpa", "create", "product", "-p", $_[0], "-v",
           $_[1], $_[2], ".=");

    print "Finished creating and versioning the product!\n";
    print "Use the command dpa update product -p $_[1] \[spec\]";
    print " to complete the process.\n\n";
    print "To find the spec, you can run dpa list products\n";
}

if(scalar(@ARGV) > 0)
{
    if($ARGV[0] eq "-h")
    {
        print_help;
    }
    elsif($ARGV[0] eq "-c")
    {
        my $name = "";
        my $version = 0;
        my $c_fpath = generate_cfg_loc($ARGV[1]);

        # Acquire config info
        acquire_file_info($c_fpath, $name, $version);
        $version += 1;
        write_file_info($c_fpath, $name, $version, $c_fpath);
        dpa_update($ARGV[1], $version, $name);
    }
    elsif($ARGV[0] eq "-e")
    {
        if(scalar(@ARGV) > 3)
        {
            my $name = $ARGV[2];
            my $version = int($ARGV[3]);
            my $c_fpath = generate_cfg_loc($ARGV[1]);
            write_file_info($c_fpath, $name, $version, $c_fpath);
            dpa_update($ARGV[1], $ARGV[3], $ARGV[2]);
        }
        else
        {
            print "Usage: cmd -e \[filepath\] \[name\] \[version\]\n";
        }
    }
}
else
{
    print "Error, no command given.\n";
}
