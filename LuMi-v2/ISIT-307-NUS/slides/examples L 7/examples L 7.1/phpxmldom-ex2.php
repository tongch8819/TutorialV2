<!DOCTYPE html>
<html>
<head>
<title>PHP XML</title>
</head>
<body>
<?php
$xmlDoc = new DOMDocument();
$xmlDoc->load("contacts.xml");

$xml = simplexml_import_dom($xmlDoc);
//echo "<p>First Contact's name: {$xml->contact[0]->name} </p>\n";
echo "<p>First Contact's name: {$xml->contact[0]->name}, {$xml->contact[0]['idx']}, {$xml->contact[0]->phone['type']}: {$xml->contact[0]->phone} </p>\n";
echo "<p>Second Contact's name: {$xml->contact[1]->name}</p>\n";
?>
