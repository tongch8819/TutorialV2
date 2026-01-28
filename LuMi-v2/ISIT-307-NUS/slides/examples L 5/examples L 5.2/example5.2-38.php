<!DOCTYPE html>
<html>
<head>
<title>Databases</title>
</head>
<body>
<h1>records</h1>
<?php
    include ("inc_dbconnect.php");

/*
    $sql = "INSERT INTO myguests(firstname,lastname, email) VALUES('Sim', 'Yang', 'yyyy@gmail.com')";
	try {
		$conn->query($sql);
		$GuestID = $conn->insert_id;
		echo "Your ID is $GuestID <br />";
	}
	catch (mysqli_sql_exception $e) {
		echo "Unable to insert the the record";
	}


    $sql = "INSERT INTO MyGuests " .
     " (firstname, lastname, email) " .
     " VALUES " .
     " ('Tom9999', 'Hon', 'tt@gmail.com'), " .
     " ('Tara999', 'Davis', 'tara@gmail.com'), " .
     " ('Kate999', 'Smith', 'kate@gmail.com')";

	try {
	   $conn->query($sql);
	   echo "Successfully added the records.<br />";
	   echo $conn->info, "<br />";
	}
	catch (mysqli_sql_exception $e) {
	   die("Unable to execute the query" . 
			 $e->getCode(). ": " . $e->getMessage());
	}

*/

/*
    $id=138;
    $email = "xxxxxx@gmail.com";
    $sql = "UPDATE MyGuests SET email='" . $email . "' WHERE id=" . $id ;
	try {
		$conn->query($sql);
		echo "Record updated successfully <br />"; }
	catch (mysqli_sql_exception $e) {
		die("Error in updating: " . $e->getMessage() );  
	}
        
    $id=140;
    $sql = "DELETE FROM MyGuests where id=" . $id;

	try {
	   $conn->query($sql);
	   echo $conn->affected_rows . 
				" row(s) were deleted.<br />"; 
	}
	catch (mysqli_sql_exception $e) {
	   echo "error" . $e->getMessage();  
	}
*/


    $sql = "SELECT * FROM MyGuests";
    
    try {
        $qResult = $conn->query($sql);
        echo $qResult->num_rows . " row(s) in the table, with " . $qResult->field_count . " fields in each row. <br />";
        //while ($row = $qResult->fetch_row())
        while ($row = $qResult->fetch_assoc())
        {
            //echo $row[0] . "-" . $row[1] . "-" . $row[2] . "-" . $row[3]  . "<br />";
            //echo $row['id'] . "-" . $row['firstname'] . "-" . $row['lastname'] . "-" . $row['email']  . "<br />";
            print_r($row);
            echo "<br />";
            }
        $qResult->free_result();
    }
    catch (mysqli_sql_exception $e) {
        die($e->getMessage());
    }


    $conn->close();
    
?>
</body>
</html>
