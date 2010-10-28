$Id: README.txt,v 1.2.2.7 2008/09/15 16:45:15 heine Exp $

Comment upload provides the ability to upload "file attachments" to comments.

Compared to Comment upload 5.x, the 6.x version still lacks views integration.

Comment upload requires at least Drupal 6.4.

INSTALLATION
============
- Copy the comment_upload folder from the archive to sites/all/modules.

- Visit Administer >> Site building >> Modules (admin/build/modules) and enable
  the comment upload module (under Other).

- Configure permission on Administer >> User management >> Permissions 
  (admin/user/permissions)

- Enable attachments for specific content types on the edit pages of specific
  content types reached via the edit links on Administer >> Content management >> 
  Content types (admin/content/node-type).


CONFIGURATION
=============

- You can enable uploads per content type on the edit pages of specific content
  types. Use the edit links on Administer >> Content management >> Content types 
  (admin/content/node-type) to access these pages.

- Display of images is governed by the Image attachments on comments setting.
  
  - Display as attachments will include images in the attachments table.
  - Display as full image will add the entire image below the comment.
  
  If you install imagecache (http://drupal.org/project/imagecache) you will see
  additional 'Display via imagecache preset [presetname] options for any 
  preset you have created.
  
  This is useful to ensure your page is not distorted by overly large images.

- Comment upload provides two permissions:

  - "upload files to comments"
  - "view files uploaded to comments"

- Allowed extensions and the maximum allowed upload sizes are taken from the 
  Upload module settings on Administer >> Site configuration >> File uploads 
  (admin/settings/uploads).


THEMING
=======
To override the display of attachments, copy the file
"comment-upload-attachments.tpl.php" to your PHPtemplate theme to make changes.

Variables:

$images      - an array of images (when not set to Display as attachments).

with each element containing:
  'url'      - url of the full image
  'image'    - <img> tag

$attachments - an array of attachments

with each element containing:
  'url'      -  url of the file
  'zebra'    - 'odd' or 'even' to enable zebra striping
  'text'     - description when set or filename of the file
  'size'     - size of the file in bytes, kB or Mb.


NOTE
====
As comment_upload heavily modifies the way comment form submission works, there
is a chance that it is not compatible with other modules changing the comment form.


AUTHORS & COPYRIGHT
===================
Comment upload 4.7.x and 5.x were written by Károly Négyesi and Jeff Eaton.
Comment upload 6.x was written by Heine Deelstra.

Comment upload 6.x Copyright (c) 2008 Ustima Web Development.

This module is licensed under the  GPL v2. See LICENSE.txt for details.