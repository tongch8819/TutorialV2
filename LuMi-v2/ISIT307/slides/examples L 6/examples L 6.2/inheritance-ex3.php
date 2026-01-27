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
  protected $model="";  //try with private
 
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
 
  public function setStyle($style)
  {
      $this->style = $style;
  }
  public function driveItWithStyle()
  {
    return 'I am ' . $this->model . '! Drive me ' . '<i>' .  $this->style . '</i> <br />';
  }
  public function hello()
  {
      return "I am <i>overriden</i> method of the parent one";
  }

}
 
echo "--------sportsCar1 <br>";
//create an instance from the child class
$sportsCar1 = new SportsCar();
   
// Use a method that the child class inherited from the parent class
$sportsCar1->setModel('Ferrari');
  
// Use a method that was added to the child class
echo $sportsCar1->driveItWithStyle();

// Use a method that is overriden
echo $sportsCar1->hello();

echo "<br>---------car1 <br>";
//create an instance from the parent class
$car1 = new Car();
$car1->setModel('Honda');
echo $car1->hello();
    
echo "<br>--------sportsCar2 <br>";
$car2 = new SportsCar();
$car2->setModel('Mazda');
$car2->setStyle('nice and slow');
echo $car2->driveItWithStyle();
echo $car2->hello();


?>
</body>
</html>
