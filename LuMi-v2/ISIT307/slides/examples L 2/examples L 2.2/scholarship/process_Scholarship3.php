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
         if (empty($data)) {
              displayRequired($fieldName);
              ++$errorCount;
              $retval = "";
         } else { // Only clean up the input if it isn't empty
              $retval = trim($data);
              $retval = stripslashes($retval);
         }
         return($retval);
    }
    
    function redisplayForm($firstName, $lastName) {
    ?>
        <h2 style = "text-align:center">Scholarship Form</h2>
        <form name = "scholarship" action = "process_scholarship3.php" method = "post">
        <p>First Name: <input type="text" name="fName" value="<?php echo $firstName; ?>" /></p>
        <p>Last Name: <input type="text" name="lName" value="<?php echo $lastName; ?>" /></p>
        <p><input type="reset" value="Clear Form" />&nbsp; &nbsp;<input type="submit" name="Submit" value="Send Form" />
        </form>
    <?php
    }

    $errorCount = 0;
    $firstName = validateInput($_POST['fName'], "First name");
    $lastName = validateInput($_POST['lName'], "Last name");

    if ($errorCount>0) {
         echo "Please re-enter the information below.<br />\n";
         redisplayForm($firstName, $lastName);
    }
    else {
         $To = "xxx@gmail.com";
         $Subject = "Scholarship Form Results";
         $Message = "Student Name: " . $firstName. " " . $lastName;
         $result = mail($To, $Subject, $Message);
         if ($result)
              $resultMsg = "Your message was successfully sent.";
         else
              $resultMsg = "There was a problem sending your message.";
    ?>
    <h2 style = "text-align:center">Scholarship Form</h2>
    <p style = "line-height:200%">Thank you for filling out the scholarship form<?php
         if (!empty($firstName))
              echo ", $firstName"
         ?>. <?php echo $resultMsg; ?>
    <?php
    }
?>
</body>
</html>
