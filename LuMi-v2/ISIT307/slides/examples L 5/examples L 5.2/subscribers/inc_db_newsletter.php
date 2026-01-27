<?php
$DBName = "newsletter";

try {
    $conn = new mysqli("localhost", "root", "");
    $conn->select_db($DBName);
}
catch (mysqli_sql_exception $e) {
    die("connection error");
}
?>
