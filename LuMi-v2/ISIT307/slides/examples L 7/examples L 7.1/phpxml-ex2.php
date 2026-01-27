<!DOCTYPE html>
<html>
<head>
<title>PHP XML</title>
</head>
<body>
<?php
$myXMLData =
"<?xml version='1.0' ?> 
<contact idx='37'>
<name>Tom White</mname>
<category>Family</categorsys>
<phone type='home'>301-555-1212</phone>
<meta id='x634724' />
</contact>";

libxml_use_internal_errors(true);

$xml=simplexml_load_string($myXMLData);
if ($xml===false){
	echo "Failed loading XML: ";
	foreach(libxml_get_errors() as $error)
		echo "<br>". $error->message;
}else{
	echo "<pre>\n";
	print_r($xml);
	echo "</pre>\n";
}
?>
</body>
</html>
