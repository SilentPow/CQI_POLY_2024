import urllib.request
import json

def web_logger(data: dict) -> None:
    url = 'https://webhook.site/2eba67ba-7336-4d55-9679-9d36d810ffd6'

    data = json.dumps(data)
    data = data.encode('utf-8')

    req = urllib.request.Request(url, data=data, method='POST')
    req.add_header('Content-Type', 'application/json')

    urllib.request.urlopen(req)
