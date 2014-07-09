import requests

result = requests.get('https://api.angel.co/1/jobs').json()

print result
