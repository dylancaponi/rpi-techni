#! /usr/bin/python

import subprocess

def internet_on2():
        # check for ip on network
        #http://www.raspberrypi.org/forums/viewtopic.php?t=16054 
        #http://stackoverflow.com/questions/3503879/assign-output-of-os-system-to-a-varia$
        proc = subprocess.Popen(["ifconfig","wlan0","|","grep","-q","'inet","addr:'"], stdout=subprocess.PIPE, shell=True)
        (out, err) = proc.communicate()
        print "program output:", out

	# this isn't actually returning a true or false right now

internet_on2();
