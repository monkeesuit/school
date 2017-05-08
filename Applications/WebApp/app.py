from flask import Flask, render_template, request, json, session, redirect, url_for
from flask_mysqldb import MySQL
from hashlib import sha256
import string
import uuid
import os






### [Flask] ###
app = Flask(__name__)
app.secret_key = 'how much wood could a woodchuck chuck if a woodchuck could chuck wood?'
app.config['IMAGES'] = 'static/images'



### [MySQL] ### => flask-mysqldb.readthedocs.io/en/latest/#flask_mysqldb.MySQL
mysql = MySQL()
# MySQL configuration
app.config['MYSQL_HOST'] = 'localhost'
app.config['MYSQL_USER'] = 'root'
app.config['MYSQL_PORT'] = 3306
app.config['MYSQL_PASSWORD'] = 'fubar'
app.config['MYSQL_DB'] = 'ImageUpload'
mysql.init_app(app)



# Default setting
itemlimit = 10

# For Sanitization
AcceptedCharacters = []
for i in string.ascii_letters:
	AcceptedCharacters.append(i)
for i in string.digits:
	AcceptedCharacters.append(i)
AcceptedCharacters.append(' ')
AcceptedCharacters.append('@')
AcceptedCharacters.append('.')

# For Salting Password
salt = 'abcdefg'





'''###$$$***###$$$***###$$$***###$$$***###$$$***###$$$***'''
#                       [FUNCTIONS]                       #
'''###$$$***###$$$***###$$$***###$$$***###$$$***###$$$***'''

def InputSanitization(input):
	for character in input:
		if character not in AcceptedCharacters:
			return 0
	return 1


### [Submit Delete] ###
@app.route("/SubmitDelete", methods=["POST"])
def SubmitDelete():

	# Make sure the user has proper credentials
	if session.get('user'):
		userid = session.get('user')
	else:
		return render_template('error.html',error = 'Unauthorized Access')

	name = request.form['name']

	# Retrieve Database Data
	cursor = mysql.connection.cursor()
	cursor.callproc('procedure_Delete',(userid, name,))
	data = cursor.fetchall()

	if len(data) is 0:
		mysql.connection.commit()
		return redirect('/')
	else:
		return render_template('error.html',error = 'An error occurred!')



### [Submit Edit] ###
@app.route("/SubmitEdit", methods=["POST"])
def SubmitEdit():

    # Make sure the user has proper credentials
    if session.get('user'):
    	userid = session.get('user')
    else:
    	return render_template('error.html',error = 'Unauthorized Access')

    name = request.form['name']
    caption = request.form['caption']

	# Sanitize Input
    check = InputSanitization(name)
    if check == 0:
        return render_template('error.html',error = 'An error occurred!')
    check = InputSanitization(caption)
    if check == 0:
    	return render_template('error.html',error = 'An error occurred!')

    # Retrieve Database Data
    cursor = mysql.connection.cursor()
    cursor.callproc('procedure_UpdateCaption',(userid, name, caption,))
    data = cursor.fetchall()

    if len(data) is 0:
    	mysql.connection.commit()
    	return redirect('/')
    else:
    	return render_template('error.html',error = 'An error occurred!')



### [Get Photo By Name] ###
@app.route("/GetPhotoByName", methods=["POST"])
def GetPhotoByName():

    # Make sure the user has proper credentials
    if session.get('user'):
    	userid = session.get('user')
    else:
    	return render_template('error.html',error = 'Unauthorized Access')

    name = request.form['name']

    # Retrieve Database Data
    cursor = mysql.connection.cursor()
    cursor.callproc('procedure_GetPhotoByName',(name, userid,))
    data = cursor.fetchall()

    # Parse Data
    post_dictionary = []

    for post in data:
        post_dictionary_entry = {
        'Name':post[3],
        'htmlName':post[3].replace(' ',''),
        'Path':post[4],
        'Caption':post[5]
        }
        post_dictionary.append(post_dictionary_entry)

    return json.dumps(post_dictionary)



