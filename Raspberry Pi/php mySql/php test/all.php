<?php
/* Attempt MySQL server connection. Assuming you are running MySQL
server with default setting (user 'root' with no password) */
$link = mysqli_connect("localhost", "phpmyadmin", "boza272zabo", "phpmyadmin");
 
// Check connection
if($link === false){
    die("ERROR: Could not connect. " . mysqli_connect_error());
}
 
// Escape user inputs for security
$email = mysqli_real_escape_string($link, $_REQUEST['email']);
$password = mysqli_real_escape_string($link, $_REQUEST['password']);

// Attempt insert query execution
$sql = "SELECT * FROM `test`";

if($result = mysqli_query($link, $sql)){
    if(mysqli_num_rows($result) > 0){

            $users = array();
                while($row = mysqli_fetch_array($result)){
                        $user = array();
						
                        $user['id'] = $row['id'];
                        $user['ime']=$row['ime'];
                        $user['prezime'] = $row['prezime'];
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