With the Views PDF module you can output a view as a PDF document. Each field of
the view can be placed on the PDF page directly in the administration interface. 
Therefore a new display called "PDF" is added.

There are already some PDF solutions such as Print. But these solutions use the 
HTML output and converts this to PDF. The disadvantages of such an integration 
are:
    * No control over page flow (e.g. page break).
    * Little or no control over page header and footer.
    * You need HTML skills to change the layout.
    * The rendering is slow and need a lot of memory, because it need to render 
      the HTML.
    * Complex tables make troubles.
    * Vector graphics can not be implemented, therefore the printing of the doc-
      ument can be problematic.
    * You are limited by HTML's capabilities.



Installation instructions:
--------------------------
   1. Download the module or checkout the module.
   2. Upload the module to your Drupal instance.
   3. Download the required libraries. Download TCPDF and FPDI. Copy the files 
      to the lib directory in the module directory. The path must be so: 
      "sites/all/libraries/tcpdf/tcpdf.php" respectively "sites/all/libraries/fpdi/fpdi.php".
      If you are using the Libraries API then put them into the libraries folder.
   4. Check under reports if you setup everything correct.
   5. Setup a view with a PDF display.
   6. Use it.

Basic Usage
-----------
   1. Setup a new view or use an existing view. How to do this see in the 
      documentation of the Views module.
   2. In the view add the display "PDF Page".
   3. Select the new added display.
   4. Select the Style. You can use PDF unformatted to place the fields in no 
      structured way on the PDF. Use PDF table to place the fields in table with 
      a table header.
   5. In the settings of the style you can setup per field settings. Such as the
      position, the size of the field, the font and so on. Important: Switch to 
      the PDF Page display in the default display this options does not appear.
   6. Under PDF Page Setting you can setup the size of the page.
   7. Under PDF Font Settings you can setup the default fonts for the PDF.
   8. Under PDF Template Settings you can setup a background PDF.
   9. To add a page break, you can add a PDF page break field. When this field 
      is rendered new page is added. Reorder the fields if necessary.
  10. You can find also a page number field. You can use it to print the current
      page number. Important for positioning the field in the header or footer, 
      you need to set the relative position in the field settings to 
      "In header / footer".

