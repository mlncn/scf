********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: user import module
Author: Robert Castelo <www.cortextcommunications.com>
Drupal: 4.7.x
********************************************************************
DESCRIPTION:

Import users into Drupal from a csv file (comma separated file).

Features include:

* Creates an account for each user.
* Match csv columns to profile fields.  
* Can optionally use the file's first row to map csv data to user profile fields.
* Option to create Usernames based on data from file, e.g. "John" + "Smith" => "JohnSmith".
* Usernames can be made of abbreviated data from file, e.g. "Jane" + "Doe" => "JDoe".
* Option to create random, human readable, Usernames.
* Option to import passwords
* Option to create random passwords for each user.
* Can set user roles.
* Option to send welcome email, with account details to each new user.
* Can set each user's contact form to enabled
* Test mode option to check for errors.
* Processing can be triggered by cron or manually by an administrator.
* Can stagger number of users imported, so that not too many emails are sent at one time.
* Multiple files can be imported/tested at the same time.
* Import into Organic Groups
* Option to make new accounts immediately active, or inactive until user logs in
* Use CSV file already uploaded through FTP (useful for large imports)
* Designed to be massively scalable.

Supported CSV File Formats:
Make sure csv file has been saved with 'Windows' line endings.
If file import fails with "File copy failed: source file does not exist." try
setting the file extension to .txt.


********************************************************************
PREREQUISITES:

  Must have customized Profile fields already entered 
  if data is to be imported into user profiles.


********************************************************************
INSTALLATION:

Note: It is assumed that you have Drupal up and running.  Be sure to
check the Drupal web site if you need assistance.

1. Place the entire user_import directory into your Drupal modules/
   directory.
   

2. Enable the user_import modules by navigating to:

     administer > modules
     
  Click the 'Save configuration' button at the bottom to commit your
  changes.
  
  
********************************************************************
USAGE

1. To set permissions of who can import users into the site, navigate to:

'administer' 
    -- 'access control' (admin/access)
    

2. To import users, navigate to:

'administer'
    -- 'settings'
        -- 'user imports'  (admin/settings/user_import)
        
3. Select 'Import' tab.

4. Press the 'browse' button to select a file to import,
    or select a file already added through FTP.

5. Click on Next.

6. Under CSV file you should see the name of the file you just uploaded.

7. Under Options you should see Ignore First Line ( use if the first row are labels ), 
    
    Contact, and Send Email.  Select whichever is appropiate.

8. Under Field Match you should see the various columns from your profile page.

9. For each csv column select a Drupal field to map. 

10. Under username select 'No', if the field is not to be used to generate the username, or select '1' - '4' 
    for the order to use the field in generating username.

    Example: 'LastName' and 'FirstName' are fields to be used as username.  So under the username
    selection chose '1' for 'FirstName' and '2' for 'Lastname', and the username generated will be in 
    the form 'FirstNameLastName'.

11. Under Role Assign select the roles the imported users will be assigned.

12. Under Save Settings, you can save your settings for use on future imports.

13. Click "Test" to do an import without committing changes to the database.  Fix any errors that are generated.

14. Click "Import" to complete the import.


********************************************************************
AUTHOR CONTACT

- Report Bugs/Request Features:
   http://drupal.org/project/user_import
   
- Comission New Features:
   http://drupal.org/user/3555/contact
   
- Want To Say Thank You:
   http://www.amazon.com/gp/registry/O6JKRQEQ774F

        
********************************************************************
ACKNOWLEDGEMENT

- Initial starting point for this module was a script by David McIntosh (neofactor.com).


- Documentation help Steve (spatz4000)
