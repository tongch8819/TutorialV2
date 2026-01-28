<!DOCTYPE html>
<html>
<head>
<title>Web template</title>
</head>
<body>
<?php 
$Dir = "."; //name_of_dir
$DirOpen = opendir($Dir);
while ($CurFile = readdir($DirOpen)) {
	if ((strcmp($CurFile, '.') != 0) && (strcmp($CurFile, '..') != 0))
		echo "<a href=\"./" . $CurFile ."\">" . $CurFile . "</a><br />\n";
        //echo "<a href=\"name_of_dir/" . $CurFile . "\">" . $CurFile . "</a><br />\n";
}
closedir($DirOpen);
 ?>
</body>
</html>

