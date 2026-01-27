<!DOCTYPE html>
<html>
<head>
<title>subscribers Table</title>
</head>
<body>
<h1>Newsletter subscribers</h1>
<?php
include("inc_db_newsletter.php");
$TableName = "subscribers";
try {
	$sql = "SELECT * FROM $TableName ORDER BY SubscriberID";
	$qRes = $conn->query($sql);
    echo "<table width='100%' border='1'>\n";
    echo "<tr><th>Subscriber ID</th>" .
         "<th>Name</th><th>Email</th>" .
         "<th>Subscribe Date</th>" .
         "<th>Confirm Date</th></tr>\n";
    while ($Row = $qRes->fetch_assoc()) {
        echo "<tr><td>{$Row['subscriberID']}</td>";
        echo "<td>{$Row['name']}</td>";
        echo "<td>{$Row['email']}</td>";
        echo "<td>{$Row['subscribe_date']}</td>";
        echo "<td>{$Row['confirmed_date']}</td></tr>\n";
    };
    echo "</table>\n";

    echo "<p>Your query returned the above " . $qRes->num_rows . " rows each with ". $qRes->field_count . " fields.</p>";
    $qRes->free_result();
    
    echo "<p><a href='subscribersAdd.php'> new subscriber</a></p>\n";
	$conn->close();
}
catch (mysqli_sql_exception $e) {
	die("Error: " . $e->getMessage());
}

?>
</body>
</html>
