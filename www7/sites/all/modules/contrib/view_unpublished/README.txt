View Unpublished
----------------
This small module adds the missing permissions "view any unpublished content"
and "view unpublished $content_type content" to Drupal 7.

This module also integrates with the core Content overview screen at /admin/content.
If you choose the "not published" filter, Drupal will show you unpublished
content you're allowed to see.

Using view_unpublished with Views
---------------------------------
Use the "Published or admin" filter, NOT "published = yes". Views will then
respect your custom permissions. Thanks to hanoii (6.x) and pcambra (7.x) for
this feature.

Common issues
-------------
If for some reason this module seems not to work, try rebuilding your node
permissions: admin/reports/status/rebuild. Note that this can take significant
time on larger installs and it is HIGHLY recommended that you back up your site
first.

Releated isses on drupal.org
----------------------------
[New node permission "view any unpublished content"](http://drupal.org/node/273595)
[Enable Node Grants for Unpublished Nodes](http://drupal.org/node/452538)


Thanks to:
----------
Florian Weber (webflo) and thekevinday for their work on the D7 port.