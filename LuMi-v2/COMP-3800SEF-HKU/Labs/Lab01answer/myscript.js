var numType = 3;
var types = ["adult", "children", "elderly"];
var prices = [200, 100, 50];
var totals = [0, 0, 0];

var total = document.getElementById("total");
var errMsg = document.getElementById("error_msg");

function fn(t) {
	totals[t] = parseInt(document.getElementById(types[t]).value) * prices[t];
	document.getElementById(types[t] + "total").innerHTML = totals[t];
	total.innerHTML = totals[0] + totals[1] + totals[2];
	errMsg.innerHTML = "";		
}

document.getElementById("adult").addEventListener("input", function() {fn(0);});
document.getElementById("children").addEventListener("input", function() {fn(1);});
document.getElementById("elderly").addEventListener("input", function() {fn(2);});

document.forms[0].addEventListener("reset", resetHandler);

function resetHandler(evt) {
	for (t = 0; t < numType; t++) {
		totals[t] = 0;
		document.getElementById(types[t] + "total").innerHTML = 0;
	}
	total.innerHTML = 0;
	errMsg.innerHTML = "";
}

function validator(){
	if (totals[0] + totals[1] + totals[2] == 0) {
		errMsg.innerHTML = "Error: Total number of tickets cannot be 0.";
		return false;
	} else
		return true;
}