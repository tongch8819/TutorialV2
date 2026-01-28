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
    <name>Tom White</name>
    <category>Family</category>
    <phone type='home'>301-555-1212</phone>
    <meta id='x634724' />
</contact>";

if (!($xml=simplexml_load_string($myXMLData)))
	die("Error: Cannot create object");
echo "<pre>\n";
print_r($xml);
echo "</pre>\n";

?>

</body>
</html>
