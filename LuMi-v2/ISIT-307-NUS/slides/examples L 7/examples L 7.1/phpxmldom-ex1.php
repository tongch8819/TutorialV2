<!DOCTYPE html>
<html>
<head>
<title>PHP XML</title>
</head>
<body>
<?php
$xmlDoc = new DOMDocument();
$xmlDoc->load("list.xml");

echo $xmlDoc->saveXML();
    
echo "<br /> <br />";
    
$x = $xmlDoc->documentElement;
foreach ($x->childNodes AS $item) {
    echo $item->nodeName . " = " . $item->nodeValue . "<br />";
}
?>

</body>
</html>
