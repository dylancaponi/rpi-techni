import urllib, json
import urllib2


# this address will have to become dynamic
#url = 'http://10.0.0.1:8083/ZAutomation/api/v1/status'
url = 'http://10.0.0.1:8083/ZAutomation/api/v1/devices'

#response = urllib.urlopen(url);
response = urllib2.urlopen(url);

#data = json.loads(response.read()) # .load doesn't work, .loads does...


data_object = json.load(response)

#print data_object[0].items

#print "sup bitches"
print data_object["data"]

print data_object[1]
#k = [i for i, data_object, k in data_object[1]]

#for "data" in data
#	print "stuff"

#json_object = json.load(response)



# find number of devices listings in json
# figure out how to cycle through this!
# for loop!

#index = 0
#print data["data"]["devices"][index]["metrics"]["title"]
#print data["data"]["devices"][0]["metrics"]["title"]
#print data["data"]["devices"][1]["metrics"]["title"]
