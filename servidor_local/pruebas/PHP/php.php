<?php

/*
// poner el contenido de un fichero en una cadena
$nombre_fichero = "archivo.yml";
$gestor = fopen($nombre_fichero, "r");
$contenido = fread($gestor, filesize($nombre_fichero));
fclose($gestor);

echo $contenido;
*/

$líneas = file('archivo.yml');

/*
$array = getArray();

function getArray() {
    return array(0, 0, 0, 0, 0);
}
var_dump($array);
* */
$c=0;


// Recorrer nuestro array, mostrar el código fuente HTML como tal y mostrar tambíen los números de línea.
foreach ($líneas as $num_línea => $línea) {
	$array[$c] = $línea;
	$c= $c + 1;
    echo "Línea #<b>{$num_línea}</b> : " . htmlspecialchars($línea) . "<br />\n";
}



//$data= $array[5];
//echo $c;
echo $array[4];




//var_dump($array);

/*
$array = array(
         "a",
         "b",
    6 => "c",
         "d",
		);
var_dump($array);
*/
?>
