<!DOCTYPE html>
<html>
<head>
	<title>functions</title>
</head>
<body>
<h1>functions</h1>
<?php
function make_sentence($name, $activity="no activity", $hours="") {
    return "Hi $name, you have $activity for $hours hrs";
}
echo make_sentence("John"), '<br>';
echo make_sentence("John", "swimming"), '<br>';
echo make_sentence("John", "swimming", "1"), '<br>';

echo make_sentence("John", activity: "hiking"), '<br>';

echo make_sentence(activity: "hiking", name: "John",  hours: "8"), '<br>';
echo make_sentence("John", hours: "8", activity: "hiking"), '<br>';

echo make_sentence("John", hours: "5"), '<br>';
echo make_sentence(hours: "5", name: "John"), '<br>';

echo "-------------", "<br>";

 function make_sentence1(&$name, $activity="no activity", $hours="") {
     return "Hi $name, you have $activity for $hours hrs";
 }
$nn = "John";
echo make_sentence1($nn), '<br>';
echo make_sentence1($nn, "swimming"), '<br>';
echo make_sentence1($nn, "swimming", "1"), '<br>';
echo make_sentence1(activity: "hiking", name: $nn,  hours: "8"), '<br>';


?>


</body>
</html>
