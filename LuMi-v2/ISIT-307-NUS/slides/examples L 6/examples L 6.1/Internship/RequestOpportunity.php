<?php
session_start();
$Body = "";
$errors = 0;
$InternID = 0;
/*
if (isset($_GET['internID']))
	$InternID = $_GET['internID']; */
if (isset($_SESSION['internID']))
	$InternID = $_SESSION['internID']; 
else {
	$Body .= "<p>You have not logged in or registered. Please return to the <a href='InternLogin.php'>Registration / Log In page</a>.</p>";
	++$errors;
}
if ($errors == 0) {
	if (isset($_GET['opportunityID']))
		$OpportunityID = $_GET['opportunityID'];
	else {
		$Body .= "<p>You have not selected an opportunity. Please return to the <a href='AvailableOpportunities.php?". SID . "'>Available Opportunities page</a>.</p>";
		++$errors;
	}
}

if ($errors == 0) {
    try {
        include ("inc_dbconnect.php");
		
        $DisplayDate = date("Y-m-d");
        $DatabaseDate = date("Y-m-d H:i:s");
        $TableName = "assigned_opportunities";
        $sql = "INSERT INTO $TableName (opportunityID, internID, date_selected) VALUES ($OpportunityID, $InternID, '$DatabaseDate')";
        $conn->query($sql) ;
		$Body .= "<p>Your request for opportunity # " . " $OpportunityID has been entered on $DisplayDate.</p>\n";
        $conn->close();
    }
    catch (mysqli_sql_exception $e) {
        $Body .= "<p>Unable to execute the query.</p>\n";
        ++$errors;
    }
}

if ($InternID > 0)
	$Body .= "<p>Return to the <a href='AvailableOpportunities.php?". SID . "'>Available Opportunities</a> page.</p>\n";
else
	$Body .= "<p>Please <a href='InternLogin.php'>Register or Log In</a> to use this page.</p>\n";

if ($errors == 0)
	setcookie("LastRequestDate", urlencode($DisplayDate), time()+60*60*24*7); //, "/examples/internship/");
?>
<!DOCTYPE html>
<html>
<head>
<title>Request opportunities</title>
</head>
<body>
<h1>College Internship</h1>
<h2>Oportunity requested Registration</h2>
<?php
echo $Body;
?>
</body>
</html>
