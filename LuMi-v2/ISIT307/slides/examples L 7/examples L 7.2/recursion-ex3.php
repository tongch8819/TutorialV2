<!DOCTYPE html>
<html>
<head>
<title>recursion</title>
</head>
<body>
<?php
function test()
{
    static $step = 0;
    //$step = 0;
    if ($step < 3) {
		$step++;
        echo "<p>into function step = $step </p>";
        test();
        $step--;
        echo "<p>out of function step = $step </p>";
    }
	else {
		echo "finish";
	}
     
}

test();

echo "<p>out of function";

?>

</body>
</html>
