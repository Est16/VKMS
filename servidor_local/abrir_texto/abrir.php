<?php

$archivo=fopen("archivo.txt", "r")
or die ("problemas al abrir archivo.txt");

while (!feof($archivo))
{
	$traer= fgets($archivo);
	$saltodelinea= n12br($traer);
	echo $saltodelinea;
}


?>
