#! /usr/bin/python

import os
import urllib2
import subprocess

def internet_on():
        # check to see if raspi is connected to the internet
        try:
                # response=urllib2.urlopen('http://www.google.com',timeout=1)
                # should just do an os ping instead
		test = os.system('ping www.google.com');
		print "connected to internet";
                print test
		return True
        except urllib2.URLError as err: pass
        print "no internet connection";
        return False

internet_on();
