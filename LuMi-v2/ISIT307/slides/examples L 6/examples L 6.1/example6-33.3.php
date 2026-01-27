<?php
    session_start();
    ?>
<!DOCTYPE html>
<html>
<head>
<title>Session</title>
</head>
<body>
<h1>Session 33.3</h1>
<?php
    if (isset($_SESSION['firstName']) && isset($_SESSION['lastName'])&& isset($_SESSION['occupation']))
        echo "<p> 33.3-" . $_SESSION['firstName'] . " ". $_SESSION['lastName'] . " is a " . $_SESSION['occupation'] . "</p>";
?>
</body>
</html>
