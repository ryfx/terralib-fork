<?
/*
	Desenha o mapa de distritos de Sao Paulo agrupado em 5 classes pelo atributo da tabela externa: num_prof 
	e exibe a legenda desse agrupamento.
 */
	$t = terraweb();
	
	if(!$t->connect("localhost", "postgres", "", "terraweb320", 5432, 2))
	{
		echo("Nao foi possivel conectar-se ao banco de dados<BR>");
		echo($t->errorMessage());
		exit();
	}
	
	if($t->setCurrentView("sp") == false)
	{
		echo($t->errorMessage() . "<BR>");
		exit();
	}
	
	$box = $t->getcurrentviewbox();
	
	if($box == false)
	{
		echo($t->errorMessage() . "<BR>");
		exit();
	}
	
	$t->setCanvasBackgroundColor(255,255,255);
	
	$t->setWorld($box[0], $box[1], $box[2], $box[3], 800, 600);
	
	$t->setTheme("distritos_sp", 0);
	
	$rampColors = array("RED", "GREEN", "BLUE");
	
	$result = $t->drawCurrentTheme("", "num_prof", "", 5, 1, $rampColors, 2, 0);
	
	if($result == false)
	{
		echo($t->errorMessage() . "<BR>");
		exit();
    }
	
	$legenda["distritos(Quantil - num_prof)"] = $result;
	
	$res = $t->setDefaultVisual(128, 0, 0, 0, 0, 2, "");
	
	if($res == false)
	{
		echo($t->errorMessage() . "<BR>");
		exit();
	}
    
	$res = $t->drawLegend($legenda, 350);
	
	$imagemSaida = $t->getLegendImage(0, 1);
	
	if($imagemSaida == false)
	{
		echo($t->errorMessage() . "<BR>");
		exit();
	}
	
	header ("Content-type: image/png");
	echo($imagemSaida);
?>