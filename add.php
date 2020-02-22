<?php
   	include("connect.php");
   	$link=Connection();

	$batV1=$_POST['batV1'];
	$batA1=$_POST['batA1'];
	$batA2=$_POST['batA2'];
	$batA3=$_POST['batA3'];
	$batA4=$_POST['batA4'];
	$batA1=$_POST['batA5'];
	$batA2=$_POST['batA6'];
	$batA3=$_POST['batA7'];
	$batA4=$_POST['batA8'];
	$vel=$_POST['vel'];
	$flat=$_POST['flat'];
	$flon=$_POST['flon'];

	$query = "INSERT INTO batLog (batV1, batA1, batA2, batA3, batA4,batA5,batA6,batA7,batA8, vel, flat, flon)
		VALUES ('".$batV1."','".$batA1."','".$batA2."','".$batA3."','".$batA4."','".$batA5."','".$batA6."','".$batA7."','".$batA8."','".$vel."','".$flat."','".$flon."')";

   	mysqli_query($link,$query);
	mysqli_close($link);

   	header("Location: index.php");
?>
