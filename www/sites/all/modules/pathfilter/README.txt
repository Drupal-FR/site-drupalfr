$Id: README.txt,v 1.9 2006/12/24 23:49:21 robroy Exp $
-----------------------
  OVERVIEW
-----------------------
Path Filter takes internal drupal paths in double quotes, written as
e.g. "internal:node/99", and replaces them with the appropriate absolute
or relative URL using drupal's url() function [1].

Suppose your site is located at 'http://example.com/mysite', clean URLs
are enabled, and 'node/99' has a URL alias of 'news/latest'. The following
are some examples of the replacements performed by this filter.

  "internal:admin/user"  ->  "http://example.com/mysite/admin/user"
  "internal:node/23"     ->  "http://example.com/mysite/node/23"
  "internal:node/99"     ->  "http://example.com/mysite/news/latest"

It even handles things like ...

  "internal:node/23?page=1#section2" ->
                    "http://example.com/mysite/node/23?page=1#section2"

It works with clean URLs enabled or disabled.

The motivation for this filter was to provide a robust way of linking to
internal URLs from within content, so that your links do not break if you
move your site to a different path (e.g. from a development site at
http://example.com/dev/ to a production site at http://example.com/).

WARNING #1: In order to avoid broken links, you should clear your cache [2]
if you change an alias for any pathfilter links.

-----------------------
  INSTALLATION
-----------------------
1. Drop it into your modules folder and turn it on.

2. Then enable it for any input formats you wish.

   WARNING #2: Path Filter must run before Drupal's HTML filter so make sure to
   set the weight accordingly!

3. Once enabled, you may configure Path Filter to convert internal paths to
   absolute URLs (default) or relative paths on the input format filter
   configuration page.

-----------------------
  USE WITH TINYMCE [3]
-----------------------
In its default configuration, TinyMCE will treat a link to "internal:foo"
as a normal relative URL and try to prefix it with the appropriate base URL,
messing it up in the process. This can be prevented by including the following
at the end of a custom TinyMCE theme function as described in TinyMCE's
INSTALL.txt.

  if (isset($init)) {
    $init['convert_urls'] = 'false';
  }

If you are not already using a custom theme for TinyMCE, adding the following
function to your template.php file should be sufficient.

function phptemplate_tinymce_theme($init, $textarea_name, $theme_name, $is_running) {
  $init = theme_tinymce_theme($init, $textarea_name, $theme_name, $is_running);
  
  // Disable conversion of relative URLs so we can use pathfilter.module
  // and it's "internal:node/99" style paths.
  if (isset($init)) {
    $init['convert_urls'] = 'false';
  }

  return $init;
}

-----------------------
  TODO
-----------------------
- Write handbook page.

[1] http://api.drupal.org/api/4.7/function/url
[2] The "empty cache" link provided by the Devel module
    (http://drupal.org/project/devel) is a convenient way to accomplish this.
[3] http://drupal.org/project/tinymce
