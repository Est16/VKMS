<?php

// Inicializacion de variables
$variable			= 0;
$valor         		= 222;
$variable3			= 0;
$id					= 2;
//$valor= $_GET['pupila'];


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

// Llamada a la funcion “insertar()”
$variable = insertar($valor, $conn);

// Creacion de la funcion "insertar()"
function insertar($valor, $conn){
	$ins = "INSERT INTO Tabla (Pupila)
	VALUES ($valor)";
	if (mysqli_query($conn, $ins)) {
		$variable=1;
		echo "Si";
	}
	else{
		$variable=0;
		echo "No";
	}
	mysqli_close($conn);
	return $variable;
}


?>
