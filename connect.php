<?php
	function Connection(){
		$server="localhost";
		$user="dbEddy";
		$pass="pass";
		$db="dbVoltBts";

		$connection = mysqli_connect($server, $user, $pass);

		if (!$connection) {
	    	die('MySQL ERROR: ' . mysqli_error());
		}

		mysqli_select_db($connection, $db) or die( 'MySQL ERROR: '. mysqli_error() );

		return $connection;
	}
?>
