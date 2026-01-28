<!DOCTYPE html>
<html>
<head>
<title>PHP XML</title>
</head>
<body>
<?php
$xml = simplexml_load_file('contacts.xml');

echo "<pre>\n";
print_r($xml);
echo "</pre>\n";

echo "<ol>\n";

foreach ($xml->contact as $c) {
    // print contact’s name, id, email.
    //The attribute will be accesible as if it were an assoc array entry. 
	echo '<li>' . $c->name . " - " . $c['idx'] . ", email:" . $c->email;
	// start an unordered list for contact’s phone numbers
	echo '<ul>';
	
	// loop over every phone number for this person
	foreach ($c->phone as $p) {
		echo '<li>', ucfirst($p['type']), ': ', $p, '</li>';
		// echo out a line including the type of number
        //The attribute will be accesible as if it were an assoc array entry. Using the entry itself, will echo it's value.
	}
	echo "</ul></li>\n"; // close the phone list
    echo 'meta: ' . $c->meta['id'] ;
}
?>
</body>
</html>
