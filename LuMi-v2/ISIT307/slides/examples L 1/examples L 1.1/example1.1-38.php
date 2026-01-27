<!DOCTYPE html>
<html>
<head>
	<title>PHP Code Blocks</title>
</head>
<body>
	<h1>PHP variables</h1>
	<p>
	<?php
        
        $VotingAge = 18;
		echo $VotingAge;
		echo "<p>The legal voting age is ", $VotingAge, ".</p>";
		echo "<p>The legal voting age is $VotingAge</p>";
		echo '<p>The legal voting age is $VotingAge</p>';
		
        $VotingAge = "value is changed";
		echo "<p>The value of the variable is $VotingAge</p>";
        
        
		
		
		$myarray = array("first", "second", "third");
		echo "<pre>";
        print_r($myarray);
        echo "</pre>";

		$myarray[] = "new one";

		echo "<pre>";
        print_r($myarray);
        echo "</pre>";
        
        $myarray[2] = "changing";
        print_r($myarray);

        $list = "one";

        $list = 10;
        
        
		
		
		$var1 = "120-130 people";
		//$var2 = (int)$var1;
        $var2 = (int)$var1-5;
		
		echo " <p> $var1 </p>";
		echo " <p> $var2 </p>";

	?>
	</p>
</body>
</html>
