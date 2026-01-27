<!DOCTYPE html>
<html>
<head>
<title>classes</title>
</head>
<body>
<h1>classes</h1>
<?php
    class MyClass{
        private int $myP=0;
        private int $second=2;
        
        function __get($name){
            return $this->$name;}
        
        function __set($name, $value){
            if ($name=="myP")
                $this->$name = $value*2;
            else
                $this->$name = $value; }
        
    }
    
    
    $myV = new MyClass();
    echo "myP = " . $myV->myP . "<br />";
    $myV->myP = 50;
    echo "myP = " . $myV->myP . "<br />";
    
    echo "second = " . $myV->second . "<br />";
    $myV->second = 20;
    echo "second = " . $myV->second . "<br />";

    $myV->newone = 5;
    echo "newone = " . $myV->newone . "<br />";
?>

</body>
</html>
