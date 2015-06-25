* Este ejemplo es para observar la estructura "segun":
* Aclarar de qué tipo puede ser o no ser la variable de control
* (por ejemplo, en C++ c solo podría ser numérica)
* Aclarar si es obligatorio o no colocar la sección por defecto
* (la correspondiente al "De Otro Modo:").

** 06-Segun.prg

	*Leer c
	*segun c hacer
	*	1: 
	*		Escribir "1"
	*	3,4,9: 
	*		Escribir "3, 5 o 9"
	*	7:
	*		Escribir "7"
	*	De otro modo:
	*		Escribir "ni 1, ni 3, ni 5, ni 9, ni 7"
	*FinSegun
	
	* Leer c
	clear
	C=0
	@02,10 say "Escriba el valor de C: " get C
	read
	@05,10 say ""

	* segun c hacer
	do case
	case C=1
	   ? "1"
	case inlist(C,3,4,9)
	   ? "3, 5 ó 9"
	case C=7
	   ? "7"
	otherwise   && no es obligatoria
	   ? "ni 1, ni 3, ni 5, ni 9, ni 7"
	endcase

** realmente la estructura CASE no tiene vble de control; cada CASE 
** puede ser una condición diferente

	cNombre   = "Hernán"
	nEdad     = 45      && ¿Cuál es la edad     del candidato en años?
	nEstatura = 1.80    && ¿Cuál es la estatura del candidato en metros?
	lCursos   = .t.     && ¿el candidato tiene cursos?
	dInscrito = "2012-01-01"  && ¿Cuál es la fecha de inscripción del candidato?
	cGenero   = "H"     && ¿Cuál es el género   del candidato en años? (<H>ombre o <M>ujer)
	lAceptado = .t.     && Al final informará si el candidato es aceptado a no
	
	do case
	case nEdad     > 35    && si es mayor de 35 años, se descarta
	   lAceptado = .f.
	case nEdad     < 20 .and. cGenero = "M" && si es mujer menor de 20 años, se contrataría como Aprendiz
	   ** es buena candidata
	case nEstatura < 1.65  && si su estatura es menor a 1.65 metros, se descarta
	   lAceptado = .f.
	case .not. lCursos     && si no tiene cursos relacionados, se descarta
	   lAceptado = .f.
	case dInscrito > "2012-08-01"  && si se inscribió posteriormente a Ago-01-2013, se descarta
	   lAceptado = .f.
	otherwise   && no es obligatoria
	   ** es buen candidato
	endcase
	** El primer case que sea verdadero es el que se ejecuta, los demás no.
	**  (Tengo entendido que, en varios lenguajes, se debe usar BREAK el final del CASE que se seleccione 
	**   para que los demás no se ejecuten o evalúen). Fox/VFP no requiere BREAK para estos efectos.

	?
	?

	?? "El inscrito", cNombre
	if lAceptado
	   ? "queda escartado"
	else
	   ? "es buen candidato"
	endif
	
**