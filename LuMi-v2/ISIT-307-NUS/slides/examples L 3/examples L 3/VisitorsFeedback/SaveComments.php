<!DOCTYPE html>
<html>
<head>
<title>Visitor comments</title>
</head>
<body>
<?php 
$Dir = "comments";
if (is_dir($Dir)) {	//in case there is not we need first to create it manualy or in code
	if (isset($_POST['save'])) {
		if (empty($_POST['name']))
			$SaveString = "Unknown Visitor\n";
		else
			$SaveString = stripslashes($_POST['name']) . "\n";
		
		$SaveString .= stripslashes($_POST['email']) . "\n";
		$SaveString .= date('r') . "\n";
		$SaveString .= stripslashes($_POST['comment']);
		$CurrentTime = microtime();
		$TimeArray = explode(" ", $CurrentTime);
		$TimeStamp = (float)$TimeArray[1] + (float)$TimeArray[0]; 
		// File name is "comment.second.microseconds.txt" */
		$SaveFileName = "$Dir/Comment.$TimeStamp.txt";
		if (file_put_contents($SaveFileName, $SaveString)>0)
			echo "File \"" . htmlentities($SaveFileName) . "\" successfully saved.<br />\n";
		else
			echo "There was an error writing \"" . htmlentities($SaveFileName) . "\".<br />\n";
	}
}
?>
<h2>Visitor Comments</h2>
<form action="SaveComments.php" method="POST">
	Your name: <input type="text" name="name" /><br />
	Your email: <input type="text" name="email" /><br />
<textarea name="comment" rows="6" cols="100"></textarea><br />
<input type="submit" name="save" value="Submit your comment" /><br />
</form>
</body>
</html>

