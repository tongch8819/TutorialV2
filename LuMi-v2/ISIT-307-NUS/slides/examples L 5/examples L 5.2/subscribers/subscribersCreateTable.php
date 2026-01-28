<!DOCTYPE html>
<html>
<head>
<title>Create 'subscribers' Table</title>
</head>
<body>
<?php
$TableName = "subscribers";
include("inc_db_newsletter.php");

try {
	$SQLstring = "SHOW TABLES LIKE '$TableName'";
	$QueryResult = $conn->query($SQLstring);
	if ($QueryResult->num_rows == 0) {
		$SQLstring = "CREATE TABLE subscribers (subscriberID SMALLINT NOT NULL AUTO_INCREMENT PRIMARY KEY,
						name VARCHAR(80), email VARCHAR(100), subscribe_date DATE, confirmed_date DATE)";
		$QueryResult = $conn->query($SQLstring);
        echo "<p>Successfully created the " . "subscribers table.</p>";
	}
	else
		echo "<p>The subscribers table already exists.</p>";
}
catch (mysqli_sql_exception $e) {
    echo "<p>Unable to execute the query.</p>" . "<p>Error code " . $e->getCode(). ": " . $e->getMessage() . "</p>";
}

$conn->close();
?>
</body>
</html>
