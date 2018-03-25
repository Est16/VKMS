<?php

/////////////////////////////////////////////////////////////
// 				  DECLARACIÓN DE VARIABLES				   //
/////////////////////////////////////////////////////////////

$variable			= 0;
$c					= 0;

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

/////////////////////////////////////////////////////////////
// OBTENER EL VECTOR CONTENIDO EN EL ARCHIVO "archivo-yml" //
/////////////////////////////////////////////////////////////

// Simula la obtención de los eventos del teclado desde un archivo

$líneas = file('archivo.yml');


foreach ($líneas as $num_línea => $línea) {
	$array[$c] = $línea;
	$c= $c + 1;
    //echo "Línea #<b>{$num_línea}</b> : " . htmlspecialchars($línea) . "<br />\n";
}

$valor 			= $array[0];

// Llamada a la funcion "conexion()"
$conn = conexion();

// Llamada a la funcion “insertar()”
$variable = insertar($valor, $conn);

/////////////////////////////////////////////////////////////
// 		FUNCIÓN DE INSERTAR DATOS A LA BASE DE DATOS 	   //
/////////////////////////////////////////////////////////////

// Inserta los eventos de teclado ($valor) a la columna "Teclado"
// de la base de datos con el Id especificado

function insertar($valor, $conn){
	$ins = "INSERT INTO Tabla (Teclado)
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
