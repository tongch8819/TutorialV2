<!DOCTYPE html>
<html>
<head>
<title>Create Table</title>
</head>
<body>
<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "mydb";

// Create connection
try {
    $conn = new mysqli($servername, $username, $password, $dbname);

	//create table
	$sql = "CREATE TABLE MyGuests2 (
        id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
        firstname VARCHAR(30) NOT NULL,
        lastname VARCHAR(30) NOT NULL,
        email VARCHAR(50))";

    $conn->query($sql);
    echo "Table MyGuests2 created successfully";  
}
catch (mysqli_sql_exception $e) {
        die("Error creating table: " . $e->getCode(). ": " . $e->getMessage());
}

$conn->close();
?>
</body>
</html>
