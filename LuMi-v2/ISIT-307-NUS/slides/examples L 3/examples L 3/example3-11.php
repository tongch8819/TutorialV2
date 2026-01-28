<!DOCTYPE html>
<html>
<head>
<title>Web</title>
</head>
<body>
<?php
    
    //file_put_contents ("test.txt", "something");
    chmod("test.txt", 0744);
    $perms = fileperms("test.txt");
    $perms = decoct($perms%01000);
    echo $perms;
    
    

  
    
 ?>
</body>
</html>

