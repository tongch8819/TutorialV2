<!DOCTYPE html>
<html>
<head>
<title>LogIn/REGISTER</title>
</head>
<body>
<h1>College Internship</h1>
<h2>Intern Registration</h2>

<?php
$errors = 0;
$email = "";
if (empty($_POST['email'])) {
	++$errors;
	echo "<p>You need to enter an e-mail address.</p>\n";
	}
else {
	$email = stripslashes($_POST['email']);
	if (preg_match("/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/i", $email) == 0) {
		++$errors;
		echo "<p>You need to enter a valid " . "e-mail address.</p>\n";
		$email = "";
	}
}

if (empty($_POST['password'])) {
	++$errors;
	echo "<p>You need to enter a password.</p>\n"; 
	$password = "";
}
else
	$password = stripslashes($_POST['password']);

if (empty($_POST['password2'])) {
	++$errors;
	echo "<p>You need to enter a confirmation password.</p>\n";
	$password2 = "";
}
else
	$password2 = stripslashes($_POST['password2']);

if ((!(empty($password))) && (!(empty($password2)))) {
	if (strlen($password) < 6) {
		++$errors;
		echo "<p>The password is too short.</p>\n";
		$password = "";
		$password2 = "";
	}
	if ($password <> $password2) {
		++$errors;
		echo "<p>The passwords do not match.</p>\n";
		$password = "";
		$password2 = "";
	}
}

if ($errors == 0) {
    try {
        include ("inc_dbconnect.php");
		
        $TableName = "interns";
        $SQLstring = "SELECT count(*) FROM $TableName" . " where email=$email";
        $QueryResult = $conn->query($SQLstring);
        $Row = $QueryResult->fetch_row();
        if ($Row[0]>0) {
            echo "<p>The email address entered (" . htmlentities($email) . ") is already registered.</p>\n";
            ++$errors;
        }
    }
    catch (mysqli_sql_exception $e) {
            echo "<p>Unable to connect to the database </p>\n";
            ++$errors;
    }
}
if ($errors > 0) {
	echo "<p>Please use your browser's BACK button to return" . " to the form and fix the errors indicated.</p>\n";
}

if ($errors == 0) {
	$first = stripslashes($_POST['first']);
	$last = stripslashes($_POST['last']);
    try {
        $SQLstring = "INSERT INTO $TableName " . " (first, last, email, password_md5) " . " VALUES( '$first', '$last', '$email', " . " '" . md5($password) . "')";
        $conn->query($SQLstring);
		$InternID = $conn->insert_id;
        $conn->close();
    }
    catch (mysqli_sql_exception $e) {
            echo "<p>Unable to insert record </p>\n";
            ++$errors;
    }
}
if ($errors == 0) {
	$InternName = $first . " " . $last;
	echo "<p>Thank you, $InternName. ";
	echo "Your new Intern ID is <strong>" . $InternID . "</strong>.</p>\n";
}

if ($errors == 0) {
	echo "<form method='post' " . 	" action='AvailableOpportunities.php'>\n";
	echo "<input type='hidden' name='internID' " . " value='$InternID'>\n";
	echo "<input type='submit' name='submit' " . " value='View Available Opportunities'>\n";
	echo "</form>\n";
}
?>
</body>
</html>
