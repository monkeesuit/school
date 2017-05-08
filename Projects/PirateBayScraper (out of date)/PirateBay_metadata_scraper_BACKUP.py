import urllib.request as request
from bs4 import BeautifulSoup
from datetime import timedelta
from datetime import date
import datetime
import urllib
import time
import json
import re
import os

#The PirateBay URL Format:
#https://thepiratebay.org/browse/{category}/{page}/{order}

#CATEGORIES:
#100 = Audio
#200 = Video
#300 = Applications
#400 = Games
#500 = Porn
#600 = Other

#PAGE:
#First page = 0

#ORDER:
#3 = Upload Date (newest first) (6 Hour Time Difference)
#4 = Upload Date (oldest first)
#5 = Size (Largest First)
#6 = Size (Smallest First)
#7 = Seeders (Descending)
#8 = Seeders (Ascending)
#9 = Leechers (Descending)
#10 = Leechers (Ascending)
#11 = Uploader (?)
#12 = Uploader (?)

# A function to request and return a webpages html doc
def getPage(url):
    r = request.Request(url)
    r.add_header('User-Agent', 'Mozilla/5.0')
    page = request.urlopen(r)
    page = page.read().decode('utf-8')
    return page

# A function to parse the browse page html doc
# and extract the links to torrents individual page 
def parseBrowsePage(page):
    linkslist = []
    tree = BeautifulSoup(page, 'html.parser')
    links = tree.find_all(attrs={'class' : 'detLink'})
    for i in links:
        linkslist.append(i['href'])
    return linkslist

# A function that scrapes a torrents page for
# the time it was uploaded, the user who uploaded it,
# the users reputation, the magnet link, and comments
def parseTorrentPage(page):
    tree = BeautifulSoup(page, 'html.parser')
    # Scrape time
    time = tree.find(string = re.compile('(.*):(.*):(.*) GMT'))
    # Scrape user name
    try:
        usernode = tree.find(href = re.compile('/user/'))
        user = usernode['href']
    except TypeError:
        user = 'anonymous'
    # Scrape reputation
    try:
        i = 0
        for sibling in usernode.next_siblings:
            i += 1
        if i > 0:
            reputation = True
        else:
            reputation = False
    except AttributeError:
        reputation = False
    # Scrape magnet link
    magnet = tree.find(attrs={'class' : 'download'})
    magnet = magnet.a['href']

    return time, user, reputation, magnet

# A function that dumps the scraped information into a
# text file using JSON format
def writeInfo(torrentlinks, time, user, reputation, magnet):
    dailylog = open('Log_PirateBay_{}.txt'.format(today), 'a')
    json.dump({'Name' : torrentlinks,
               'Time' : time,
               'User' : user,
               'Reputation' : reputation,
               'Magnet' : magnet},
              dailylog,
              separators=(',',':'))
    dailylog.write('\n')
    dailylog.close()

# A function that checks the last entry into the log to
# see if it was uploaded more than 24 hours ago (the upload
# time is in GMT)
def timeCheck():
    dailylog = open('Log_PirateBay_{}.txt'.format(today), 'r')
    for lines in dailylog:
        lastline = lines
        pass
    dailylog.close()
    lastline = json.loads(lastline)
    time = str(lastline['Time'])
    time = time.strip(' GMT ')
    time = datetime.datetime.strptime(time,  '%Y-%m-%d %X')
    ago = (datetime.datetime.now() + timedelta(hours=4) - timedelta(days=1))
    print('Time Check:', time)
    print('          :', ago)
    print()
    if time > ago:
        return False
    else:
        return True
    
# A function that removes those entries that are older
# than 24 hours
def cleanUp():
    dailylog = open('Log_PirateBay_{}.txt'.format(today), 'r')
    temp = open('temp.txt', 'w')
    for lines in dailylog:
        line = json.loads(lines)
        time = str(line['Time'])
        time = time.strip(' GMT ')
        time = datetime.datetime.strptime(time,  '%Y-%m-%d %X')
        ago = (datetime.datetime.now() + timedelta(hours=4) - timedelta(days=1))
        if time > ago:
            json.dump(line, temp)
            temp.write('\n')
    dailylog.close()
    temp.close()
    
    dailylog = open('Log_PirateBay_{}.txt'.format(today), 'w')
    temp = open('temp.txt', 'r')
    for lines in temp:
        line = json.loads(lines)
        json.dump(line, dailylog)
        dailylog.write('\n')
    dailylog.close()
    temp.close()
    os.remove('temp.txt')

# A function to scrape thePirateBay, if the last entry on the page
# is less than 24 hours old then the function calls itself again,
# except it now scrapes the next page.  When th elast entry of the
# page is older than 24 hours it runs a clean up function to remove
# all entries older than 24 hours
def Scrape(category = '300', startingpage = '0', order = '3'):
    print('Working...')
    browse = '/browse/{}/{}/{}/'.format(category, startingpage, order)
    page = getPage(url + browse)
    linkslist = parseBrowsePage(page)
    for torrentlinks in linkslist:
        try:
            torrentpage = getPage(url+torrentlinks)
        except urllib.error.HTTPError:
            print('HTTP Error 404:', url + torrentlinks, 'Not found')
            continue
        time, user, reputation, magnet = parseTorrentPage(torrentpage)
        writeInfo(torrentlinks, time, user, reputation, magnet)
        print('Name      :', torrentlinks)
        print('Time      :', time)
        print('User      :', user)
        print('Reputation:', reputation)
        print('Magnet    :', magnet)
        print()
    if timeCheck() == False:
        Scrape(category, str(int(startingpage)+1), order)
    else:
        cleanUp()
        
if __name__ == "__main__":
    url = 'https://thepiratebay.org'
    today = date.today().isoformat()
    Scrape()


        

