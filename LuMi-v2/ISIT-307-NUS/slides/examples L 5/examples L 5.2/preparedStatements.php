<!DOCTYPE html>
<html>
<head>
<title>prepared statements</title>
</head>
<body>
<?php


    include ("inc_dbconnect.php");
    
try {

    // prepare and bind
    $stmt = $conn->prepare("INSERT INTO MyGuests (firstname, lastname, email) VALUES (?, ?, ?)"); 
    $stmt->bind_param("sss", $fname, $lname, $email);


    // set parameters and execute
    $fname = "John555";
    $lname = "Doe555";
    $email = "john@example.com";
    $stmt->execute();

    $fname = "Mary333";
    $lname = "Moe333";
    $email = "mary@example.com";
    $stmt->execute();

    $stmt->close();
}
catch (mysqli_sql_exception $e){
        die("Unable to execute the query" . $e->getCode(). ": " . $e->getMessage());
}
  
$conn->close();

?>
</body>
</html>
