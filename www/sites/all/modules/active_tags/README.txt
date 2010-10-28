$Id: README.txt,v 1.1 2008/09/12 08:31:03 darrenmuk Exp $

-- SUMMARY -- 

Active Tags adds a new option to free tagging taxonomies. If selected
the taxonomy widget is replaced by a new jQuery enabled tag entry widget.

For a full description visit the project page:
  http://drupal.org/project/active_tags
Bug reports, feature suggestions and latest developments:
  http://drupal.org/project/issues/active_tags


-- REQUIREMENTS --

A site configured with at least one vocabulary.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Edit the vocabulary for a free tagging taxonomy to enable active tags.

Administer >> Content Management >> Taxonomy >> edit vocabulary

* If you do not have a vocabulary, create one and check the active tags
option.


-- CUSTOMIZATION --

* You can change the appearance of the tag display by overriding the active_tags.css
file in your theme. To do this enter the following in your theme.info file:

  stylesheets[all][] = active_tags.css

then create a copy of active_tags.css within your theme folder.


-- CONTACT --

Web:
* http://drupal.org/project/active_tags
* http://www.darrenmothersele.com/node/47

Current maintainer:
* Darren Mothersele - darren@darrenmothersele.com


