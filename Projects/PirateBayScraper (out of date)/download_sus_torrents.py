import urllib.request as request
from bs4 import BeautifulSoup
import webbrowser

# A function to request and return a webpages html doc
def getPage(url):
    r = request.Request(url)
    r.add_header('User-Agent', 'Mozilla/5.0')
    page = request.urlopen(r)
    page = page.read().decode('utf-8')
    return page

url = 'https://thepiratebay.org'

flaggedlist = []
downloadedlist = []
notdownloadedlist = []

flagged = open('Flagged.txt', 'r')
downloaded = open('Downloaded.txt', 'r')

for lines in flagged:
    flaggedlist.append(lines)
for lines in downloaded:
    downloadedlist.append(lines)

for i in flaggedlist:
    if i not in downloadedlist:
        notdownloadedlist.append(i)

flagged.close()
downloaded.close()

for i in notdownloadedlist:
    #Get HTML DOC
    page = getPage(url+i)
    #Scrape it for magnetlink
    tree = BeautifulSoup(page, 'html.parser')
    magnet = tree.find(attrs={'class' : 'download'})
    magnet = magnet.a['href']
    #open magnet link
    print('Attempting to download a flaggeed torrent...')
    webbrowser.open(magnet)
    #add to downloaded log
    downloaded = open('Downloaded.txt', 'a')
    downloaded.write(i)
    downloaded.close()

