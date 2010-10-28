$Id: README.txt,v 1.1 2005/11/17 02:56:38 vauxia Exp $

INSTALLATION
	Hopefully, you know the drill by now :)
	1. Place the entire mimemail directory into your Drupal modules 
	   directory.
	2. Enable the mimemail module by navigating to:
     administer > modules
    
USAGE
  This module may be required by other modules, but is not terribly
  useful by itself. Once installed, any module can send messages by 
  calling the mimemail() function:
	
	$sender     - a user object or email address
	$recipient  - a user object or email address
	$subject    - subject line
	$body       - body text in html format
	$plaintext  - boolean, whether to send messages in plaintext-only
	            (default false)

  This module creates a user preference for receiving plaintext-only
  messages.  This preference will be honored by all calls to mimemail()
  
  Messages are formatting using theme_mimemail_message($body), which
  includes all css files from the currently active theme and compresses
  the HTML version of the text.  Create a new theme function if your 
  web stylesheets are too heavy for this.
  
CREDITS

	MAINTAINER: Allie Micka < allie at pajunas dot com > 
	
	* Allie Micka
	  Mime enhancements and HTML mail code
	  
	* Gerhard Killesreiter
	  Original mail and mime code
	  
  * Robert Castelo
    	HTML to Text and other functionality 