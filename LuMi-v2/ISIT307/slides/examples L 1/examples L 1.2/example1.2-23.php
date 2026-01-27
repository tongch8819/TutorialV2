<!DOCTYPE html>
<html>
<head>
	<title>functions</title>
</head>
<body>
<h1>functions</h1>
<?php
    $GlobalVariable = "this is my value";

    function scopeExample() {
        global $GlobalVariable;
        echo "<p>$GlobalVariable</p>";
        $GlobalVariable = "if I change it";
        
    }
    
    scopeExample();
    echo "<p>$GlobalVariable</p>";
    //echo "<p>something to display</p>";

    $GlobalVariable = "new value";
    echo "<p>$GlobalVariable</p>";
?>
</body>
</html>
