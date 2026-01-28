<!DOCTYPE html>
<html>
<head>
<title>Web</title>
</head>
<body>
<?php 
/* $Dir = ".";
$DirEntries = scandir($Dir);
foreach ($DirEntries as $Entry) {
	echo $Entry . "<br />\n";
} */

$Dir = ".";//$Dir = "name_of_dir";
$DirEntries = scandir($Dir);
foreach ($DirEntries as $Entry) {
	if ((strcmp($Entry, '.') != 0) && (strcmp($Entry, '..') != 0))
		
		echo "<a href=\"./" . $Entry . "\">" . $Entry . "</a><br />\n";
    
        //echo "<a href=\"name_of_dir/" . $Entry . "\">" . $Entry . "</a><br />\n";
} 
?>
</body>
</html>

