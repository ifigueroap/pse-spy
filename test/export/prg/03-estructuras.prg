* Este ejemplo es para observar las estructuras condicionales y 
* repetitivas m�s simples

** 03-Estructuras.prg

	clear

	*c<-0
	*Mientras c<=10 Hacer
	*	c = c+1
	*	Escribir c
	*FinMientras
	c = 0
	do while c = 10
		c = c + 1
		? c
	enddo
	
	*c = 0
	*Repetir     NO EXISTE EN FOXPRO/VFP, pero creo que se puede reemplazar por el Mientras
	*	c = c+1
	*	Escribir c
	*Hasta que c=10
	
	*c = 0
	*Repetir     NO EXISTE EN FOXPRO/VFP, pero creo que se puede reemplazar por el Mientras
	*	c = c+1
	*	Escribir c
	*Mientras que c<10
	
	*si c=10 Entonces
	*	Escribir "Si"
	*FinSi
	if c=10 then   && la part�cula "then" puede omitirse (me parece que en seudoc�digo s� se puede usar)
		? "Si"
	endif
	
	if c=10 then   && la part�cula "then" puede omitirse
		? "Si"
	else
		? "No"
	endif
	
**