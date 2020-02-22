<?php
	include("connect.php");
	$link=Connection();
	$result=mysqli_query($link,"SELECT * FROM batLog ORDER BY timeStamp DESC");
?>

<html>
   <head>
      <title>Battery Data</title>
   </head>
<body>
   <h1>Voltage Analog Read</h1>

   <table border="1" cellspacing="1" cellpadding="1">
		<tr>
			<td>&nbsp;Time&nbsp;</td>
			<td>&nbsp;Voltage1&nbsp;</td>
                        <td>&nbsp;Current1&nbsp;</td>
                        <td>&nbsp;Current2&nbsp;</td>
                        <td>&nbsp;Current3&nbsp;</td>
                        <td>&nbsp;Current4&nbsp;</td>
						<td>&nbsp;Current5&nbsp;</td>
                        <td>&nbsp;Current6&nbsp;</td>
                        <td>&nbsp;Current7&nbsp;</td>
                        <td>&nbsp;Current8&nbsp;</td>
			<td>&nbsp;VelKmph&nbsp;</td>
			<td>&nbsp;LAT&nbsp;</td>
			<td>&nbsp;LON&nbsp;</td>
		</tr>

      <?php
		  if($result!==FALSE){
		     while($row = mysqli_fetch_array($result)) {
		        printf("<tr><td> &nbsp;%s </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td></tr>",
		           $row["timeStamp"], $row["batV1"], $row["batA1"], $row["batA2"], $row["batA3"], $row["batA4"],$row["batA5"], $row["batA6"], $row["batA7"], $row["batA8"], $row["vel"], $row["flat"], $row["flon"]);
		     }
		     mysqli_free_result($result);
		     mysqli_close();
		  }
      ?>

   </table>
</body>
</html>
