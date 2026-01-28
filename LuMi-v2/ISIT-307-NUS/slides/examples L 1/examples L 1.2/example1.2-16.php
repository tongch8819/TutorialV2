<!DOCTYPE html>
<html>
<head>
	<title>functions</title>
</head>
<body>
<h1>functions</h1>
<?php

	
	function makecoffee($type = "cappuccino")
	{
		return "<p> Making a cup of $type </p>";
	};

	echo makecoffee();
    echo makecoffee(null);
	echo makecoffee("espresso");
?>

</body>
</html>
