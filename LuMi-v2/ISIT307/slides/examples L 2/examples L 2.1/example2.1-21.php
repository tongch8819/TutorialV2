<!DOCTYPE html>
<html>
<head>
<title>examples</title>
</head>
<body>
<?php
    $ExampleString = "woodworking project";
    echo $ExampleString . "<br />\n";

    echo substr($ExampleString,4) . "<br />\n";
    echo substr($ExampleString,4,7) . "<br />\n";
    echo substr($ExampleString,0,8) . "<br />\n";
    echo substr($ExampleString,-7) . "<br />\n";
    echo substr($ExampleString,-12,4) . "<br />\n";
    $subString = substr($ExampleString, 5, -2);
    echo $subString . "<br />\n";

    echo substr($ExampleString,-5,-2) . "<br />\n";
        
    echo strrev($ExampleString) . "<br />\n";
    echo str_shuffle($ExampleString) . "<br />\n";
?>
</body>
</html>

