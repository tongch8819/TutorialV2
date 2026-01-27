<!DOCTYPE html>
<html>
<head>
<title>example</title>
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

array_splice($HospitalDepts, 3, 0, array("Ophtamology", "Otolaryngology"));

echo "<h2>Array after adding at 3</h2>\n";
echo "<pre>\n";
print_r($HospitalDepts);
echo "</pre>\n";

array_splice($HospitalDepts, 3, 0, "Add New");

echo "<h2>Array after adding at 3</h2>\n";
echo "<pre>\n";
print_r($HospitalDepts);
echo "</pre>\n";

array_splice($HospitalDepts, 1, 2);
    
echo "<h2>Array after delete at 1 delete 2</h2>\n";
echo "<pre>\n";
print_r($HospitalDepts);
echo "</pre>\n";

    
array_splice($HospitalDepts, 1, 1, array("add1", "add2"));
echo "<pre>\n";
print_r($HospitalDepts);
echo "</pre>\n";
    


?>
</form>
</body>
</html>

