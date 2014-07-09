import json
import urllib2

data = urllib2.urlopen('http://suggestqueries.google.com/complete/search?hl=en&ds=yt&client=youtube&hjson=t&q=jum&cp=3')

j = json.load(data)
k = [i for i, j, k in j[1]]
l = json.dumps(k)
