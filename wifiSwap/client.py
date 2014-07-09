#! /usr/bin/python

import os
import urllib2
import subprocess

def switch2client():
        # switch to wifi client
        print 'switch to client config'
        os.system('sudo cp ~/repos/wifiSwap/interfacesClient /etc/network/interfaces');

switch2client();
