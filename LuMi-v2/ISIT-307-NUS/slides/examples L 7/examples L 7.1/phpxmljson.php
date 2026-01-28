<!DOCTYPE html>
<html>
<head>
<title>PHP XML</title>
</head>
<body>
<?php
$myXMLstr =
"<aaaa Version='1.0'>
   <bbb>
      <cccc>
        <dddd id='id-pass' />
		<eeee name='hearman' age='24'  />
      </cccc>
    </bbb>
</aaaa>";

if (($xml=simplexml_load_string($myXMLstr))){
    echo "<pre>\n";
    print_r($xml);
    echo "</pre>\n";
    
    $json = json_encode($xml);
	echo "<pre>\n";
	print_r($json);
	echo "</pre>\n";
	
	$array = json_decode($json, true);
	echo "<pre>\n";
	print_r($array);
	echo "</pre>\n";
} 
?>
</body>
</html>
