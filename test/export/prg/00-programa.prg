* 00-Programa.PRG
* la anterior l�nea no es necesaria, pero podr�a ser �til, ya que no hay
* otra forma de traducir "Proceso" y "FinProceso"
* 
* Este ejemplo es simplemente para ver el esqueleto de un programa.
* Por ejemplo, en C++ ver�a la declaraci�n del main y el return 0 al final.
* Tambi�n es para ver c�mo se introducen comentarios en el c�digo.
* 
* Los comentarios en Foxpro/VFP (Visual FoxPro) se pueden definir de tres formas:
* Primera forma: Al comienzo de la l�nea, escribir un asterisco, y as� toda la 
*                l�nea ser� un comentario (( recomendado ))
* Segunda forma: Al comienzo de la l�nea, escribir el comando NOTE, y as� toda la 
*                l�nea ser� un comentario (( no muy usado ))
* Tercera forma: Al final de una l�nea "executable", escribir doble clave de m�sica, 
*                y as� todo el <<resto>> de la l�nea ser� un comentario (( recomendado ))
* 

* Proceso sin_titulo  && no existe algo similar en Fox

    * Hay varias formas de implementar algo de "Hola Mundo"
	
	* lo sgte se muestra en la consola, que es la ventana ppal de VFP (no es exactamente 
	* la consola "negra" de Windows, de DOS, ni de otros lenguajes de programaci�n); 
	* podemos verla como la ventana de edici�n que tienes los editores (blanca, y se ve 
	* atr�s de las ventanas ejecutables, pero no se edita en ella, s�lo se ven resultados 
	* de operaciones)
	? "Hola Mundo"
	* Las instrucciones tampoco tiene un carater "obligatorio" de terminaci�n.
	* Sin embargo si una instrucci�n no cabe en la misma l�nea, se usa el punto y coma 
	* para indicarle al motor que la instrucci�n sigue en la sigte l�nea (probalmente 
	* �sto no lo necesites)
	
	@07,10 say "Hola, Hern�n Cano"  && �ste es mi nombre
	* lo anterior muestra la frase en la fila 10, en la columna siete
	
* FinProceso          && no existe algo similar en Fox
