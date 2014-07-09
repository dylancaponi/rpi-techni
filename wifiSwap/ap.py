#! /usr/bin/python

import os

def switch2AP():
        # switch to wifi access point
        print 'switch to access point config'
        os.system('sudo cp ~/repos/wifiSwap/interfacesAccessPoint /etc/network/interfaces');

switch2AP();

