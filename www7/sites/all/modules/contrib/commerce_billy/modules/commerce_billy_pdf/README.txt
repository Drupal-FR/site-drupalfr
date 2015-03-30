Commerce Billy PDF
******************
Enables PDF donwload of invoiced and canceled orders.

Installation
************

1. Dowload dompdf lib in place it into the libraries folder, e.g. sites/all/libraries/dompdf

Possible drush make file:
libraries[dompdf][download][type] = "get"
libraries[dompdf][download][url] = "http://dompdf.googlecode.com/files/dompdf_0-6-0_beta3.tar.gz"
libraries[dompdf][directory_name] = "dompdf"
libraries[dompdf][destination] = "libraries"


2. Add dompdf fonts to public dir:

dompdf needs write access to its font directory.
Copy "libraries/dompdf/lib/fonts" to your public files directory:
  public://fonts (example: sites/default/files/fonts)

To check if everything is at the right place, you should find
  public://fonts/Courier.afm (for example at
  sites/default/files/fonts/Courier.afm).


Administration
**************
Got to: Store > Configuration > Billy invoice settings > PDF settings
        (/admin/commerce/config/billy-invoice/pdf)


Troubles
********
- Invoice links returns "Error generating PDF invoice. Please contact the
  website administrator." :
    Check your watchdog which contains information about the exception. Usually
    fonts are not correctly installed.

Credits
*******
Matthias Hutterer (mh86)
Klaus Purer (klausi)
