#! /usr/bin/python

import os

def ping():
	try:
		output = os.system('ping -c 2 -W 2 www.google.com')
		print output
		if output == 0:
			#connected to internet
			print 'successfully pinged google'
			return True
		else:
			#not connected to internet
			print 'cannot ping google'
			return False
	except:
		print "ping error"
		return False

ping();
