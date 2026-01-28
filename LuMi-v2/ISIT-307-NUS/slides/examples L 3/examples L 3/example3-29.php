<!DOCTYPE html>
<html>
<head>
<title>Files</title>
</head>
<body>
<?php 
    $Dir = "."; //../examples";
if (isset($_POST['upload'])) {
	if (isset($_FILES['new_file'])) {
		if (move_uploaded_file($_FILES['new_file']['tmp_name'], $Dir . "/" . $_FILES['new_file']['name']) == TRUE) {
			chmod($Dir . "/" . $_FILES['new_file']['name'], 0644);
			echo "File \"" . htmlentities($_FILES['new_file']['name']) ."\"successfully uploaded. <br />\n";
		}
		else
			echo "There was an error:". $_FILES['new_file']['error'] . " uploading \"" . htmlentities($_FILES['new_file']['name']) . "\".<br />\n";
	}
}
?>
<form action="example3-29.php" method="POST" enctype="multipart/form-data">
<input type="hidden" name="MAX_FILE_SIZE" value="500000" /><br />
	File to upload:<br />
<input type="file" name="new_file" /><br />
	(500,000 byte limit) <br />
<input type="submit" name="upload" value="Upload the File" />
<br />
</form>
</body>
</html>

