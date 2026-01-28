<!DOCTYPE html>
<html>
<head>
	<title>functions</title>
</head>
<body>
<h1>functions</h1>
<p>
<?php

	function IncrementByValue($CountByValue) {
		++$CountByValue;
		echo "<p>IncrementByValue() value is $CountByValue.</p>";
	};

	function IncrementByReference(&$CountByReference) {
		++$CountByReference;
		echo "<p>IncrementByReference() value is $CountByReference.</p>";
	};
	
	

	$Count = 1;
	echo "<p>Main program starting value is $Count.</p>";
	IncrementByValue($Count);
	echo "<p>Main program after call for IncrementByValue, count value is $Count.</p>";
	IncrementByReference($Count);
	echo "<p>Main program after call for IncrementByReference, count value is $Count.</p>";

?>

</p>
</body>
</html>
