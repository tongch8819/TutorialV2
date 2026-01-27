<!DOCTYPE html>
<html>
<head>
<title>Create DATABASE</title>
</head>
<body>
<?php
include "inc_dbconnect.php";

// Create database
$sql = "CREATE DATABASE myDB2";
try {
    $conn->query($sql);
    echo "Database created successfully"; }
catch(mysqli_sql_exception $e) {
    die("Error creating database: " . $e->getCode(). ": " . $e->getMessage()); }

//Drop database
/*
$sql = "DROP DATABASE myDB2";
try {
    $conn->query($sql);
    echo"Database deleted successfully";   
 }
catch(mysqli_sql_exception $e){
    die( "Error deleting database: " . $e->getCode(). ": " . $e->getMessage());}
*/
$conn->close();
?>
</body>
</html>
