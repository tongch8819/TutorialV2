<!DOCTYPE html>
<html>
<head>
<title>strings</title>
</head>
<body>
<?php
    $Email = "my.email@uow.edu.au";
    echo "<p>if I use strchr - " . strchr($Email, ".") . "</p>";
    echo "<p>if I use strrchr - " . strrchr($Email, ".") . "</p>";
    echo "<p> the @ is at position - " . strpos($Email, "@") . "</p>";
    echo "<p>if I replace the email - " . str_replace("email", "e-mail", $Email) . "</p>";
    
    if (strpos($Email, "m") === FALSE)
        echo "the char is not found";
    else
        echo "the char is at position ", strpos($Email, "m");
?>
</body>
</html>

