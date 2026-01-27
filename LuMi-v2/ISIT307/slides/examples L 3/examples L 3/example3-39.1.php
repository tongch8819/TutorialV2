<!DOCTYPE html>
<html>
<head>
<title>Volunteers</title>
</head>
<body>
<h1>Coast City Charity Event Volunteers</h1>
<?php
	if (isset($_POST['first_name']) && isset($_POST['last_name'])) {
		$VolunteerFirst = addslashes($_POST['first_name']);
		$VolunteerLast = addslashes($_POST['last_name']);
		$NewVolunteer = "$VolunteerLast, $VolunteerFirst \n";
		$VolunteersFile = "volunteers.txt";
		if (file_put_contents($VolunteersFile, $NewVolunteer, FILE_APPEND) > 0)
			echo "<p>" . stripslashes($_POST['first_name']) . " " . stripslashes($_POST['last_name']) . " has been registered to volunteer at the event!</p>\n";
		else
			echo "<p>Registration error!</p>";
	}
	else
		echo "<p>To sign up to volunteer at the event, enter your first and last name and click the Registerbutton.</p>";
?>
<form action="example3-39.1.php" method="POST">
<p>First Name: <input type="text" name="first_name" size="30" /></p>
<p>Last Name: <input type="text" name="last_name" size="30" /></p>
<p><input type="submit" value="Register" /></p>
</form>
</body>
</html>

