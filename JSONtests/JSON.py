import json
from pprint import pprint

with open('test2.json') as data_file:
	data2 = json.load(data_file)
	print(data2)
	pprint(data2)
	data_file.close()

with open('test.json') as data_file:
	data = json.load(data_file)
	pprint(data)
	data_file.close()

# picking part of the data to print
print data["a"][3]
