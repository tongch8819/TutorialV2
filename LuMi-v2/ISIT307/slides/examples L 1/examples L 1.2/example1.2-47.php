<!DOCTYPE html>
<html>
<head>
<title>for statements</title>
</head>
<body>
<?php

$DaysOfWeek = array("Monday", "Tuesday","Wednesday", "Thursday", "Friday","Saturday", "Sunday");

foreach ($DaysOfWeek as $Day) {
     echo "<p>$Day</p>";
}
echo "------------<br />";

foreach ($DaysOfWeek as $DayNum => $Day) {
     echo "<p>$DayNum is $Day</p>";
}

echo "------------<br />";

    $FastFoods = array("pizza", "burgers", "french fries","tacos", "fried chicken");
    for ($Count = 0; $Count < 5; ++$Count)
        echo $FastFoods[$Count], "<br />";

    echo "------------<br />";
    
    foreach ($FastFoods as $ff)
        echo $ff, "<br />";
    
    
    echo "------------<br />";
    
    foreach ($FastFoods as $ind => $ff) {
        echo "$ind -> $ff, <br />";
    }


?>
</body>
</html>
