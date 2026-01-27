<!DOCTYPE html>
<html>
<head>
<title>diff/intersect array</title>
</head>
<body>
<?php 
$Top10inArea = array("Russia", "China", "Canada", "United States", "Brazil", "Australia", "India", "Argentina",
						"Kazakhstan", "Algeria");
$Top10inPopulation = array("China", "India", "United States", "Indonesia", "Brazil", "Pakistan", "Bangladesh", "Russia",
						"Nigeria", "Japan");
						
$Result = array_diff($Top10inPopulation, $Top10inArea);
$Result = array_values($Result);
echo "<p>The following of the most populous countries are not also the largest in area:</p>\n";
echo "<p>\n";
for ($i = 0; $i < count($Result); ++$i) {
	echo "{$Result[$i]}<br />\n";
}
echo "</p>\n";

$Result = array_intersect($Top10inPopulation, $Top10inArea);
$Result = array_values($Result);
echo "<p>The following of the most populous countries are also among the largest in area:</p>\n";
echo "<p>\n";
for ($i = 0; $i < count($Result); ++$i) {
	echo "{$Result[$i]}<br />\n";
}
echo "</p>\n";
?>
</form>
</body>
</html>

