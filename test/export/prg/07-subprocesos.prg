* Este ejemplo es para ver la forma de declarar y utilizar subprocesos/funciones.
* Aclarar si hay diferencia entre subprocesos/funciones que no
*    retornen nada, y los que sí retornen algún resultado.
* Las dos primeras reciben argumentos por copia, las tres ultimas
*   por referencia (en PSeInt, cuando no se indica se utiliza por copia,
*   menos los arreglos que siempre pasan por referencia)
* Indicar las consideraciones especiales que se deban tener en cuenta
*   al pasar arreglos.
* Si es necesario hacer fordware declarations de las funciones
*   cuando no se implementan en orden, indicarlo y ejemplificarlo.
	
* Sí hay diferencia entre subprocesos/funciones que sí retornen algún resultado y los que no,
*    la diferecia está en el RETURN final.

* En Fox/VFP no hay diferencia primordial entre PROCEDURE y FUNCTION, pero mi recomendación es:
* Usar (a) PROCEDURE cuando no devuelva valor (sólo ejecuta instrucciones)
*    y (b) FUNCTION cuando sí devuelva valor,
* Y recibir los parámetros con "parameters"
* 
* Para ejecutar cualq de las dos recomiendo:
*    NombreFuncion() si devuelve valor, y si no devuelve valor "do NombreFuncion with ...." 
*     o "do NombreFuncion" si no usa parámetros.
* Puedes simplificar ésto a NombreFuncion(--) con o sin parámetros

* En el orden en que se envíen los parámetros, en ese mismo orden se reciben
	
** 07-SubProcesos.prg

    clear
	
	* Escribir FuncionDoble(5)
	? FuncionDoble(5)
	@01,05 say 'FuncionDoble(5)'
	@02,05 say FuncionDoble(5)
	
?'NoRetornaNada'
	NoRetornaNada(3,9)    && estas tres formas de ejecutar son válidas en Fox/VFP
	=NoRetornaNada(3,9)   && estas tres formas de ejecutar son válidas en Fox/VFP
	do NoRetornaNada with 3,9 && estas tres formas de ejecutar son válidas en Fox/VFP
	
	*c=0
	*PorReferencia(c)
	*Escribir c

* Por defecto, las variables y matrices pasan a las funciones definidas por el usuario en términos de valor.
* Por lo tanto, los cambios realizados en la función de las variables y los arreglos pasados no se devuelven 
*   al programa de llamada. 
* Sin embargo, puede pasar variables y matrices por referencia anteponiéndoles el signo de arroba (@), así:
	c=0
?'PorReferencia(@c)'
	PorReferencia(@c)
	? c
	
	Dimension a[10],b[3,4]
	a=0     && de esta forma se pueden inicializar todos y cada uno de los elementos de la matriz/vector
	b=0     && en Fox/VFP es necesario inicializar antes de "usarlos"
	RecibeVector(@a)  && dado que el vector cambiará dentro de la función, debemos pasarlo "por referencia"
	
	RecibeMatriz(@b)
	@10,01 say "RecibeMatriz(@b)"
	for k=1 to 12   && la matriz b es bidimensional, pero se puede utilizar un sólo índice para recorrerlo
	   ?? b(k),'-'  && sin embargo ésto quizá no sea necesario saberlo para seudocódigo
	next
	?
	
** FinProceso

function FuncionDoble(b)
	a=2*b
	return a  && cuando devuelve valor, no puede omitirse esta línea RETURN
endfunc  && no es necesaria, pero quizá para seudocodigo sí
function FuncionDoble   && ES LA QUE YO TE RECOMENDARIA para cuando devuelve valor
parameters b
	a=2*b
	return a  && cuando devuelve valor, no puede omitirse
endfunc
function FuncionDoble
lparameters b
	a=2*b
	return a  && cuando devuelve valor, no puede omitirse
endfunc

procedure NoRetornaNada
parameters a,b
	? a+b
	return    && puede omitirse pues no devuelve valor
endproc

procedure PorReferencia
parameters b
	b=7
	return    && puede omitirse pues no devuelve valor
endproc

procedure RecibeVector(v)
	* Leer v[1]
	@02,40 say "Escriba el valor de v[1]: " get v[1]
	read
	return    && puede omitirse pues no devuelve valor
endproc

procedure RecibeMatriz(m)
	*Leer m[1,1]
	@04,40 say "Escriba el valor de m[1,1]: " get m[1,1]
	read
	return    && puede omitirse pues no devuelve valor
endproc

**