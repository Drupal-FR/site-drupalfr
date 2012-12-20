Drupal commentrss.module README.txt
==============================================================================

This module provides RSS feeds for comments. This is useful for allowing
readers to subscribe to comments for a blog post, article, or forum topic.
It currently provides three types of feeds. Each type of comment feed
may be disabled if unneeded.

  * complete site feed        /crss
  * per node feeds        eg. /crss/node/12
  * per term feeds        eg. /crss/term/13

Comment feeds provide an alternative to email subscriptions, allowing users to
monitor discussions without having to provide their email address. Due to the
limited capabilities of RSS, threading is not preserved and the comments are
listed in reversed time order.


Installation
------------------------------------------------------------------------------
 
 Required:
  - Copy the module files to sites/all/modules/
  - Enable the module as usual from Drupal's admin pages.
  
 Optional:
  - Configure the module on "Administer" >> "Content management" >>
    "RSS publishing" >> "Comment RSS settings" if the defaults are not
    fitting for you.
 

Credits / Contact
------------------------------------------------------------------------------

This module was created and is currently maintainer by Gabor Hojtsy
(http://drupal.org/user/4166). Moshe Weitzman provided several fixes
and improvements, and Chris Cook maintained it for quite some time as well.
