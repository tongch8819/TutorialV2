<!DOCTYPE html>
<html>
<head>
<title>example </title>
</head>
<body>
<?php
$a[1003]="a";
$a[1100]="b";
$a[-900]="c";
$a[]="d";

print_r($a);

echo "<hr />";


$ProvincialCapitals = array(
   "Newfoundland and Labrador" => "St. John's",
   "Prince Edward Island" => "Charlottetown",
   "Nova Scotia" => "Halifax",
   "New Brunswick" => "Fredericton",
   "Quebec" => "Quebec City");

foreach ($ProvincialCapitals as $Capital) {
   echo "The capital of " . key($ProvincialCapitals)
                 ." is $Capital<br />\n";  }

echo "--------------<br />";

foreach ($ProvincialCapitals as $Capital) {
   echo "The capital of " . key($ProvincialCapitals) .
                                          " is $Capital<br />\n";
   next($ProvincialCapitals);  }

echo "--------------<br />";

foreach ($ProvincialCapitals as $Province => $Capital)
{
    echo "The capital of $Province is $Capital<br />\n";
}
    
echo "--------------<br />";

if (in_array("Charlottetown", $ProvincialCapitals))
     echo "<p>The capital 'Charlottetown' is in the list.</p>";

echo "--------------<br />";

$keyCap = array_search ("Charlottetown", $ProvincialCapitals);
if ($keyCap !== FALSE)
     echo "<p>The 'Charlottetown' is capital of $keyCap.</p>";

echo "--------------<br />";
echo "<br /><h3>When using array_keys()</h3>";
$provinces = array_keys($ProvincialCapitals);
echo "<pre>\n";
print_r($provinces);
echo "</pre>\n";

echo "--------------<br />";


$ProvincialCapitals["something"] = "Charlottetown";
echo "--------------<br />";
echo "<pre>\n";
print_r($ProvincialCapitals);
echo "</pre>\n";
echo "--------------<br />";

$provinces = array_keys($ProvincialCapitals,"Charlottetown");
echo "<pre>\n";
print_r($provinces);
echo "</pre>\n";
    
echo "--------------<br />";

echo "<br /><h3>When using array_slices()</h3>";
$ThreeProvinces = array_slice ($ProvincialCapitals, 2, 3);
foreach ($ThreeProvinces as $Province => $Capital)
{
    echo "The capital of $Province is $Capital<br />\n";
}

echo "--------------<br />";
    
$provinces = array("t1", "t10", "t3", "t14");
natsort($provinces);
//sort($provinces);
echo "<pre>\n";
print_r($provinces);
echo "</pre>\n";

?>
</body>
</html>

