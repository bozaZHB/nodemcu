<?php
/* Mora preko parametara*/
$link = mysqli_connect("localhost", "phpmyadmin", "boza272zabo", "phpmyadmin");
 
// Check connection
if($link === false){
    die("ERROR: Could not connect. " . mysqli_connect_error());
}
 
// Escape user inputs for security
$id = mysqli_real_escape_string($link, $_REQUEST['id']);
$ime = mysqli_real_escape_string($link, $_REQUEST['ime']);
$prezime = mysqli_real_escape_string($link, $_REQUEST['prezime']);

// Attempt insert query execution
$sql = "UPDATE `test` SET `prezime`='$prezime' WHERE ime = '$ime'";

if($result = mysqli_query($link, $sql)){

	$sql = "SELECT * FROM `test` WHERE ime = '$ime'";
	
	$result = mysqli_query($link, $sql);
	
	$row = mysqli_fetch_array($result);
	$user = array(); 
	$user['id'] = $row['id']; 
	$user['ime']=$row['ime']; 
	$user['prezime'] = $row['prezime']; 

	$response_data = array();
	$response_data['error']=false; 
	$response_data['message'] = 'Successful';
	$response_data['user']=$user; 
	header('Content-type: application/json');
	echo json_encode($response_data);

	mysqli_free_result($result);

} else{
    echo "ERROR: Could not able to execute $sql. " . mysqli_error($link);
}

 
mysqli_close($link);
?>