import urllib.request as request
from bs4 import BeautifulSoup
from glob import glob
import fileinput
import urllib
import json
import os

# A function that goes through the logs and records
# the urls of every torrent and puts them in an array
''' This Function Needs To Be Worked On - Priority Should Be Given To New
    Torrents and After A Certain Period Of Time A Torrent Should Be Checked
    For Comments Less Often.'''
def retreiveLinks(broken):
    links = []
    logfiles = glob('Log_PirateBay_*.txt')
    for lines in fileinput.input(logfiles):
        data = json.loads(lines)
        if data['Name'] in broken:
            pass
        else:
            links.append(data['Name'])
    return links

# A function to request and return a webpages html doc
def getPage(url):
    r = request.Request(url)
    r.add_header('User-Agent', 'Mozilla/5.0')
    page = request.urlopen(r)
    page = page.read().decode('utf-8')
    return page

# A function that scrapes the comments of a torrent and creates a
# dictionary entry where commenter and time of comment are the key
# and the value is the actual comment.  This dictionary is then dumped
# into a text file in JSON format.
def scrapeComments(links):
    brokenlinks = []
    commentlog = open('PirateBay_Comments.txt', 'w')
    print('Checking', len(links), 'Torrents For Comments...')
    for link in links:
        try:
            page = getPage(url+link)
        except urllib.error.HTTPError:
            print('HTTP Error 404:', url + link)
            brokenlinks.append(link)
            continue
        tree = BeautifulSoup(page, 'html.parser')
        commenterslist = []
        commenttimelist = []
        commentslist = []
        commentdict = {}
        try:
            numcomments = int(tree.find(id ='NumComments').string)
        except AttributeError:
            print('AttributeError raised on', link)
        
        if numcomments > 0:
            for tag in tree.find_all(attrs={'class' : 'byline'}):
                commenterslist.append(tag.a['href'])
            for tag in tree.find_all(attrs={'class' : 'byline'}):
                commenttimelist.append(str(tag.contents[2]).strip('[]\n\t\s\'\"'))
            for tag in tree.find_all(attrs={'class' : 'comment'}):
                commentslist.append(str(tag.contents).strip('[]\n\t\s\'\"br'))
            if len(commentslist) == len(commenterslist) == len(commenttimelist):
                for i in range(0, len(commentslist)):
                    key = str(commenterslist[i]) + str(commenttimelist[i])
                    commentdict[key] = commentslist[i]
            else:
                print('ERROR in scrapeComments(): unequal lists')
                
            json.dump({'Name' : link,
                      'Comments' : commentdict},
                      commentlog,
                      separators=(',',':'))
            commentlog.write('\n')
    commentlog.close()
    return brokenlinks
            
# A function that checks comments against a list of words.
# If one of these words is in the comment then the torrent is flagged.
# A flagged torrent file should be downloaded immediately for further
# investigation.
def flagger():
    print('Checking Comments For Indicators Of Malware...')
    commentlog = open('PirateBay_Comments.txt', 'r')
    badwords = open(os.path.dirname(__file__) + '/../badwords.txt', 'r')
    flagged = open('Flagged.txt', 'a')
    for lines in commentlog:
        data = json.loads(lines)
        for keys, values in data['Comments'].items():
            badwords.seek(0)
            comment = str(values).lower()
            for words in badwords:
                if words.strip() in comment.strip():
                    print(data['Name'],'==> Flagged')
                    flagged.write(data['Name'])
                    flagged.write('\n')
    commentlog.close()
    badwords.close()
    flagged.close()

    noDuplicates()

# Remove Duplicates
def noDuplicates():
    unique = []
    flagged = open('Flagged.txt', 'r')
    for lines in flagged:
        if lines not in unique:
            unique.append(lines)
    flagged.close()
    flagged = open('Flagged.txt', 'w')
    for i in unique:
        flagged.write(i)
    flagged.close()

if __name__ == "__main__":           
    url = 'https://thepiratebay.org'    
    broken = []
    brokenlinkslog = open('BrokenLinks.txt', 'r')
    for i in brokenlinkslog:
        broken.append(i.strip())        
    links = retreiveLinks(broken)
    brokenlinks = scrapeComments(links)
    brokenlinkslog = open('BrokenLinks.txt', 'a')
    for i in brokenlinks:
        brokenlinkslog.write(i)
        brokenlinkslog.write('\n')
    brokenlinkslog.close()
    flagger()
