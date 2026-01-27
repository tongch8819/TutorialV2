<!DOCTYPE html>
<html>
<head>
<title>unique array</title>
</head>
<body>
<?php 
$TopSellers = array(
					"Ford F-Series", "Chevrolet Silverado",
					"Toyota Camry", "Honda Accord",
					"Toyota Corolla", "Ford F-Series",
					"Honda Civic", "Honda CR-V", "Honda Accord",
					"Nissan Altima", "Toyota Camry",
					"Chevrolet Impala", "Dodge Ram",
					"Honda CR-V");
echo "<p>The top selling vehicles are:</p>\n<p>";
/*
for ($i = 0; $i < count($TopSellers); ++$i) {
    echo "{$TopSellers[$i]}<br />\n";
}
 */
echo "<pre>\n";
print_r($TopSellers);
echo "</pre>\n";
echo "</p>\n";

$TopSellers = array_unique($TopSellers);
echo "<h2>Array after removing duplicates</h2>\n";
echo "<pre>\n";
print_r($TopSellers);
echo "</pre>\n";

    
$TopSellers = array_values($TopSellers);
echo "<h2>Array after renumbering</h2>\n";
echo "<pre>\n";
print_r($TopSellers);
echo "</pre>\n";

unset($TopSellers[6]);
echo "<h2>Array after unset</h2>\n";
echo "<pre>\n";
print_r($TopSellers);
echo "</pre>\n";
    
?>
</body>
</html>

