<?php

// Inicializacion de variables
$variable			= 0;
$valor         		= 1;
$variable3			= 0;
$id					= 4;

function conexion(){

	$servername = "localhost";
	$username   = "root";
	$password   = "esther";
	$dbname     = "MIBD"; //root

	$conn = new mysqli($servername, $username, $password, $dbname);
	//echo "Connection accepted";
	if (!$conn) {
	die("Connection failed: " . mysqli_connect_error());
	echo "Connection failed";
	}

	return $conn;
}

// Llamada a la funcion "conexion()"
$conn = conexion();

// Llamada a la funcion "recuperar"
$recu = recuperar($id, $conn);

// Creacion de la funcion "recuperar"
function recuperar ($id, $conn) {
	$rep = "SELECT Pupila FROM Tabla WHERE Id=$id";
	$resultado = mysqli_query($conn, $rep);
	if (!$resultado) {
		echo 'No se pudo ejecutar la consulta: ' . mysqli_error();
		exit;
		$variable3=0;
	}
		while ($fila =mysqli_fetch_assoc($resultado)) {
		echo $fila["Pupila"];
		$variable3=1;
	}
	mysqli_close($conn);
	return $variable3;
}


$data= $fila["Pupila"];

?>

<sensor>
	<?php
		echo $data;
	?>
</sensor>
