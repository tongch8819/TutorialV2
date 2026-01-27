<?php
    declare(strict_types=1);
?>
<!DOCTYPE html>
<html>
<head>
	<title>functions</title>
</head>
<body>
<h1>functions</h1>
<?php
function double_num(int|float $number) : int|float {
    return $number*=2;
}
$num = 5;
echo '$num =', $num, '<br>';
echo 'double_num returns ', double_num($num), '<br>';

$num = 4.6;
echo '$num =', $num, '<br>';
echo 'double_num returns ', double_num($num), '<br>';
?>

</body>
</html>
