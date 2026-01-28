<!DOCTYPE html>
<html>
<head>
<title>Inheritance</title>
</head>
<body>
<?php
//The parent class
class Car {
  // Private property inside the class
  protected $model="";
  //Public setter method
  public function setModel($model)
  {
    $this->model = $model;
  }
  public function hello()
  {
    return "I am a <i>" . $this->model . "</i><br />";
  }
}
//The child class inherits the code from the parent class
class SportsCar extends Car{
 
  private $style = 'fast and furious';
  
  public function driveItWithStyle()
  {
    return $this->hello() . 'Drive me ' . '<i>' . $this->style . '</i>';
  }
}
//create an instance from the child class
$sportsCar1 = new SportsCar();  
// Use a method that the child class inherited from the parent class
$sportsCar1->setModel('Ferrari');
// Use a method that was added to the child class
echo $sportsCar1->driveItWithStyle();
?>
</body>
</html>
