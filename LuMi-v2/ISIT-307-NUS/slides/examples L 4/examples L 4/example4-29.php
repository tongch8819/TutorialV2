<!DOCTYPE html>
<html>
<head>
<title>example </title>
</head>
<body>
<?php 
$Provinces = array("Newfoundland and Labrador", "Prince Edward Island", "Nova Scotia");
$Territories = array("Nunavut", "Northwest Territories");

$Canada = array_merge($Provinces, $Territories);
echo "<pre>\n";
print_r($Canada);
echo "</pre>\n";

$Provinces1 = array("Newfoundland and Labrador" => "St. John's",
                    "Prince Edward Island" => "Charlottetown",
                    "Nova Scotia" => "Halifax");
$Provinces2 = array( "New Brunswick" => "Fredericton",
                            "Quebec" => "Quebec City");
$Canada1 = array_merge($Provinces1, $Provinces2);
echo "<pre>\n";
print_r($Canada1);
echo "</pre>\n";
   
//$Provinces = array("Newfoundland and Labrador", "Prince Edward Island", "Nova Scotia");
//$Territories = array("Nunavut", "Northwest Territories");

$Territories[] = "NortEast";
$Canada2 = array_combine( $Territories, $Provinces);
echo "<pre>\n";
print_r($Canada2);
echo "</pre>\n";


//Fatal error: Uncaught ValueError: array_combine(): Argument #1 ($keys) and argument #2 ($values) must have the same number of elements in /Applications/XAMPP/xamppfiles/htdocs/examples/example4.1-29.php:38 Stack trace: #0 /Applications/XAMPP/xamppfiles/htdocs/examples/example4.1-29.php(38): array_combine(Array, Array) #1 {main} thrown in /Applications/XAMPP/xamppfiles/htdocs/examples/example4.1-29.php on line 38


$Provinces2 = array( "New Brunswick" => "Fredericton", "Quebec" => "Quebec City");

$arrayCity = array("one" => "oneone", "two" => "twotwo");
$Canada3 = array_combine( $arrayCity, $Provinces2);
echo "<pre>\n";
print_r($Canada3);
echo "</pre>\n";


array_shift($Canada3);
echo "<pre>\n";
print_r($Canada3);
echo "</pre>\n";
?>
</body>
</html>

