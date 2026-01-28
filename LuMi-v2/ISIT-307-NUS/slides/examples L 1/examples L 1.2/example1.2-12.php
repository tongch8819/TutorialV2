<!DOCTYPE html>
<html>
<head>
	<title>functions</title>
</head>
<body>
<h1>functions</h1>
<?php

function add_some(&$text) {
    $text = "one". $text."problem?";
}

$my_text = "Is there ";
echo "<p>",$my_text, "</p>";
add_some($my_text);
echo "<p>",$my_text, "</p>";

$new = "second";
add_some($new);
echo "<p>",$new, "</p>";


?>

</body>
</html>
