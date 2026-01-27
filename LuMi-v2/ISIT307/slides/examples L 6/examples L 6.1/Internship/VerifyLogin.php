<?php
session_start();
?>
<!DOCTYPE html>
<html>
<head>
<title>verify login</title>
</head>
<body>
<h1>College Internship</h1>
<h2>Verify Intern Login</h2>

<?php
$errors = 0;

try {
    include ("inc_dbconnect.php");
    
    $TableName = "interns";
    $SQLstring = "SELECT internID, first, last FROM $TableName" . " where email='" . stripslashes($_POST['email']) ."' and password_md5='" . md5(stripslashes($_POST['password'])) . "'";
    $qRes = $conn->query($SQLstring);
    if ($qRes->num_rows==0) {
        echo "<p>The e-mail address/password " . " combination entered is not valid. </p>\n";
        ++$errors;
    }
    else {
        $Row = $qRes->fetch_assoc();
        $InternID = $Row['internID'];
        $InternName = $Row['first'] . " " . $Row['last'];
        echo "<p>Welcome back, $InternName!</p>\n";
        $_SESSION['internID'] = $InternID;
		
		$qRes->free_result();
		$conn->close();
    }
}
catch(mysqli_sql_exception $e) {
    echo "<p>Error: unable to connect/insert record in the database.</p>";
    ++$errors;
}
if ($errors > 0) {
	echo "<p>Please use your browser's BACK button to return " . " to the form and fix the errors indicated.</p>\n";
}
if ($errors == 0) {
	echo "<form method='post' " . " action='AvailableOpportunities.php?" . SID . "'>\n";
	//echo "<input type='hidden' name='internID' " . " value='$InternID'>\n";
	echo "<input type='submit' name='submit' " . " value='View Available Opportunities'>\n";
	echo "</form>\n"; 
	//echo "<p><a href='AvailableOpportunities.php?" . "internID=$InternID'>Available " . " Opportunities</a></p>\n";
}
?>
</body>
</html>
