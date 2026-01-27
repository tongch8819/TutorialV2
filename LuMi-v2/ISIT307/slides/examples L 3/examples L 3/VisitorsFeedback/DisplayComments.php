<!DOCTYPE html>
<html>
<head>
<title>Visitors' comments</title>
</head>
<body>
<h2>Visitor Feedback</h2>
<hr />
<?php
	$Dir = "comments";
	if (is_dir($Dir)) {
		$CommentFiles = scandir($Dir, 1);
		foreach ($CommentFiles as $FileName) {
			if (($FileName != ".") && ($FileName !="..")) {
				echo "From <strong>$FileName</strong><br />";
				echo "<pre>\n";
				$Comment = file_get_contents($Dir . "/" .$FileName);
				echo $Comment;
				//readfile($Dir . "/" .$FileName);
				echo "</pre>\n";
				echo "<hr />\n";
			}
		}
	}
?>

</body>
</html>

