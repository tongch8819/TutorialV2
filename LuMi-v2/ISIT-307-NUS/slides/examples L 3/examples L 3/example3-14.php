<!DOCTYPE html>
<html>
<head>
<title>Web</title>
</head>
<body>
<?php 
    $Dir = "."; 
    $DirOpen = opendir($Dir);
    while ($CurFile = readdir($DirOpen)) {
        echo $CurFile . "<br />\n";
    }
    closedir($DirOpen);
 ?>
</body>
</html>

