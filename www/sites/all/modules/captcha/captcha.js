// $Id: captcha.js,v 1.1 2009/06/18 10:04:16 soxofaan Exp $

$(document).ready(function(){

	// Add onclick handler to checkbox for adding a CAPTCHA description
	// so that the textfields for the CAPTCHA description are hidden
	// when no description should be added.
	$("#edit-captcha-add-captcha-description").click(function() {
		if ($("#edit-captcha-add-captcha-description").is(":checked")) {
			// Show the CAPTCHA description textfield(s).
			$("#edit-captcha-description-wrapper").show("slow");
		}
		else {
			// Hide the CAPTCHA description textfield(s).
			$("#edit-captcha-description-wrapper").hide("slow");
		}
	});
	// Hide the CAPTCHA description textfields if option is disabled on page load.
	if (!$("#edit-captcha-add-captcha-description").is(":checked")) {
		$("#edit-captcha-description-wrapper").hide();
	}

});
