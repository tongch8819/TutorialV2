<?php
    setcookie("pr[0]", "Elena", time()+3600);
    setcookie("pr[1]", "Vlahu", time()+3600);
    setcookie("pr[2]", "lecturer", time()+3600);
    
    setcookie("pr1[firstName]", "John");
    setcookie("pr1[lastName]", "Doe");
    setcookie("pr1[occupation]", "lecturer");

    ?>
<!DOCTYPE html>
<html>
<head>
<title>Cookies</title>
</head>
<body>
<h1>cookies</h1>
<?php
   if (isset($_COOKIE['pr']))
    echo "{$_COOKIE['pr'][0]} {$_COOKIE['pr'][1]} is a {$_COOKIE['pr'][2]}. <br />";
    if (isset($_COOKIE['pr1']))
    echo "{$_COOKIE['pr1']['firstName']} {$_COOKIE['pr1']['lastName']} is a {$_COOKIE['pr1']['occupation']}. <br />";
    
    if (isset($_COOKIE['pr1'])) {
        foreach ($_COOKIE['pr1'] as $keys => $value) {
            echo $keys. " : " . $value ." <br />\n";
        }
}
    
?>
</body>
</html>
