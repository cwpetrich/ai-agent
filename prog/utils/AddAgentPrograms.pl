#!/usr/bin/perl
use Getopt::Std;
use strict;

my %Options;
my $ok = getopts('A:M:S:T', \%Options);
my $argerr = 0;
  
if(!$ok || !$Options{'A'})
{
  print STDERR "getopts failed.\n";
  die "Usage: $0 -A agent_file.pm -M dir/Makefile.agents -S dir/SourceFile.cpp\n";
}

# loads $aps
our $aps;
require $Options{'A'};

if($Options{'M'})
{
  create_make_file($aps,$Options{'M'});
}
if($Options{'S'})
{
  create_source_file($aps, $Options{'S'});
}
if($Options{'T'})
{
  create_test_run($aps);
}

sub create_test_run
{
  my ($aps) = @_;
  my $fh;
  my $ap;
  my $args = "-w 4 -h 4 -s 1000 -d 0 -t 0 -D 0 -R 1000 -p .005 -M -1 -S -2 -z 9845129";
  
  open($fh, "> test_run.bash") || (die "open: $!");

  print $fh '#!/bin/bash
';
  foreach $ap (@{$aps})
    {
      print $fh "echo -n '$ap->{name}:  ' \n";
      print $fh "./RunProg ./VW_Tester $args -a $ap->{key} | egrep ^Average \n";
    }
  close($fh);
  chmod 0755, "test_run.bash";
}

sub create_make_file
{
  my ($aps,$filename) = @_;
  my $fh;
  my $ap;
  my $c;
  
  open($fh, "> $filename") || (die "open: $filename: $!");

  print $fh 'CXX_SRC := $(CXX_SRC) ';
  foreach $ap (@{$aps})
    {
      if(! $ap->{remote})
	{
	  foreach $c (@{$ap->{cpp}})
	    {
	      print $fh "$c ";
	    }
	}
    }
  print $fh "\n";
  
  print $fh 'CXX_HEADERS := $(CXX_HEADERS) ';
  foreach $ap (@{$aps})
    {
      foreach $c (@{$ap->{h}})
	{
	  print $fh "$c ";
	}
    }
  print $fh "\n";
  
  print $fh 'REMOTE_SRC := $(REMOTE_SRC) ';
  foreach $ap (@{$aps})
    {
      if($ap->{remote})
	{
	  foreach $c (@{$ap->{cpp}})
	    {
	      print $fh "$c ";
	    }
	}
    }
  print $fh "\n";
  
  close($fh);
}

sub create_source_file
{
  my ($aps, $filename) = @_;
  my $fh;
  
  open($fh, "> $filename") || (die "open: $filename: $!");

  print $fh '
#include <ai_agent.h>

';

  display_includes($aps, $fh);

  print $fh '
namespace ai
{
  namespace Agent
  {
    AgentProgram *AgentProgramSelect(Options *opts)
    {
      AgentProgram *agent_program = 0;
      switch(opts->GetArgChr("agent"))
        {
';

  display_constructors($aps, $fh);

  print $fh '
        default:
          std::cout << "Unknown agent program " << std::endl
';

  display_options($aps, $fh);

  print $fh '      
		    << std::endl;
          exit(1);
          break;
        }
      return agent_program;
    }
  }
}
';
  close($fh);
}


sub display_includes
{
  my ($aps, $fh) = @_;
  my $ap;
  
  foreach $ap (@{$aps})
    {
      print $fh '#include "'.$ap->{header}.'"'."\n";
    }
}

sub display_constructors
{
  my ($aps, $fh) = @_;
  my $ap;
  
  foreach $ap (@{$aps})
    {
      print $fh "        case '".$ap->{key}."':\n";
      print $fh "          agent_program = new ".$ap->{constructor}.";\n";
      print $fh "          break;\n";
    }
}

sub display_options
{
  my ($aps, $fh) = @_;
  my $ap;
  
  foreach $ap (@{$aps})
    {
      print $fh '                    << "'.$ap->{key}.' : '.$ap->{name}.'" << std::endl'."\n";
    }
}
