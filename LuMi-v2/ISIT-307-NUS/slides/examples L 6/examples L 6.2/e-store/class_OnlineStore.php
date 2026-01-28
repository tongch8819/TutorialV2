<?php
class OnlineStore {
	private $conn = NULL;
	private $storeID = "";
	private $inventory = array();
	private $shoppingCart = array();
	function __construct() {
		include("inc_OnlineStoreDB.php");
		$this->conn = $conn;
	}
	
	public function setStoreID($storeID) {
		if ($this->storeID != $storeID) {
			$this->storeID = $storeID;
            try {
                $sql = "SELECT * FROM inventory where storeID = '" . $this->storeID . "'";
                $qRes = $this->conn->query($sql);
				$this->inventory = array();
				$this->shoppingCart = array();
				while (($Row = $qRes->fetch_assoc())!== NULL) {
					$this->inventory[$Row['productID']]= array();
					$this->inventory[$Row['productID']]['name']= $Row['name'];
					$this->inventory[$Row['productID']]['description']= $Row['description'];
					$this->inventory[$Row['productID']]['price']= $Row['price'];
					$this->shoppingCart[$Row['productID']]= 0;
				}
			}
            catch (mysqli_sql_exception $e){
                $this->storeID = "";
            }
		}
	}
	
	public function getStoreInformation() {
		$retval = FALSE;
		if ($this->storeID != "") {
            try {
                $sql = "SELECT * FROM store_info where storeID = '" . $this->storeID . "'";
                $qRes = $this->conn->query($sql);
				$retval = $qRes->fetch_assoc();
			}
            catch (mysqli_sql_exception $e){
            }
		}
		return($retval);
	}
	
	public function getProductList() {
		$retval = FALSE;
		$subtotal = 0;
		if (count($this->inventory) > 0) {
			echo "<table width='100%'>\n";
			echo "<tr><th>Product</th><th>Description</th><th>Price Each</th><th># in Cart</th><th>Total Price</th><th>&nbsp;</th></tr>\n";
			foreach ($this->inventory as $ID => $Info) {
				echo "<tr><td>" . htmlentities($Info['name']) . "</td>\n";
				echo "<td>" . htmlentities($Info['description']) . "</td>\n";
				echo "<td class='currency'>\${$Info['price']}</td>\n";
				echo "<td class='currency'>" . $this->shoppingCart[$ID] . "</td>\n";
				$value = $Info['price']*$this->shoppingCart[$ID];
				echo"<td class='currency'>\${$value}</td>\n";
				echo "<td><a href='" . $_SERVER['SCRIPT_NAME'] . "?PHPSESSID=" . session_id() . "&ItemToAdd=$ID'>Add " . " Item</a></td>\n"; 
				$subtotal += ($Info['price'] * $this->shoppingCart[$ID]);
			}
			echo "<tr><td colspan='4'>Subtotal</td>\n";
			echo "<td class='currency'>\${$subtotal}</td>\n";
			echo "<td>&nbsp;</td></tr>\n";
			echo "</table>";
			$retval = TRUE;
		}
		return($retval);
	}

	public function addItem() {
		$ProdID = $_GET['ItemToAdd'];
		if (array_key_exists($ProdID, $this->shoppingCart))
			$this->shoppingCart[$ProdID] += 1;
	}
	
	function __wakeup() {
		include("inc_OnlineStoreDB.php");
		$this->conn = $conn;
	}

	function __destruct() {
			$this->conn->close();
	}
}
?>
