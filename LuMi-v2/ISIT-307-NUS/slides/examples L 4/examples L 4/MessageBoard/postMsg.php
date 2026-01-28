<!DOCTYPE html>
<html>
<head>
<title>Post message</title>
</head>
<body>
<?php
$msgfile = "messages.txt";
if (isset($_POST['submit'])) {
	$Subject = stripslashes($_POST['subject']);
	$Name = stripslashes($_POST['name']);
	$Message = stripslashes($_POST['message']);
	// Replace any '~' characters with '-' characters
	$Subject = str_replace("~", "-", $Subject);
	$Name = str_replace("~", "-", $Name);
	$Message = str_replace("~", "-", $Message);
    $Message = str_replace("\n", "-", $Message);
	$ExistingSubjects = array();
	if (file_exists($msgfile) && filesize($msgfile)> 0) 
	{
		$MessageArray = file($msgfile);
		$count = count($MessageArray);
		for ($i = 0; $i < $count; ++$i) {
			$CurrMsg = explode("~", $MessageArray[$i]);
			$ExistingSubjects[] = $CurrMsg[0];
		}
	}
	if (in_array($Subject, $ExistingSubjects)) 
	{
		echo "<p>The subject you entered already exists!<br />\n";
		echo "Please enter a new subject and try again.<br />\n";
		echo "Your message was not saved.</p>"; 
		$Subject = "";
	}
	else 
	{
		$MessageRecord = "$Subject~$Name~$Message\n";
		$MessageFile = fopen($msgfile, "ab");// opens file for writing only and place the pointer at the end
		if ($MessageFile === FALSE)
			echo "There was an error saving your message!\n";
		else 
		{
			fwrite($MessageFile, $MessageRecord);
			fclose($MessageFile);
			echo "Your message has been saved.\n";
			$Subject = "";
			$Name = "";
			$Message = "";
		}
	}
}
else 
{
	$Subject = "";
	$Name = "";
	$Message = "";
}
?>
<h1>Post New Message</h1>
<hr />
<form action="postMsg.php" method="POST">
<span style="font-weight:bold">Subject:</span>
<input type="text" name="subject" value="<?php echo $Subject; ?>" />
<span style="font-weight:bold">Name:</span>
<input type="text" name="name" value="<?php echo $Name; ?>" /><br />
<textarea name="message" rows="6" cols="80"><?php echo $Message; ?></textarea><br />
<input type="submit" name="submit" value="Post Message" />
<input type="reset" name="reset" value="Reset Form" />
</form>
<hr />
<p>
<a href="showMsg.php">View Messages</a>
</p>
</body>
</html>

