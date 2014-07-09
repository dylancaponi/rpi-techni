#!/usr/bin/perl

my $MAC = 'b8:27:eb';
my @devs = @ARGV ? @ARGV : qw/bridge0 en0 en1/;

arp();
foreach my $dev (@devs)
{
	my ($ip) = `ifconfig $dev` =~ /inet\s+(\S+)\.\d+\s/;

	if ($ip)
	{	
		print "Scanning $dev $ip.1/24\n";
		`nmap -sn $ip.0/24`;
		arp();
	}

}

sub arp
{
	my ($match) = `arp -na` =~ /\((.*?)\)\s+at\s+$MAC/;
	if ($match)
	{
		my $ssh = "ssh pi\@$match";
		print "\n$ssh\n\n";

		exec($ssh);
		exit;
	}
}
