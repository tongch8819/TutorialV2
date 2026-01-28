<?php
    session_start();
    $_SESSION['firstName'] = "Elena";
    $_SESSION['lastName'] = "Vlahu";
    $_SESSION['occupation'] = "lecturer";
    $a = array("one", "two");
    $_SESSION['arr'] = $a;
    ?>
<!DOCTYPE html>
<html>
<head>
<title>Session</title>
</head>
<body>
<h1>Session</h1>
<?php
    
    if (isset($_SESSION['firstName']) && isset($_SESSION['lastName'])&& isset($_SESSION['occupation']))
        echo "<p>" . $_SESSION['firstName'] . " ". $_SESSION['lastName'] . " is a " . $_SESSION['occupation'] . "</p>";
        print_r ($_SESSION['arr']);
?>

    <p><a href='<?php echo "example6-33.2.php?PHPSESSID=" . session_id() ?>'>next SESSION_ID</a></p>
    <p><a href='<?php echo "example6-33.1.php?" . SID ?>'>next SID</a></p>
    <p><a href='<?php echo "example6-33.3.php" ?>'>next COOKIE</a></p>
</body>
</html>
