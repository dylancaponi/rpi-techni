#! /usr/bin/python

import os
import urllib2
import subprocess

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

def internet_on():
	# check to see if raspi is connected to the internet
	try:
		response=urllib2.urlopen('http://www.google.com',timeout=1)
		print "connected to internet";
		return True
	except urllib2.URLError as err: pass
	print "no internet connection";
	return False	

def internet_on2():
	# this method doesn't really do anything yet

	# check for ip on network
	#http://www.raspberrypi.org/forums/viewtopic.php?t=16054 
	#http://stackoverflow.com/questions/3503879/assign-output-of-os-system-to-a-variable-and-prevent-it-from-being-displayed-on
	proc = subprocess.Popen(["ifconfig","wlan0","|","grep","-q","'inet","addr:'"], stdout=subprocess.PIPE, shell=True)
	(out, err) = proc.communicate()
	print "program output:", out

def clientMode_on():
	# check which config file is active
	file = open('/etc/network/interfaces','r')
	data = file.read(2)
	if "0" in data:
		print "there's a 0 in the data"
		return False
	elif "1" in data:
		print "there's NOT a 0"
		return True
	else:
		print "config file headers have been altered \n program exiting"
		# how to exit program?

def switch2AP():
	# switch to wifi access point
	print 'switch to access point config'
	os.system('sudo cp ~/repos/wifiSwap/interfacesAccessPoint /etc/network/interfaces');
	
def switch2client():
	# switch to wifi client
	print 'switch to client config'
	os.system('sudo cp ~/repos/wifiSwap/interfacesClient /etc/network/interfaces');


print "start wifi swap";

# if client config active
if clientMode_on():
	# check if the internet is NOT connected
	if not ping():
		# if the internet is not connected switch to AP mode
		switch2AP();
		# then reboot
		print 'uncomment reboot'
		#os.system('sudo reboot');
	else:
		# do nothing, the internet is working
		print 'enjoy your working internet';
		# possibly email ip to yourself here?
else:
	print 'in access point mode'
	# check that we didn't just do this on last reboot
	#???	

	# switch to client mode
	#switch2client();

	# and reboot
