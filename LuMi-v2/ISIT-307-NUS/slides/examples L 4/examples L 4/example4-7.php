<!DOCTYPE html>
<html>
<head>
<title>pop/push array</title>
</head>
<body>
<?php 
$HospitalDepts = array(
					"Anesthesia",
					"Molecular Biology",
					"Neurology",
					"Pediatrics");

echo "<h2>Original Array</h2>\n";
echo "<pre>\n";
print_r($HospitalDepts);
echo "</pre>\n";

echo array_pop($HospitalDepts);
//array_pop($HospitalDepts);
// Removes "Pediatrics"
echo "<h2>Array after pop</h2>\n";
echo "<pre>\n";
print_r($HospitalDepts);
echo "</pre>\n";

array_push($HospitalDepts, "Psychiatry", "Pulmonary Diseases");
echo "<h2>Array after push</h2>\n";
echo "<pre>\n";
print_r($HospitalDepts);
echo "</pre>\n";
    
$HospitalDepts[] = "ER";
echo "<h2>Array after push</h2>\n";
echo "<pre>\n";
print_r($HospitalDepts);
echo "</pre>\n";
?>
</form>
</body>
</html>

