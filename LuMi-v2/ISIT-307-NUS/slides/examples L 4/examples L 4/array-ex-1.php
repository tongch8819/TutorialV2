<!DOCTYPE html>
<html>
<head>
<title>Arrays</title>
</head>
<body>
<?php
$VolumeConversions = array(
     array(1, 0.125, 0.0625, 0.03125, 0.0078125), // Ounces
     array(8, 1, 0.5, 0.25, 0.0625), // Cups
     array(16, 2, 1, 0.5, 0.125), // Pints
     array(32, 4, 2, 1, 0.25), // Quarts
     array(128, 16, 8, 4, 1) // Gallons
);

echo "<pre>";
print_r($VolumeConversions);
echo "</pre>";

echo $VolumeConversions[0][0], "<br \>";
echo $VolumeConversions[4][0], "<br \>";

$VolumeConversions1 = array(
"ounces" => array("ounces" => 1,"cups" => 0.125,
                "pints" => 0.0625, "quarts" => 0.03125,
                 "gallons" => 0.0078125),
"cups" => array("ounces" => 8, "cups" => 1, "pints" =>0.5, "quarts" => 0.25, "gallons" => 0.0625),
"pints" => array("ounces" => 16, "cups" => 2, "pints" =>1, "quarts" => 0.5, "gallons" => 0.125),
"quarts" => array("ounces" => 32, "cups" => 4, "pints" =>2, "quarts" => 1, "gallons" => 0.25),
"gallons" => array("ounces" => 128, "cups" => 16,
            "pints" =>8, "quarts" => 4, "gallons" => 1));

echo "<pre>";
print_r($VolumeConversions1);
echo "</pre>";

echo $VolumeConversions1["ounces"]["pints"], "<br />";

$a    = 'car'; // $a is a string
echo $a . "<br />";
$a[0] = 'b';   // note that $a is still a string
echo $a . "<br />";		//bar
$a[100] = 'b';   // note that $a is still a string
echo $a;




    $a = array(
                "NSW" => "Sydney",
                "Victoria" => "Melbourne",
                "West Australia" => "Perth",
                "Northern Territory" => "Darwin",
                "South Australia" => "Adelaide",
                "ACT" => "Canberra",
                "Tasmania" => "Hobart");

    $b = array("a", "b", "c", "d", "e", "f", "g");

    
    array_splice($a, 2, 0, array("iceland" => "Iceland", "russia" => "Russia", "antartica" => "Antartica"));
    echo '<pre>';
    print_r($a);
    echo '</pre>';

    $c = array("Iceland" => "iceland", "Russia" => "russia", "Antartica" => "antartica");
    
    array_splice($b, 4, 2, $c); //array_splice($b, 4, 0, $c);
    
    echo '<pre>';
    print_r($b);
    echo '</pre>';

?>


</body>
</html>