### [Get User Photos] ###
@app.route("/GetUserPhotos", methods=["POST"])
def GetUserPhotos():

    # Make sure the user has proper credentials
    if session.get('user'):
    	userid = session.get('user')
    else:
    	return render_template('error.html',error = 'Unauthorized Access')

    limit = itemlimit
    offset = request.form['o']
    total = 0

    # Retrieve Database Data
    cursor = mysql.connection.cursor()
    cursor.callproc('procedure_GetUserPhotos',(userid, limit, offset, total,))
    data = cursor.fetchall()

    cursor.close()
    cursor = mysql.connection.cursor()
    cursor.execute('SELECT @_procedure_GetUserPhotos_3')
    output = cursor.fetchall()

    # Parse Data
    response=[]
    post_dictionary = []
    
    for post in data:
        post_dictionary_entry = {
        'Name':post[3],
        'htmlName':post[3].replace(' ',''),
        'Path':post[4],
        'Caption':post[5]
        }
        post_dictionary.append(post_dictionary_entry)
    
    response.append(post_dictionary)
    response.append({'total':output[0][0]})

    return json.dumps(response)



### [Get All Photos] ###
@app.route("/GetAllPhotos", methods=["POST"])
def GetAllPhotos():

    limit = itemlimit
    offset = request.form['o']
    total = 0

    # Retrieve Database Data
    cursor = mysql.connection.cursor()
    cursor.callproc('procedure_GetAllPhotos', (limit, offset, total))
    data = cursor.fetchall()

    cursor.close()
    cursor = mysql.connection.cursor()
    cursor.execute('SELECT @_procedure_GetAllPhotos_2')
    output = cursor.fetchall()

    # Parse Data
    response=[]
    post_dictionary = []
    
    for post in data:
        post_dictionary_entry = {
        'Name':post[3],
        'Path':post[4],
        'Caption':post[5]
        }
        post_dictionary.append(post_dictionary_entry)
    
    response.append(post_dictionary)
    response.append({'total':output[0][0]})

    return json.dumps(response)




### [Submit] ###
@app.route("/Submit", methods=["POST"])
def Submit():

    # Attribute upload to a user or declare it public
    if session.get('user'):
    	userid = session.get('user')
    else:
    	userid = 0

    # Pull user data from form
    name = request.form['filename']
    path = request.form['filepath']
    caption = request.form['caption']

	# Sanitize Input
    check = InputSanitization(name)
    if check == 0:
    	return render_template('error.html',error = 'An error occurred!')
    check = InputSanitization(caption)
    if check == 0:
    	return render_template('error.html',error = 'An error occurred!')

    # Connect to MySQL DB ImageUpload and run Upload procedure
    cursor = mysql.connection.cursor()
    cursor.callproc('procedure_Upload',(userid, name, path, caption))
    data = cursor.fetchall()

    if len(data) is 0:
    	mysql.connection.commit()
    	return redirect('/')
    else:
    	return render_template('error.html',error = 'An error occurred!')



### [Upload] ###
@app.route("/Upload", methods=["POST", "GET"])
def Upload():
    if request.method == "POST":
    	file = request.files['file']
    	extension = os.path.splitext(file.filename)[1]
    	filename = str(uuid.uuid4()) + extension
    	file.save(os.path.join(app.config['IMAGES'], filename))
    	return json.dumps({'filename':filename})



### [Log Out] ###
@app.route("/LogOut")
def LogOut():
    # Remove users session token
    session.pop('user',None)
    return redirect("/")



