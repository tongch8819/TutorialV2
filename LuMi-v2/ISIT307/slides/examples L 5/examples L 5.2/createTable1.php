<!DOCTYPE html>
<html>
<head>
<title>Create Table</title>
</head>
<body>
<?php
    include 'inc_dbconnect.php';
	
	//$conn->select_db("mydb");
/*

    // sql to create table
    $sql = "CREATE TABLE MyGuests1 (
        id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
        firstname VARCHAR(30) NOT NULL,
        lastname VARCHAR(30) NOT NULL,
        email VARCHAR(50),
        reg_date TIMESTAMP
    )";

    try {
        $conn->query($sql);
        echo "Table MyGuests1 created successfully";  }
    catch (mysqli_sql_exception $e) {
        die("Error creating table: " . $e->getCode(). ": " . $e->getMessage());}
*/

    // sql to delete table
    $sql = "DROP TABLE MyGuests2";
    try {
        $conn->query($sql);
        echo "Table MyGuests2 deleted successfully";  }
    catch (mysqli_sql_exception $e) {
        die("Error dropping table: " . $e->getCode(). ": " . $e->getMessage());}
 

$conn->close();
?>
</body>
</html>
