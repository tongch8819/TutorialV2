<!DOCTYPE html>
<html>
<head>
<title>PHP Code Blocks</title>
</head>
<body>
<h1>Multiple Script Sections</h1>
<h2>First Script Section</h2>
<?php
	print "<p>Output from the first script section.</p>";
?>
<h2>Second Script Section</h2>
<?php 
		echo "<p>Output from the second script section.</p>", "something else";
?>

</body>
</html>
