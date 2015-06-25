* Este ejemplo es para ver c�mo trabajan los operadores.
* Si hay operadores que no est�n en el lenguaje, o que no aplican
*   para todos los tipos, indicarlo y utilizar si es posible alguna
*   funci�n est�ndar que lo reemplace (por ejemplo, pow en c++ en lugar
*   de ^).
* Si se utilizan bibliotecas, aclarar como se incluyen las bibliotecas
*   en ese lenguaje (#include, import, etc).
* Al final se repiten los operadores, pero aplicados a cadenas de 
*   caracteres, ya que en algunos lenguajes las comparaciones se realizan
*   de forma especial, y ademas el + tambi�n tiene un significado 
*   especial (concatenar). 

** 08-Operadores.prg

clear 

	a=11
	b=2
	c=.t.   && verdadero
	d=.f.   && Falso
	e="Hola" 
	f="Mundo"
	
	* algebr�icos
	? a+b
	? a*b
	? a/b
	? a-b
	? a^b  &&  potencia
	? a%b  &&  resto de la divisi�n entera
	
	?
	* relacionales (comparaciones)
	? a=b
	? a<>b  &&  distinto       
	?a#b       
	?!a=b
	? a<b 
	? a>b 
	? a<=b 
	? a>=b 
	
	?
	* l�gicos
	? c .and. d
	? c .or. d 
	? .not. (c .and. d)
	
	?
	* para cadenas
	? e+" "+f  &&  el + para cadenas las concatena
	? e=f
	? e<>f
	? e<f 
	? e>f 
	? e<=f 
	? e>=f 

**