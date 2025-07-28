const char PAGINA_INICIAL[] PROGMEM = R"*****( 
<!DOCTYPE html>
<html>
<title>Leituras atualizadas de temperatura do MAX6675</title>
<head>
<script type = "text/javascript">
	
	setInterval(function() {
		atualizaTemperatura();
	},1000);
	
	function  atualizaTemperatura() {
		let objetoXmlHttpRequest = new XMLHttpRequest();
		objetoXmlHttpRequest.onreadystatechange = function() {
			if (this.readyState == 4 && this.status == 200) {
				document.getElementById("valorTemperatura").innerHTML =  this.responseText;
			}
		};
		objetoXmlHttpRequest.open("GET","atualiza_temperatura",true);
		objetoXmlHttpRequest.send();
	}
	
</script>
</head>

<body onload="atualizaTemperatura()">
<h1> ESP32 + MAX6675</h1>
<div>Temperatura atual: </div>
<div id="valorTemperatura"></div>

</body>




</html>

)*****";