*   
* Este ejemplo es para ver cuáles son las funciones equivalentes
*   a las predefinidas en PSeInt.
*   
* Indicar en cada caso si hay que agregar algún #include, import
*   o similar a la cabecera del programa
*   
* Si una función no existe, pero es fácil de implementar, ofrecer la
*   implementación para introducir cuando sea necesario. Por ejemplo, en
*   C++ no hay función para pasar toda una cadena a mayúsculas pero se
*   puede implementar fácilmente aplicando toupper (que opera sobre un
*   único caracter) a cada caracter de la cadena mediante un ciclo for.
*   
* Para las funciones trigonométricas, indicar si el argumento se
*   recibe en grados o radianes.
*   
* En algunos casos pueden reemplazarse por expresiones. Por ejemplo,
*   en c++ puedo obtener el truncado casteando a entero, o puedo concatenar
*   strings con el operador +, o necesito aplicarle un mod al resultado de
*   rand para tener un valor máximo como en la función Azar. Estos cambios
*   no ameritan declarar una nueva función, ya que pueden hacerse in-place
*   sin generar mucho ruido.
*   
* Ninguna de las funciones de PSeInt modifican el argumento (el pasaje
*   de parámetros es siempre por copia).


** 09-funciones.prg

* En las funciones trigonométricas, el argumento se debe enviar en RADIANES.

clear 

	x = 1.5
	? SQRT(x)       && raíz cuadrada
	? abs(x)        && valor absoluto
	? LOG(x)        && logaritmo natural
	? exp(x)        && exponencial, e^x
	? SIN(x)        && seno
	? cos(x)        && coseno
	? tan(x)        && tangente
	
	xx=.7
	? ASIN(xx)       && arco-seno
	* The value of xx can range from +1 through –1, and the value ASIN() returns 
	*   can range from –pi/2 through +pi/2 ( –1.57079 to 1.57079)
	
	? acos(xx)       && arco-coseno
	* The value of xx can range from +1 through –1, and the value ACOS() returns 
	*   can range from 0 through pi (3.141592).
	
	? atan(x)       && arco-tangente
	? ROUND(x,0)    && redondo a entero
	? INT(x)        && truncamiento a entero
	? RAND(x)       && número al azar entre 0 y x-1 inclusive ---lo lamento: no sé si funciona como necesitas---
	s="Hola mundo!!!"
	? LEN(s)    && cantidad de letras
	? UPPER(s)  && retorna una copia toda en mayúsculas  "HOLA MUNDO!!!"
	? LOWER(s)  && retorna una copia toda en minúsculas  "hola mundo!!!"
	? PROPER(s) && retorna una copia con mayúscula inicial "Hola Mundo!!!"
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


** Este ejemplo muestra un número aleatorio que se encuentra entre 1 y 10:
function EjemploRandom
   gnLower = 1
   gnUpper = 10
   ? INT((gnUpper - gnLower + 1) * RAND( ) + gnLower)
endfunc

**