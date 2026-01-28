<!DOCTYPE html>
<html>
<head>
<title>example</title>
</head>
<body>
<?php 
$TopSellers = array(
					"Chevrolet Impala",
					"Chevrolet Malibu",
					"Chevrolet Silverado",
					"Ford F-Series",
					"Toyota Camry",
					"Toyota Corolla",
					"Nissan Altima",
					"Honda Accord",
					"Honda Civic",
					"Dodge Ram");
echo "<h2>Original Array</h2>\n";
echo "<pre>\n";
print_r($TopSellers);
echo "</pre>\n";

echo array_shift($TopSellers);

//array_shift($TopSellers);
echo "<h2>Array after Shifting</h2>\n";
echo "<pre>\n";
print_r($TopSellers);
echo "</pre>\n";

array_unshift($TopSellers, "Honda CR-V");
echo "<h2>Array after Unshifting</h2>\n";
echo "<pre>\n";
print_r($TopSellers);
echo "</pre>\n";
    
array_unshift($TopSellers, "Honda M1", "Honda M2", "Honda M3");
echo "<h2>Array after Unshifting</h2>\n";
echo "<pre>\n";
print_r($TopSellers);
echo "</pre>\n";
?>
</form>
</body>
</html>