### [Log In] ###
@app.route("/LogIn", methods=["POST"])
def Validate():

    try:
        # Pull user data from form
        username=request.form['username']
        password=request.form['password']

		# Sanitize Input
        check = InputSanitization(username)
        if check == 0:
        	return render_template('error.html',error = 'An error occurred!')
        check = InputSanitization(password)
        if check == 0:
        	return render_template('error.html',error = 'An error occurred!')

        # Connect to MySQL DB ImageUpload and run LogIn procedure
        cursor = mysql.connection.cursor()
        cursor.callproc('procedure_LogIn',(username))
        data = cursor.fetchall()

        # Check if data returned -> implies valid username
        # Check if password is correct
        if len(data) > 0:
        	if str(data[0][4]) == sha256((salt + password).encode('utf-8')):
        		# If password correct give user a cryptographically signed session token
        		session['user'] = data[0][0]            # data[0][0] -> user_id
        		return redirect("/UserHomePage")
        	else:
        		# If password incorrect, alert user
        		return render_template('error.html',error = 'Invalid Log In.')
        else:
        	# If no data returned, alert user
        	return render_template('error.html',error = 'Invalid Log In.')

    # Handle unexpected errors gracefully
    except Exception as e:
    	return render_template('error.html',error = e)



### [Sign Up] ###
@app.route("/SignUp", methods=["POST", "GET"])
def SignUp():

    # Pull user data from form
    firstname=request.form['requested_firstname']
    lastname=request.form['requested_lastname']
    email=request.form['requested_email']
    username=request.form['requested_username']
    password1=request.form['requested_password1']

	# Sanitize Input
    check = InputSanitization(firstname)
    if check == 0:
        return redirect("/")
    check = InputSanitization(lastname)
    if check == 0:
        return json.dumps({'error': 'Invalid Characters'})
    check = InputSanitization(email)
    if check == 0:
        return json.dumps({'error': 'Invalid Characters'})
    check = InputSanitization(username)
    print(check)
    if check == 0:
        return json.dumps({'error': 'Invalid Characters'})
    check = InputSanitization(password1)
    if check == 0:
    	return json.dumps({'error': 'Invalid Characters'})


    # Connect to MySQL DB ImageUpload and run SignUp procedure
    cursor = mysql.connection.cursor()
    cursor.callproc('procedure_SignUp',
                    (firstname+' '+lastname,
                     email,
                     username,
                     sha256((salt + password1).encode('utf-8'))))
    data = cursor.fetchall()

    # Check if SignUp procedure succeeded -> sends no message if success i.e. len(data) = 0
    #                                     -> sends a message if fail
    if len(data) is 0:
    	# If SignUp procedure succeeds, commit changes
    	mysql.connection.commit()
    	cursor.close()
    	return json.dumps({'message':'User created successfully !'})
    else:
    	# if SignUp procedure fails, alert user
    	return json.dumps({'error':str(data[0])})





#$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$#
''' Restricted Access Pages '''
#$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$#



### [User Home Page] ###
@app.route("/UserHomePage")
def showUserHome():
	# Check if user has proper credentials -> 'user' session token
	if session.get('user'):
		# If user has credentials, redirect to their home page
		return render_template('user.html')
	else:
		# If user does not have proper credentials, alert user
		return render_template('error.html',error = 'Unauthorized Access')

### [Edit Page] ###
@app.route("/EditPage")
def showEdit():
	# Check if user has proper credentials -> 'user' session token
	if session.get('user'):
		# If user has credentials, redirect to their home page
		return render_template('edit.html')
	else:
		# If user does not have proper credentials, alert user
		return render_template('error.html',error = 'Unauthorized Access')



#$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$#
''' End Resdtricted Access Pages '''
#$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$#





#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#
#             PUBLIC PAGES            #
#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#

### [Upload Page] ###
@app.route("/UploadPage")
def showUpload():
	return render_template('upload.html')

### [Log In Page] ###
@app.route("/LogInPage")
def showLogIn():
	return render_template('login.html')

### [Sign Up Page] ###
@app.route("/SignUpPage")
def showSignUp():
	return render_template('signup.html')

### [Welcome Page] ###
@app.route("/")
def main():
	return render_template('home.html')

#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#
#          END PUBLIC PAGES           #
#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#



### [Run] ###
if __name__ == "__main__":
	app.run()
