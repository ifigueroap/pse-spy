*   
* Este ejemplo es para ver cu�les son las funciones equivalentes
*   a las predefinidas en PSeInt.
*   
* Indicar en cada caso si hay que agregar alg�n #include, import
*   o similar a la cabecera del programa
*   
* Si una funci�n no existe, pero es f�cil de implementar, ofrecer la
*   implementaci�n para introducir cuando sea necesario. Por ejemplo, en
*   C++ no hay funci�n para pasar toda una cadena a may�sculas pero se
*   puede implementar f�cilmente aplicando toupper (que opera sobre un
*   �nico caracter) a cada caracter de la cadena mediante un ciclo for.
*   
* Para las funciones trigonom�tricas, indicar si el argumento se
*   recibe en grados o radianes.
*   
* En algunos casos pueden reemplazarse por expresiones. Por ejemplo,
*   en c++ puedo obtener el truncado casteando a entero, o puedo concatenar
*   strings con el operador +, o necesito aplicarle un mod al resultado de
*   rand para tener un valor m�ximo como en la funci�n Azar. Estos cambios
*   no ameritan declarar una nueva funci�n, ya que pueden hacerse in-place
*   sin generar mucho ruido.
*   
* Ninguna de las funciones de PSeInt modifican el argumento (el pasaje
*   de par�metros es siempre por copia).


** 09-funciones.prg

* En las funciones trigonom�tricas, el argumento se debe enviar en RADIANES.

clear 

	x = 1.5
	? SQRT(x)       && ra�z cuadrada
	? abs(x)        && valor absoluto
	? LOG(x)        && logaritmo natural
	? exp(x)        && exponencial, e^x
	? SIN(x)        && seno
	? cos(x)        && coseno
	? tan(x)        && tangente
	
	xx=.7
	? ASIN(xx)       && arco-seno
	* The value of xx can range from +1 through �1, and the value ASIN() returns 
	*   can range from �pi/2 through +pi/2 ( �1.57079 to 1.57079)
	
	? acos(xx)       && arco-coseno
	* The value of xx can range from +1 through �1, and the value ACOS() returns 
	*   can range from 0 through pi (3.141592).
	
	? atan(x)       && arco-tangente
	? ROUND(x,0)    && redondo a entero
	? INT(x)        && truncamiento a entero
	? RAND(x)       && n�mero al azar entre 0 y x-1 inclusive ---lo lamento: no s� si funciona como necesitas---
	s="Hola mundo!!!"
	? LEN(s)    && cantidad de letras
	? UPPER(s)  && retorna una copia toda en may�sculas  "HOLA MUNDO!!!"
	? LOWER(s)  && retorna una copia toda en min�sculas  "hola mundo!!!"
	? PROPER(s) && retorna una copia con may�scula inicial "Hola Mundo!!!"
	*? SubCadena(s,1,2) && retorna la cadena desde la pos 1 (segunda en base 0) a la 2 (tercera en base 0): "ol"
	? SUBSTR(s,2,3) && retorna la cadena desde la pos 2 a la 3: "ol"
	
	? concatenar(s," Mundo")   && UDF: se debe crear
	
	*? ConvertirANumero("15.5")  && retorna un real (15.5)
	? VAL("15.5")                && retorna un real (15.5)
	*? ConvertirATexto(15.5)     && retorna una cadena ("15.5")
	? STR(15.5)                  && retorna una cadena ("15.5")

**

function Concatenar(P1, P2)
return P1+P2
endfunc


** Este ejemplo muestra un n�mero aleatorio que se encuentra entre 1 y 10:
function EjemploRandom
   gnLower = 1
   gnUpper = 10
   ? INT((gnUpper - gnLower + 1) * RAND( ) + gnLower)
endfunc

**