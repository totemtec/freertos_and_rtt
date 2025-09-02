#!/usr/bin/perl -w
#==========================================================================
# daxpy_gendata.pl
#
# Author : Jerry Zhao
# Date   : Feb 10, 2023
#
(our $usageMsg = <<'ENDMSG') =~ s/^\#//gm;
#
# Simple script which creates an input data set and the reference data
# for the daxpy benchmark.
#
ENDMSG

use strict "vars";
use warnings;
no  warnings("once");
use Getopt::Long;

#--------------------------------------------------------------------------
# Command line processing
#--------------------------------------------------------------------------

our %opts;

sub usage()
{

  print "\n";
  print " Usage: daxpy_gendata.pl [options] \n";
  print "\n";
  print " Options:\n";
  print "  --help  print this message\n";
  print "  --size  size of input data [1000]\n";
  print "  --seed  random seed [1]\n";
  print "$usageMsg";

  exit();
}

sub processCommandLine()
{

  $opts{"help"} = 0;
  $opts{"size"} = 100;
  $opts{"seed"} = 1;
  Getopt::Long::GetOptions( \%opts, 'help|?', 'size:i', 'seed:i' ) or usage();
  $opts{"help"} and usage();

}

#--------------------------------------------------------------------------
# Helper Functions
#--------------------------------------------------------------------------

sub printArray
{
  my $arrayName = $_[0];
  my $arrayRef  = $_[1];

  my $numCols = 20;
  my $arrayLen = scalar(@{$arrayRef});

  print "static data_t ".$arrayName."[DATA_SIZE] = \n";
  print "{\n";

  if ( $arrayLen <= $numCols ) {
    print "  ";
    for ( my $i = 0; $i < $arrayLen; $i++ ) {
      # print sprintf("%3.2f",$arrayRef->[$i]);
      print sprintf("%3d",$arrayRef->[$i]);
      if ( $i != $arrayLen-1 ) {
        print ", ";
      }
    }
    print "\n";
  }

  else {
    my $numRows = int($arrayLen/$numCols);
    for ( my $j = 0; $j < $numRows; $j++ ) {
      print "  ";
      for ( my $i = 0; $i < $numCols; $i++ ) {
        my $index = $j*$numCols + $i;
        # print sprintf("%3.2f",$arrayRef->[$index]);
        print sprintf("%3d",$arrayRef->[$index]);
        if ( $index != $arrayLen-1 ) {
          print ", ";
        }
      }
      print "\n";
    }

    if ( $arrayLen > ($numRows*$numCols) ) {
      print "  ";
      for ( my $i = 0; $i < ($arrayLen-($numRows*$numCols)); $i++ ) {
        my $index = $numCols*$numRows + $i;
        # print sprintf("%3.2f",$arrayRef->[$index]);
        print sprintf("%3d",$arrayRef->[$index]);
        if ( $index != $arrayLen-1 ) {
          print ", ";
        }
      }
      print "\n";
    }

  }

  print  "};\n\n";
}

#--------------------------------------------------------------------------
# Main
#--------------------------------------------------------------------------

sub main()
{

  processCommandLine();
  srand($opts{"seed"});
  rand(19);
  my @values1;
  my @values2;
  my @sum;
  for ( my $i = 0; $i < $opts{"size"}; $i++ ) {
    # my $value1 = int(rand(12));
    # my $value2 = int(rand(7));
    my $value1 = int(rand(60));
    my $value2 = int(rand(100));
    push( @values1, $value1 );
    push( @values2, $value2 );
    push( @sum, $value1 + $value2);
  }


  print "\n\#define DATA_SIZE ".$opts{"size"}." \n\n";
  printArray( "input1_data", \@values1 );
  printArray( "input2_data", \@values2 );
  printArray( "verify_data", \@sum );

}

main();

