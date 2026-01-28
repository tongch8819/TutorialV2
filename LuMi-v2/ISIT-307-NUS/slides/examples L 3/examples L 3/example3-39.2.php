<!DOCTYPE html>
<html>
<head>
<title>Volunteers</title>
</head>
<body>
<h2>Volunteers</h2>
<hr />
<?php
	$FileName = "./volunteers.txt";
    
    $Volunt = file_get_contents($FileName);
    
    echo $Volunt;
    
    echo "<hr />\n";
    
    
    readfile($FileName);
    
    echo "<hr />\n";
    
    
    $content = file($FileName);
    for($i=0; $i<count($content); $i++){
        echo $i+1,".$content[$i]<br />";
    }
    
    echo "<hr />\n";
    
?>

</body>
</html>

