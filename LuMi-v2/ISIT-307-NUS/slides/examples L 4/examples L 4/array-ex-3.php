<!DOCTYPE html>
<html>
<head>
<title>Arrays</title>
</head>
<body>
<?php

$fruits = array("a" => "orange", "b" => "banana", "c" => "apple", "d" => "lemon");

function test_alter(&$item1, $key, $prefix)
{
    $item1 = "$prefix: $item1";
}

function test_print($item1, $key)
{
    echo "$key. $item1<br />\n";
}

echo "Before ...:<br />";
array_walk($fruits, 'test_print');

array_walk($fruits, 'test_alter', 'fruit');
echo "... and after:<br />";

array_walk($fruits, 'test_print');

/*
bool array_walk (array &$array , callable $callback [, mixed $userdata = NULL])
Applies the user-defined callback function to each element of the array.
When processing the (associative) array, the first parameter of your function must be value, the second should be key, and the third can be user data (NULL if not provided)
*/

?>

</body>
</html>

