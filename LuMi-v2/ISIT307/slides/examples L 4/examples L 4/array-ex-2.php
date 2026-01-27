<!DOCTYPE html>
<html>
<head>
<title>Arrays</title>
</head>
<body>
<?php
$double = function($a) {
    return $a * 2;
};
function cube($a){
	return $a*$a*$a;
};

// This is our range of numbers
$numbers = range(1, 5);
echo "<pre>";
print_r ($numbers);
echo "</pre>";

// Use a callback here to
// double the size of each element in our
// range
$new_numbers = array_map($double, $numbers);
echo "<pre>";
print_r ($new_numbers);
echo "</pre>";


// Use a callback here to
// cube the size of each element in our
// range
$new_numbers = array_map('cube', $numbers);
echo "<pre>";
print_r ($new_numbers);
echo "</pre>";

$a1 = array("a"=>1, "b"=>2, "c"=>3);
$new_numbers = array_map('cube', $a1);
echo "<pre>";
print_r ($new_numbers);
echo "</pre>";

/*
In PHP, callback is a function object/reference with type callable. A callback/callable variable can act as a function, object method and a static class method.
 A callback function is a function which is passed as an argument into another function. Any existing function can be used as a callback function. To use a function as a callback function, pass a string containing the name of the function as the argument of another function.
Anonymous functions, also known as closures, allow the creation of functions which have no specified name. They are most useful as the value of callback parameters, but they have many other uses.
array_map() returns an array containing the results of applying the callback function to the corresponding index of array1 (and ... if more arrays are provided) used as arguments for the callback. The number of parameters that the callback function accepts should match the number of arrays passed to array_map().
*/

?>
</body>
</html>
