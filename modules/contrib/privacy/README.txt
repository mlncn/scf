// $Id: README.txt,v 1.1 2009/01/28 23:38:34 cptncauliflower Exp $

DESCRIPTION
-----------

Define which cck fields should be able to keep private.


INSTALL
-------

Follow these steps:
1. Untar the tarball into your module directory (sites/all/modules)
2. Enable the module.  CCK must also be enabled.
3. Create your content types and additional fields with the cck module
5. Go to admin/content/privacy and enable the fields which should be able to
keep private (only users with 'administer privacy' rights are able to do this)
6. When a user creates a node, he can check if a field should be private or
not.

Definition of 'private':
- Only the author of the node or the users with 'administer privacy' or 'view
  all privacy' rights are able to see hidden fields.

CREDITS
-------

This module is developed by the webteam of jeugdwerknet.be (Pieter, Wouter and
Wim) and is based on http://drupal.org/project/cck_field_privacy and
http://drupal.org/project/profile_privacy (the difference is that this module
is not dependent on the profile, friendlist and jquery_impromptu module)
