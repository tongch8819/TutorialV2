<!DOCTYPE html>
<html>
<head>
<title>loop statements</title>
</head>
<body>
<?php
    $Count = 1;
    while ($Count <= 5) {
            echo " $Count<br /> ";
            ++$Count;
    }
    echo " <p>You have printed 5 numbers.</p> ";

    $Count = 10;
    while ($Count > 0) {
        echo " $Count<br /> ";
        --$Count;
    }
    echo " <p>We have liftoff. </p> ";

    $Count = 1;
    while ($Count <= 100) {
            echo " $Count<br /> ";
            $Count *= 2;
    }

/*
    $Count = 1;
    while ($Count <= 10) {
            echo " The number is $Count ";
    }
*/
    
    
?>
</body>
</html>
