import requests

result = requests.get('http://192.168.1.5:8083/ZAutomation/api/v1/devices').json()

print result

line = '---------'
print line


#print result[0]

#print line
#print result[0].items

print result['data']
