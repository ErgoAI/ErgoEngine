
<?php
	$to = "flora-users@lists.sourceforge.net";
	if (mail($to, $subject, $body, ("From: " . $address))) {
	   printf("<br>Email has been sent. Thank you.");
	} else {
	   printf("<br>Sorry, an error occurred while sending email. Please try later.");
	}
?>
