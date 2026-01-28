<!DOCTYPE html>
<html>
<head>
<title>Stack</title>
</head>
<body>
<?php
/*
class Stack {
  public $stack;
  public $top;
 
  function __construct() {
    $this->stack = array();
    $this->top = -1;
  }
 
  //add new element
  public function push($item) {
    //$this->stack[] = $item;
    $this->top++;
    array_push($this->stack, $item);
    echo $item." is added into the stack. <br />";
  }
 
  //delete top element
  public function pop() {
    if($this->top < 0){
      echo "Stack is empty. <br />";
    } else {
      //$item = $this->stack[$this->top];
      //unset($this->stack[$this->top]);
      $item = array_pop($this->stack);
      $this->top--;
      
      echo $item." is deleted from the stack. <br />";
    }
  }
 
  public function topElement() {
    if($this->top < 0) {
      echo "Stack is empty. <br />";
    } else {
      return $this->stack[$this->top];
    }
  }

  // is stack empty or not
  public function isEmpty() {
    if($this->top == -1) {
        echo "Stack is empty. <br />";
    } else {
        echo "Stack is not empty. <br />";
    }
  }
   
  //size of the stack
  public function size() {
    return count($this->stack);
    //return $this->top+1;
  }
  
  //pront all elements
  public function printStack(){
    echo "current stack: <br />";
    print_r($this->stack);
    echo "end of the stack <br />";
  }
}
 

$mySt = new Stack();
$mySt->isEmpty();

$mySt->push(5);
$mySt->push(-23);
$mySt->push(40.5);
$mySt->push(22);
 
$mySt->printStack();

$mySt->pop();

$mySt->printStack();
$mySt->push(10);
$mySt->printStack();
$mySt->isEmpty();
echo "stack size: " . $mySt->size();
 */
?>

<?php

echo "<br><br><p>with SPL </p>";
$q = new SplStack();
$q[] = 1;
$q[] = 2;
$q[] = 3;
$q->push(4);
$q->push(5);
$q->rewind(); //Rewind iterator back to the start
while($q->valid()){
    echo $q->current(),", ";
    $q->next();
}
echo "<p>" . $q->pop() . "</p>";
$q->rewind();
while($q->valid()){
    echo $q->current(),", ";
    $q->next();
}

?>

</body>
</html>
