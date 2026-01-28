<!DOCTYPE html>
<html>
<head>
<title>PHP XML</title>
</head>
<body>
<?php
$xml = simplexml_load_file('contacts.xml'); // using SimpleXML, read the file into memory

$meta = $xml->xpath('//meta'); // Xpath search to find all 'meta' tags no matter what the depth

foreach ($meta as $m) {
	echo "Meta - {$m['id']}<br />\n";
}

$email = $xml->xpath('/contacts/contact/email'); // find all email tags within a contact tag from the root of the XML document

foreach ($email as $e) {
	echo "Email - {$e}<br />\n";
}

$cell = $xml->xpath('contact/phone[@type="cell"]/..'); // find any contact who has a cellphone number

foreach ($cell as $c) {
	echo "Cell Contact - {$c->name}<br />\n";
}
?>
