import hashlib
import itertools
import string

def generatePermutations():
  characters = []
  
  for x in list(map(chr, range(ord('a'), ord('z')+1))):
    characters.append(x)
  for x in list(map(chr, range(ord('A'), ord('Z')+1))):
    characters.append(x)
  for x in list(map(chr, range(ord('0'), ord('9')+1))):
    characters.append(x)

  file = open("permutations.txt","w")
  for i in range(1,5):
    for y in itertools.product(characters, repeat=i):
      file.write(''.join(y)+'\n')
  file.close()

def formatxsplit():                       # Makes a file containing only the hash values found in xsplit
    xsplitold = open("xsplit_leak.txt", "r", errors='ignore')
    xsplitnew = open("xsplit.txt", "w")
    for lines in xsplitold:
        length = len(lines.split())
        try:
            xsplitnew.write(lines.split()[length-1].strip()+'\n')
        except IndexError:
            pass
    xsplitnew.close()
    xsplitold.close()

def stripYahoo():                         # Puts passwords found in yahoo file into a password dictionary
  yahoo = open("yahoo.txt", "r", errors='ignore')
  passwordDictionary = open("PasswordDictionary.txt", "w")
  for lines in yahoo:
    length = len(lines.split(':'))
    passwordDictionary.write(lines.split(':')[length-1])
  yahoo.close()
  passwordDictionary.close()

def formatDictionary():                   # Removes repeated entries from dictionary
  unique = []
  
  passwordDictionary = open("PasswordDictionary.txt", "r")
  for line in passwordDictionary:
    if line in unique:
      pass
    else:
      unique.append(line)
  passwordDictionary.close()

  passwordDictionary = open("PasswordDictionary.txt", "w")
  for i in unique:
    passwordDictionary.write(i+'\n')
  passwordDictionary.close()

def determineHash(length):
    if length == 32:
        return "MD5"
    if length == 40:
        return "SHA-1"
    if length == 64:
        return "SHA-256"

def getInfo():                            # Finds the length of the hash values found in the files
    formspring = open("formspring.txt", "r")
    length = len(formspring.readline().strip())
    function = determineHash(length)
    print('{:10}'.format("formspring"), length, function)
    formspring.close()

    linkedin = open("Linkedin.txt", "r")
    length = len(linkedin.readline().strip())
    function = determineHash(length)
    print('{:10}'.format("linkedin"), length, function)
    linkedin.close()

    xsplit = open("xsplit.txt", "r")
    length = len(xsplit.readline().strip())
    function = determineHash(length)
    print('{:10}'.format("xsplit"), length, function)
    xsplit.close()

def isSalted():                           # Uses the most popular passwords to check if the file is salted
    canarySHA1 = []
    canarySHA256 = []

    canaries = open("canaries.txt", "r")
    for lines in canaries:
        canarySHA1.append(hashlib.sha1(lines.strip().encode()).hexdigest())
        canarySHA256.append(hashlib.sha256(lines.strip().encode()).hexdigest())
    canaries.close()

    found = 0
    xsplit = open("xsplit.txt", "r")
    for lines in xsplit:
        if found == 1:
            break
        for element in canarySHA1:
            if lines.strip() == element:
                found = 1
                print("xsplit Not Salted")
                break
    xsplit.close()
   
    found = 0
    linkedin = open("Linkedin.txt", "r")
    for lines in linkedin:
        if found == 1:
            break
        for element in canarySHA1:
            if lines.strip() == element:
                found = 1
                print("linkedin Not Salted")
                break
    linkedin.close()

    found = 0
    formspring = open("formspring.txt", "r")
    for lines in formspring:
        if found == 1:
            break
        for element in canarySHA256:
            if lines.strip() == element:
                found = 1
                print("formspring Not Salted")
                break
    formspring.close()

def xsplitAttack():                       # Compares stolen hash value to hashed values of known plaintext
                                          # passwords
  xsplit = open("xsplit.txt", "r")
  passwordDictionary = open("PasswordDictionary.txt", "r")
  brokenPasswords = open("BrokenXSPLIT.txt", "w")
  
  for lines in xsplit:
    passwordDictionary.seek(0)
    for passwords in passwordDictionary:
      hashedPassword = passwords.split(':')[1]
      if lines.strip() == hashedPassword:
        print('Broke a hash')
        brokenPasswords.write('{}:{}\n'.format(lines.strip(),passwords.split(':')[0]))
        break

  xsplit.close()
  passwordDictionary.close()
  brokenPasswords.close()

def linkedinAttack():                   # Compares stolen hash value to hashed values of known plaintext
                                        # passwords (just the final 35 characters of the hash)
  linkedin = open("linkedin.txt", "r")
  passwordDictionary = open("PasswordDictionary.txt", "r")
  brokenPasswords = open("BrokenLINKEDIN.txt", "w")
  
  for lines in linkedin:
    passwordDictionary.seek(0)
    for passwords in passwordDictionary:
      hashedPassword = passwords.split(':')[1]
      if ''.join(list(lines.strip())[5:40]) == ''.join(list(hashedPassword.strip())[5:40]):
        print('Broke a hash')
        brokenPasswords.write('{}:{}\n'.format(lines.strip(),passwords.split(':')[0]))
        break

  linkedin.close()
  passwordDictionary.close()
  brokenPasswords.close()

def hashPasswords():                        # Takes the password dictionary and appends the SHA1 & SHA256 hash
                                            # of each line to that line
  passwordDictionary = open("PasswordDictionary.txt", "r")
  temp = open("temp.txt", "w")
  
  for lines in passwordDictionary:
    sha1 = hashlib.sha1(lines.strip().encode()).hexdigest()
    sha256 = hashlib.sha256(lines.strip().encode()).hexdigest()
    temp.write('{}:{}:{}\n'.format(lines.strip(),sha1,sha256))
    passwordDictionary.readline()
  
  temp.close()
  passwordDictionary.close()

  passwordDictionary = open("PasswordDictionary.txt", "w")
  temp = open("temp.txt", "r")

  for lines in temp:
    passwordDictionary.write(lines)

  temp.close()
  passwordDictionary.close()

def simpleFormspringAttack():
  plaintextPasswords = open("canaries.txt","r")
  permutations = open("permutations.txt", "r")
  formspring = open("formspring.txt", "r")

  for z in formspring:
    plaintextPasswords.seek(0)
    for x in plaintextPasswords:
      permutations.seek(0)
      for y in permutations:
        if len(y)>3:
          break
        saltedPassword1 = x.strip()+y.strip()
        saltedPassword2 = y.strip()+x.strip()
        hash1 = hashlib.sha256(saltedPassword1.encode()).hexdigest()
        hash2 = hashlib.sha256(saltedPassword2.encode()).hexdigest()
        if z.strip() == hash1:
          print('Found Salt Appended',z,x,y)
          break
        if z.strip() == hash2:
          print('Found Salt Prepended',z,y,x)
          break














