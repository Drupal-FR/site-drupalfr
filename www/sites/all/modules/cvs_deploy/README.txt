The CVS deploy module makes Drupal work better for sites that checkout
Drupal source directly from CVS.  If you run "cvs checkout ..." to
setup the directory where your Drupal installation lives, this module
is for you.

The first improvement is that at the module administration page
(administer >> build >> modules or 'admin/build/modules'), the version
column will be filled in with human-readable strings.  If there is no
version information in the .info files, this module will look for the
sticky tag in the CVS directory for each module, and attempt to
resolve the version string based on the tag or branch you have checked
out from. If you happen to have an older checkout of a module that
used 'version = "$Name ..." in the .info file, CVS deploy will resolve
that into a human-readable version string, too.

The other major improvement is the interaction with the Update status
module included in Drupal 6.x core.  If you deploy your site from CVS,
you can still use Update status to warn you about newer versions, so
long as you have the CVS deploy module enabled.

Send feature requests and bug reports to the issue queue for the
CVS deploy module: http://drupal.org/node/add/project_issue/cvs_deploy

Written by: Derek Wright ("dww") http://drupal.org/user/46549

$Id: README.txt,v 1.2 2008/02/16 17:22:43 dww Exp $
