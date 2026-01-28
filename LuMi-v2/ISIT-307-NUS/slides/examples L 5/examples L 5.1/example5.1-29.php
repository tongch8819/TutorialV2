<!DOCTYPE html>
<html>
<head>
<title>classes</title>
</head>
<body>
<h1>classes</h1>
<?php
    class BankAccount {
        private $balance;
        
        function __construct($bal =0){
            $this->balance = $bal;
        }
        public function setBalance($newValue) {
              if ($newValue >0)
                  $this->balance = $newValue;
        }
        public function getBalance() {
              return $this->balance;
        }
    }
    
    $checking = new BankAccount();
    $checking->setBalance(100);
    echo "<p>Your checking account balance is " . $checking->getBalance() . "</p>\n";
    $newCh = new BankAccount();
    $newCh->setBalance(-50);
    echo "<p>Your checking account balance is " . $newCh->getBalance() . "</p>\n";
    $newCh = new BankAccount(150);
    echo "<p>Your checking account balance is " . $newCh->getBalance() . "</p>\n";

?>

</body>
</html>
