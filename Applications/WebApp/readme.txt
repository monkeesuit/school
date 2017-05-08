To get the website running just navigate to the directory containing app.py and run the command:
“python3 app.py”

This should start the web app running on local host (127.0.0.1) port# 5000.

When navigating to http://127.0.0.1:5000/ you will first be taken to the home page which displays all users uploads.

Any one can upload pictures by going to the upload page, filling out the form and submitting it.
If you upload a picture while not signed in the picture will be considered public.
If you are signed in and upload a picture you will be the owner of that picture.

If you are an owner of a picture you can delete or edit it by going to the edit page, clicking on the name of your file, and either filling out the form and pressing the edit button in order to edit the caption, or just press the delete button to delete the picture.

To become a registered user you can go to the signup page and register.
To sign in you must navigate to to the login page.
Upon successful login you will be given a session token to validate your actions.
Signed in users have access to their own home page which only displays their images.
Pressing the logout button will result in the session token being deleted.

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

mySql is used as a backend database, most of the tables and procedures source files can be found in the sql folder as well as a general description of the schema.

the static folder contains folders for stylesheets (css folder), uploaded images (images folder) and the javascript (js folder).

* the javascript is not well commented yet but here is a general overview:
	[+] buttonSignUp.js simply posts the signup info to the webservers /SignUp 			function
	[+] displayImage.js is no longer used
	[+] edit.js posts the name of an upload to the webservers /GetPhotoByName 
		function and uses the response to generate a form that allows the user 		to edit or delete the upload
	[+] getAllPhotos.js generate the images and paging found on the home page
	[+] getEditPhotos.js generate the selectable items and paging found on the 
		edit page
	[+] getUserPhotos.js generate the images and paging found on the users home 
		page 
	[+] upload.js just provides a little user feedback when the user starts 			uploading a file

the templates folder contains all the html used.

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
