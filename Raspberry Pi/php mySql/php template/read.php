<?php
/* Attempt MySQL server connection. Assuming you are running MySQL
server with default setting (user 'root' with no password) */
$link = mysqli_connect("localhost", "phpmyadmin", "boza272zabo", "phpmyadmin");
 
// Check connection
if($link === false){
    die("ERROR: Could not connect. " . mysqli_connect_error());
}
 
// inputs 
$day = mysqli_real_escape_string($link, $_REQUEST['day']);
$password = mysqli_real_escape_string($link, $_REQUEST['password']);

// query
$sql = "SELECT * FROM `smart_home_temperature` WHERE date BETWEEN '$day 00:00:00' AND '$day 23:59:59';";

if($result = mysqli_query($link, $sql)){
    if(mysqli_num_rows($result) > 0){

		$users = array();
		while($row = mysqli_fetch_array($result)){
			$user = array();
						
			$user['id'] = $row['id'];
			$user['date']=$row['date'];
			$user['temperature'] = $row['temperature'];
			array_push($users, $user);
		}
				
			
		$response_data = array();
		$response_data['error']=false; 
		$response_data['message'] = 'Login Successful';
		$response_data['user']=$users; 
		header('Content-type: application/json');
		echo json_encode($response_data);

        mysqli_free_result($result);
    } else{
        echo "No records matching your query were found.";
    }
} else{
    echo "ERROR: Could not able to execute $sql. " . mysqli_error($link);
}
 
mysqli_close($link);
?>