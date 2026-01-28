<!DOCTYPE html>
<html>
<head>
<title>PHP </title>
</head>
<body>
<?php

function sum($one,...$numbers) {
    $acc = 0;
	echo "<br /> $one and then numbers: <br />";
    foreach ($numbers as $n) {
        echo $n, "<br />";
        $acc += $n; //$acc = $acc + $n;
    }
    return $acc;
}
function sum1(...$numbers) {
    $acc = 0;
    echo "<br /> numbers: <br />";
    foreach ($numbers as $n) {
        echo $n, "<br />";
        $acc += $n; //$acc = $acc + $n;
    }
    return $acc;
}

echo "<br> function sum:";
$sum = sum("first");
echo "sum = $sum <br />";

$sum = sum("second",1, 2, 3);
echo "sum = $sum <br />";

$sum = sum("third",10, 20);
echo "sum = $sum <br />";


echo "<br> function sum1:";
$sum = sum1();
echo "sum = $sum <br />";

$sum = sum1(100);
echo "sum = $sum <br />";

$sum = sum1(100, 200);
echo "sum = $sum <br />";
?>
