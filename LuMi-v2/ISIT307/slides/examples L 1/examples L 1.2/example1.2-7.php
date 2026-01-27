<!DOCTYPE html>
<html>
<head>
	<title>functions</title>
</head>
<body>
	<h1>functions</h1>
	<p>
	<?php
		function average_numbers($a, $b, $c) {
            $sum = $a+$b+$c;
            $avg = $sum/3;
            //$avg = (int)($sum/3);
		
            echo "<p> $a+$b+$c </p>";
            echo "<p>", $a+$b+$c, "</p>";
            
            return $avg;
        }
	
	echo average_numbers(5,6,7);
    echo "<br />------ <br />";
    echo average_numbers(5,5,7);
	?>
	</p>
</body>
</html>
