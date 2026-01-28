<!DOCTYPE html>
<html>
<head>
<title>classes</title>
</head>
<body>
<h1>classes</h1>
<?php

    class BankAccount1 {
        private float $balance;
        public function __construct(float $balance){
            $this->balance = $balance;
        }
        
        public function getBalance() {
               return $this->balance;
        }
    }

    $checking1 = new BankAccount1(100);
    echo "<p>$", $checking1->getBalance(), "</p>";

//*****
    class BankAccount2 {
        public function __construct(private float $balance){
        }
        
        public function getBalance() {
               return $this->balance;
        }
    }

    $checking2 = new BankAccount2(200);
    echo "<p>$", $checking2->getBalance(), "</p>";

//*****
    class BankAccount3 {
        private string $name;
        public function __construct(private float $balance, string $name){
            $this->name = $name;
        }
        
        public function getInfo() {
               return $this->name . ": $" . $this->balance;
        }
    }

    $checking3 = new BankAccount3(300, "Elena");
    echo "<p>", $checking3->getInfo(), "</p>";

//*****
    class BankAccount4 {
        private string $name;
        public function __construct(private float $balance, string $name, private string $town = "unknown"){
            $this->name = $name;
        }
        
        public function getInfo():string {
               return $this->name . " from " . $this->town . ": $" . $this->balance;
        }
    }

    $checking4 = new BankAccount4(400, "Joe", "Wollongong");
    echo "<p>", $checking4->getInfo(), "</p>";

    $checking5 = new BankAccount4(400, "Will");
    echo "<p>", $checking5->getInfo(), "</p>";

?>

</body>
</html>
