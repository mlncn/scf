$Id: README.txt,v 1.1.2.4 2009/01/04 12:14:39 fago Exp $

Content Profile Module
------------------------
by Wolfgang Ziegler, nuppla@zites.net

With this module you can build user profiles with drupal's content types.


Installation 
------------
 * Copy the module's directory to your modules directory and activate the module.
 
 Usage:
--------
 * There will be a new content type "profile". Customize its settings at
   admin/content/types.
 * At the bottom of each content type edit form, there is a checkbox, which allows
   you to mark a content type as profile.
 * When you edit a profile content type there will be a further tab "Content profile",
   which provides content profile specific settings.


Content profiles per role:
--------------------------
You may, but you need not, mark multiple content types as profile. By customizing 
the permissions of a content type, this allows you to create different profiles for
different roles.


Hints:
------

 * When using content profiles the "title" field is sometimes annoying. You can rename
   it at the content types settings or hide it in the form and auto generate a title by
   using the auto nodetitle module http://drupal.org/project/auto_nodetitle.
   
 * If you want to link to a content profile of a user, you can always link to the path
   "user/UID/profile/TYPE" where UID is the users id and TYPE the machine readable content
   type name, an example path would be "user/1/profile/profile".
   This path is working regardless the user has already profile content created or not.

 * If you want to theme your content profile, you can do it like with any other content.
   Read http://drupal.org/node/266817.
   
 * If you want a content profile to be private while your site content should be available
   to the public, you need a module that allows configuring more fine grained access control
   permissions, e.g. the module Content Access (http://drupal.org/project/content_access)
   allows you to that.

