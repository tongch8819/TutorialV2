<!DOCTYPE html>
<html>
<head>
<title>Scholarships</title>
</head>
<body>
<?php
    function displayRequired($fieldName) {
        echo "The field \"$fieldName\" is required.<br />\n";
    }
    function validateInput($data, $fieldName) {
        global $errorCount;
        if (empty($data)) { //if it is empty
            displayRequired($fieldName);
            ++$errorCount;
            $retval = "";
        } else { // Only clean up the input if it isn't empty
            $retval = trim($data);
            $retval = stripslashes($retval);
        }
        return($retval);
    }
    $errorCount = 0;
    //$firstName = validateInput($_GET['fName'], "First name");
    //$lastName = validateInput($_GET['lName'], "Last name");
    $firstName = validateInput($_POST['fName'],"First name");
    $lastName = validateInput($_POST['lName'],"Last name");
    if ($errorCount>0)
        echo "Please use the \"Back\" button to re-enter the data.<br />\n";
    else
        echo "Thank you for filling out the scholarship form, " . $firstName . " " . $lastName . ".";
?>
</body>
</html>

