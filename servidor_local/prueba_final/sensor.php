<?php

/////////////////////////////////////////////////////////////
// 				  DECLARACIÓN DE VARIABLES				   //
/////////////////////////////////////////////////////////////

$variable			= 0;
$valor         		= 1;
$variable3			= 0;
$id					= 1;

/////////////////////////////////////////////////////////////
// 		  FUNCIÓN DE CONEXIÓN CON LA BASE DE DATOS 		   //
/////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////
// 		FUNCIÓN DE RECUPERAR DATOS A LA BASE DE DATOS 	   //
/////////////////////////////////////////////////////////////

// Recupera los eventos de teclado ($valor) a la columna "Teclado"
// de la base de datos con el Id especificado

function recuperar ($id, $conn) {
	$rep = "SELECT Teclado FROM Tabla WHERE Id=$id";
	$resultado = mysqli_query($conn, $rep);
	if (!$resultado) {
		echo 'No se pudo ejecutar la consulta: ' . mysqli_error();
		exit;
		$variable3=0;
	}
		while ($fila =mysqli_fetch_assoc($resultado)) {
		echo $fila["Teclado"];
		$variable3=1;
	}
	mysqli_close($conn);
	return $variable3;
}


$data= $fila["Teclado"];

?>


<sensor>
	<?php
		echo $data;
	?>
</sensor>
