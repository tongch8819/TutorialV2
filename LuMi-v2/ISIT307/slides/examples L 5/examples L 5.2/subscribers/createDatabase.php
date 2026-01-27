<!DOCTYPE html>
<html>
<head>
<title>Create DATABASE</title>
</head>
<body>
<?php
$servername = "localhost";
$username = "root";
$password = "";

// Create connection
try {
    $conn = new mysqli($servername, $username, $password);
}
catch (mysqli_sql_exception $e) {
    die("Connection failed: " . $e->getCode(). ": " . $e->getMessage());
}

// Create database
$sql = "CREATE DATABASE newsletter";
try {
    $conn->query($sql);
    echo "Database created successfully";
}
catch (mysqli_sql_exception $e) {
    die ("Error creating database: " . $e->getCode(). ": " . $e->getMessage());
}
$conn->close();
?>
</body>
</html>
