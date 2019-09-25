<?php
/* Attempt MySQL server connection. Assuming you are running MySQL
server with default setting (user 'root' with no password) */
$link = mysqli_connect("localhost", "phpmyadmin", "boza272zabo", "phpmyadmin");
 
// Check connection
if($link === false){
    die("ERROR: Could not connect. " . mysqli_connect_error());
}
 
// Escape user inputs for security
$id = mysqli_real_escape_string($link, $_REQUEST['id']);
$email = mysqli_real_escape_string($link, $_REQUEST['email']);
$name = mysqli_real_escape_string($link, $_REQUEST['name']);
$school = mysqli_real_escape_string($link, $_REQUEST['school']);

// Attempt insert query execution
$sql = "UPDATE `users` SET `email`='$email',`name`='$name',`school`='$school' WHERE id = '$id'";

if($result = mysqli_query($link, $sql)){

	$sql = "SELECT * FROM `users` WHERE id = '$id'";
	
	$result = mysqli_query($link, $sql);
	
	$row = mysqli_fetch_array($result);
	$user = array(); 
	$user['id'] = $row['id']; 
	$user['email']=$row['email']; 
	$user['name'] = $row['name']; 
	$user['school'] = $row['school']; 

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