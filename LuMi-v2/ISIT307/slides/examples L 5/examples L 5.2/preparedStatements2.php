<!DOCTYPE html>
<html>
<head>
<title>prepared statements</title>
</head>
<body>
<?php


    include ("inc_dbconnect.php");
    
try {
    $sql = "SELECT firstname, lastname, email FROM MyGuests"; //ORDER by firstname
    $stmt = $conn->prepare($sql);

    // execute statement
    $stmt->execute();

    //fetch the resultset
    echo "with resultset <br >";
    $qRes = $stmt->get_result();
    while ($row = $qRes->fetch_row())
    {
        echo $row[0]. " -" . $row[1] . " - " . $row[2] . "<br />";
    }

    // close statement
    $stmt->close();

}
catch (mysqli_sql_exception $e){
        die("Unable to execute the query" . $e->getCode(). ": " . $e->getMessage());
}
  
$conn->close();

?>
</body>
</html>
