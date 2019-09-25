<?php
/* Mora preko parametara*/
$link = mysqli_connect("localhost", "phpmyadmin", "boza272zabo", "phpmyadmin");
 
// Check connection
if($link === false){
    die("ERROR: Could not connect. " . mysqli_connect_error());
}

$id = mysqli_real_escape_string($link, $_REQUEST['id']);

// Attempt insert query execution
$sql = "DELETE FROM `test` WHERE id = '$id'";

if($result = mysqli_query($link, $sql)){
	$response_data = array();
	$response_data['error']=false; 
	$response_data['message'] = 'Successful deleted';
	header('Content-type: application/json');
	echo json_encode($response_data);

	mysqli_free_result($result);

} else{
    echo "ERROR: Could not able to execute $sql. " . mysqli_error($link);
}

 
mysqli_close($link);
?>