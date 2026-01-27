<!DOCTYPE html>
<html>
<head>
<title>recursion</title>
</head>
<body>
<?php
 
function factorial( $n ) {
  if ( $n == 0 ) {
    echo "Base case: \$n = 0. Returning 1...<br>";
    return 1;
  }
  echo "Computing {$n} * " . ($n-1) . "! <br>";
  $result = ( $n * factorial( $n-1 ) );
  echo "Result of $n * " . ($n-1) . "! = {$result}. Returning {$result}...<br>";
  return $result;
}
 
echo "The factorial of 3 is: " . factorial( 3 );
 
?>


</body>
</html>
