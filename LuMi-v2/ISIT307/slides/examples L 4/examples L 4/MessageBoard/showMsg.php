<!DOCTYPE html>
<html>
<head>
<title>Post message</title>
</head>
<body>
<h1>Message Board</h1>

<p>
<a href="postMsg.php">Post New Message</a><br />
<a href="showMsg.php?action=RemoveDuplicates">Remove Duplicate Messages</a><br />
<a href="showMsg.php?action=DeleteFirst">Delete First Message</a><br />
<a href="showMsg.php?action=DeleteLast">Delete Last Message</a><br />
<a href="showMsg.php?action=SortAscending">Sort Messages - ascending</a><br />
<a href="showMsg.php?action=SortDescending">Sort Messages - descending</a><br />
</p>

<?php
if (isset($_GET['action'])) {
    //based on the link user has clicked different action/operation will be performed
	if ((file_exists("messages.txt")) && (filesize("messages.txt") != 0)) {
		$MessageArray = file("messages.txt");
        /* the operations are performed on the array containing the content of the file
           -> each element in the array is representing one line of the file content
           -> each element/line is having 3 parts of the information: subject~name~message
         */
		switch ($_GET['action']) {
			case 'DeleteFirst': 
				array_shift($MessageArray);
				break;
			case 'DeleteLast':
				array_pop($MessageArray);
				break;
			case 'DeleteMessage':
				if (isset($_GET['message']))
					array_splice($MessageArray, $_GET['message'], 1);
				/*if (isset($_GET['message'])) {
					$Index = $_GET['message'];
					unset($MessageArray[$Index]);
					$MessageArray = array_values($MessageArray);
					} */
				break;
			case 'RemoveDuplicates':
				$MessageArray = array_unique($MessageArray);
				$MessageArray = array_values($MessageArray);
				break;
			case 'SortAscending':
				sort($MessageArray);
				break;
			case 'SortDescending':
				rsort($MessageArray);
				break;
		} // End of the switch statement
		if (count($MessageArray)>0) {
			$NewMessages = implode($MessageArray); //create one string of all elements (lines) in the array
            /* after operation on the array is completed, the content of the array is changed,
              so the new content will be overwritten in the 'messages.txt' file - the old content will be replaced with the content of the current array
             */
			$MessageStore = fopen("messages.txt","wb"); //opens file for writing only and deletes any existing content
			if ($MessageStore === false)
				echo "There was an error updating the message file\n";
			else {
				fwrite($MessageStore, $NewMessages);
				fclose($MessageStore);
			}
		}
		else
			unlink("messages.txt");
	}
}
/* after any changes are performed on the content, now we have new content of the file, so that content will be re-displayed on the screen
    This is the same code that is executed when the page is loaded for the first time (when no action is selected by the user)
   */
if ((!file_exists("messages.txt")) || (filesize("messages.txt")== 0))
	echo "<p>There are no messages posted.</p>\n";
else {
	$MessageArray = file("messages.txt");
	echo "<table style=\"background-color:lightgray\" border=\"1\" width=\"100%\">\n";
	$count = count($MessageArray);
	for ($i = 0; $i < $count; ++$i) {
		$CurrMsg = explode("~", $MessageArray[$i]);
		$KeyArray[] = $CurrMsg[0];
		$ValueArray[] = $CurrMsg[1] . "~" . $CurrMsg[2];
		//$KeyMessageArray[$CurrMsg[0]] = $CurrMsg[1] . "~" . $CurrMsg[2];
        
        /* instead creating two new arrays, you can simply use $CurrMsg array to display the content of each array element (line in the file) */
	}
	$KeyMessageArray = array_combine($KeyArray, $ValueArray);
	$Index = 1;
	foreach($KeyMessageArray as $Message) {
		$CurrMsg = explode("~", $Message);
		echo "<tr>\n";
		echo "<td width=\"5%\" style=\"text-align:center\"><span style=\"font-weight:bold\">" . $Index . "</span></td>\n";
		echo "<td width=\"85%\"><span style=\"font-weight:bold\"> Subject:</span> " . htmlentities(key($KeyMessageArray)) ."<br />";
		echo "<span style=\"font-weight:bold\"> Name:</span> " . htmlentities($CurrMsg[0]) . "<br />";
		echo "<span style=\"text-decoration:underline; font-weight:bold\">Message</span><br />\n" . htmlentities($CurrMsg[1]) ."</td>\n";
		echo "<td width=\"10%\"	style=\"text-align:center\">" . "<a href='showMsg.php?" . "action=DeleteMessage&" . "message=" . ($Index - 1) . "'>Delete This Message</a>" . "</td>\n";
		echo "</tr>\n"; 
		++$Index;
		next($KeyMessageArray);
	}
}
?>
</body>
</html>

