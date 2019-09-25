<?php
/* Prima i sa parametrima i body sa form-data */
$link = mysqli_connect("localhost", "phpmyadmin", "boza272zabo", "phpmyadmin");
 
if($link === false){
    die("ERROR: Could not connect. " . mysqli_connect_error());
}
 
$ime = mysqli_real_escape_string($link, $_REQUEST['ime']);
$prezime = mysqli_real_escape_string($link, $_REQUEST['prezime']);

$sql = "INSERT INTO test (ime, prezime) VALUES ('$ime', '$prezime')";
if(mysqli_query($link, $sql)){
			header('Content-type: application/json');
            $message = array(); 
            $message['error'] = false; 
            $message['message'] = 'User created successfully';
            echo json_encode($message);
} else{
			header('Content-type: application/json');
            $message = array(); 
            $message['error'] = true; 
            $message['message'] = 'Error user created';
            echo json_encode($message);
}
 
mysqli_close($link);
?>