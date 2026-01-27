<?php
session_start();
?>
<!DOCTYPE html>
<html>
<head>
<title>Available opportunities</title>
</head>
<body>

<h1>College Internship</h1>
<h2>Available Opportunities</h2>

<?php

/*
if (isset($_SESSION['internID']))
	$InternID = $_SESSION['internID'] ;
else	
	$InternID = −1;
*/
/*
if (isset($_GET['internID']))
	$InternID = $_GET['internID'];
else
		$InternID = −1;
*/
/*
if (isset($_REQUEST['internID']))
	$InternID = $_REQUEST['internID'];
else
		$InternID = −1;
*/

if (isset($_COOKIE['LastRequestDate']))
	$LastRequestDate = $_COOKIE['LastRequestDate'];
else
	$LastRequestDate = "";

try {
    include ("inc_dbconnect.php");
    
    $TableName = "interns";
    $sql = "SELECT * FROM $TableName WHERE internID='" . $_SESSION['internID'] . "'";
    $qRes = $conn->query($sql);
    if ($qRes->num_rows == 0) {
        die ("<p>Invalid Intern ID!</p>");
    }
    $Row = $qRes->fetch_assoc();
    $InternName = $Row['first'] . " " . $Row['last'];
    
    $TableName = "assigned_opportunities";
    $ApprovedOpportunities = 0;
    $sql = "SELECT COUNT(opportunityID) FROM $TableName WHERE internID='" . $_SESSION['internID'] . "' AND date_approved IS NOT NULL";
    $qRes = $conn->query($sql);
    if ($qRes->num_rows > 0) {
        $Row = $qRes->fetch_row();
        $ApprovedOpportunities = $Row[0];
        $qRes->free_result();
    }
        
    $SelectedOpportunities = array();
    $sql = "SELECT opportunityID FROM $TableName WHERE internID='" . $_SESSION['internID'] . "'";
    $qRes = $conn->query($sql);
    if ($qRes->num_rows > 0) {
        while ($Row = $qRes->fetch_row())
            $SelectedOpportunities[] = $Row[0];
        $qRes->free_result();
    }

    $AssignedOpportunities = array();
    $sql = "SELECT opportunityID FROM $TableName WHERE date_approved IS NOT NULL";
    $qRes = $conn->query($sql);
    if ($qRes->num_rows > 0) {
        while ($Row = $qRes->fetch_row())
            $AssignedOpportunities[] = $Row[0];
        $qRes->free_result();
    }

    $TableName = "opportunities";
    $Opportunities = array();
    $sql = "SELECT opportunityID, company, city, start_date, end_date, position, description FROM $TableName";
    $qRes = $conn->query($sql);
    if ($qRes->num_rows > 0) {
        while ($Row = $qRes->fetch_assoc())
            $Opportunities[] = $Row;
        $qRes->free_result();
    }
    $conn->close();

}
catch (mysqli_sql_exception $e){
    die ("<p>Error in connection with the database server or database </p>\n");
}


if (!empty($LastRequestDate))
	echo "<p>You last requested an internship opportunity on " . $LastRequestDate . ".</p>\n";

echo "<table border='1' width='100%'>\n";
echo "<tr>\n";
echo " <th style='background-color:cyan'>Company</th>\n";
echo " <th style='background-color:cyan'>City</th>\n";
echo " <th style='background-color:cyan'>StartDate</th>\n";
echo " <th style='background-color:cyan'>EndDate</th>\n";
echo " <th style='background-color:cyan'>Position</th>\n";
echo " <th style='background-color:cyan'>Description</th>\n";
echo " <th style='background-color:cyan'>Status</th>\n";
echo "</tr>\n";
foreach ($Opportunities as $Opportunity) {
	if (!in_array($Opportunity['opportunityID'], $AssignedOpportunities)) {
		echo "<tr>\n";
		echo " <td>" . htmlentities($Opportunity['company']) . "</td>\n";
		echo " <td>" . htmlentities($Opportunity['city']) . "</td>\n";
		echo " <td>" . htmlentities($Opportunity['start_date']) . "</td>\n";
		echo " <td>" . htmlentities($Opportunity['end_date']) . "</td>\n";
		echo " <td>" . htmlentities($Opportunity['position']) . "</td>\n";
		echo " <td>" . htmlentities($Opportunity['description']) . "</td>\n";
		echo " <td>";
		if (in_array($Opportunity['opportunityID'], $SelectedOpportunities))
			echo "Selected";
		else {
			if ($ApprovedOpportunities>0)
				echo "Open";
			else
				//echo "<a href='RequestOpportunity.php?" . "internID=$InternID&" . "opportunityID=" . $Opportunity['opportunityID'] . "'>Available</a>";
				echo "<a href='RequestOpportunity.php?" . SID . "&opportunityID=" . $Opportunity['opportunityID'] . "'>Available</a>";
		}
		echo "</td>\n";
		echo "</tr>\n";
	}
}

echo "</table>\n";
echo "<p><a href='InternLogin.php'>Log Out</a></p>\n";
?>
</body>
</html>
