<!DOCTYPE html>
<html>
<head>
<title>recursion</title>
</head>
<body>
<?php
 
$folderPath = "..";
 
function readFolder( $path ) {
  if ( !( $dir = opendir( $path ) ) ) 
	  die( "Can't open $path" );
  
  $filenames = array();
  
  while ( $filename = readdir( $dir ) ) {
    if ( $filename != '.' && $filename != '..' ) {
      if ( is_dir( "$path/$filename" ) ) 
		  $filename .= '/';
	
	$filenames[] = $filename;
    }
  }
  closedir ( $dir );
  // Sort the filenames in alphabetical order
  sort( $filenames );
  
  // Display the filenames, and process any subfolders
  echo "<ul>";
  foreach ( $filenames as $filename ) {
    echo "<li>$filename";
    if ( substr( $filename, -1 ) == '/' ) 
		readFolder( "$path/" . substr( $filename, 0, -1 ) );
    echo "</li>";
  }
  echo "</ul>";
}
echo "<h2>Contents of '$folderPath':</h2>";
readFolder( $folderPath );
?>

</body>
</html>
