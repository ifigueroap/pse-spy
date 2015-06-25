* 00-Programa.PRG
* la anterior línea no es necesaria, pero podría ser útil, ya que no hay
* otra forma de traducir "Proceso" y "FinProceso"
* 
* Este ejemplo es simplemente para ver el esqueleto de un programa.
* Por ejemplo, en C++ vería la declaración del main y el return 0 al final.
* También es para ver cómo se introducen comentarios en el código.
* 
* Los comentarios en Foxpro/VFP (Visual FoxPro) se pueden definir de tres formas:
* Primera forma: Al comienzo de la línea, escribir un asterisco, y así toda la 
*                línea será un comentario (( recomendado ))
* Segunda forma: Al comienzo de la línea, escribir el comando NOTE, y así toda la 
*                línea será un comentario (( no muy usado ))
* Tercera forma: Al final de una línea "executable", escribir doble clave de música, 
*                y así todo el <<resto>> de la línea será un comentario (( recomendado ))
* 

* Proceso sin_titulo  && no existe algo similar en Fox

    * Hay varias formas de implementar algo de "Hola Mundo"
	
	* lo sgte se muestra en la consola, que es la ventana ppal de VFP (no es exactamente 
	* la consola "negra" de Windows, de DOS, ni de otros lenguajes de programación); 
	* podemos verla como la ventana de edición que tienes los editores (blanca, y se ve 
	* atrás de las ventanas ejecutables, pero no se edita en ella, sólo se ven resultados 
	* de operaciones)
	? "Hola Mundo"
	* Las instrucciones tampoco tiene un carater "obligatorio" de terminación.
	* Sin embargo si una instrucción no cabe en la misma línea, se usa el punto y coma 
	* para indicarle al motor que la instrucción sigue en la sigte línea (probalmente 
	* ésto no lo necesites)
	
	@07,10 say "Hola, Hernán Cano"  && éste es mi nombre
	* lo anterior muestra la frase en la fila 10, en la columna siete
	
* FinProceso          && no existe algo similar en Fox
