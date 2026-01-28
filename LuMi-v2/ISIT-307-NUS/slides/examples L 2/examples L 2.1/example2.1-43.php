<!DOCTYPE html>
<html>
<head>
<title>examples</title>
</head>
<body>
<?php
$my_str = "Bob is working";
echo "<p>$my_str</p>";

$my_str[0] = "R";
echo "<p>$my_str</p>";

echo "<p> try this with negative -5: $my_str[-5]</p>";

for($i=0; $i< strlen($my_str); $i++)
    echo "<p> * $my_str[$i]</p>";

echo "<p>-----------------</p>";

$Presidents = "George Washington; John Adams; Thomas Jefferson;James Madison;James Monroe";
$PresidentArray = explode("; ", $Presidents);
foreach ($PresidentArray as $President) {
        echo "$President<br />";
}

?>



</body>
</html>

