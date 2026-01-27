<!DOCTYPE html>
<html>
<head>
<title>Create</title>
</head>
<body>
<?php
	$num=6;

if($num>5){
    if($num<10)
        echo "<p> the number is between 5 and 10 </p>";
    }
	
	if($num>5 && $num<10)
		echo "<p> the number is between 5 and 10 </p>";
	else
		echo "<p> the number is not between 5 and 10 </p>";
    
	
    $num=9;
	if($num==5)
		echo "<p> the number is  5 </p>";
	elseif ($num<10) 
		echo "<p> the number is less than 10 </p>";
	else
		echo "<p> the number is out of range </p>";

    $a = 6;
    $b = 6;
    if (($a<=>$b) == -1)
        echo "<p> a < b </p>";
    elseif (($a<=>$b) == 1)
        echo "<p> a > b </p>";
    else
        echo "<p> a = b </p>";


    $page = "Login";
    
    if ($page == "Home") echo "You selected Home";
    elseif ($page == "About") echo "You selected About";
    elseif ($page == "News") echo "You selected News";
    elseif ($page == "Login") echo "You selected Login";
    elseif ($page == "Links") echo "You selected Links";
    else echo "Unrecognized selection";

echo"<br />";
// another option
    switch ($page) {
        case "Home":
            echo "You selected Home";
            break;
        case "About":
            echo "You selected About";
            break;
        case "News":
            echo "You selected News";
            break;
        case "Login":
            echo "You selected Login";
            break;
        case "Links":
            echo "You selected Links";
            break;
        default:
            echo "Unrecognized selection";
            break;
    }

?>
</body>
</html>
