* Este ejemplo es para ver cómo se declaran y definen variables 
* y constantes de distintos tipos. En pseudocódigo flexible no hace 
* falta explicitar el tipo, pero en muchos lenguajes sí. 
* Si el lenguaje permite abreviar las acciones de declarar y asignar
* el valor en una sola, es preferible que la traducción no utilice
* ese recurso. Ej: en C++ preferiría "int a; a=1;" antes que "int a=1;"
* También es para observar cómo se escriben las constantes
* de los diferentes tipos, y cómo se realizan las asignaciones.
* Si el lenguaje permite diferenciar entero de real, a debe ser entero
* y b real.
* Si hay varias formas de operar con cadenas, pensar con cual es
* más fácil realizar la entrada/salida, y aplicar los operadores.
* Por ejemplo, en C++ usando la clase string, los operadores se
* comportan como en pseudocódigo, mientras que usando arreglos
* de chars hay que invocar funciones.
* Por último, el caso de la variable p, es un caso donde
* no se puede determinar el tipo, ya que dependerá de qué ingrese
* el usuario cuando ejecute el programa. Indicar si el lenguaje es
* tipado o permite hacer esto. Si es tipado, indicar si existe
* algun mecanismo simple para que el programa compile asumiendo que
* es un número, pero permita al usuario ver fácilmente el problema
* y cambiar el tipo. Por ejemplo, en C/C++ uso "#define SIN_TIPO float"
* para luego declarar las variables como "SIN_TIPO x;".

** 01-Tipos.prg

    * FoxPro no es un lenguaje fuertemente tipado
	* Lo sgte está en el lenguaje para ayudar sólo en el Intellisense
	
    * Si una vble se declara como pública (ejemplos sgtes), la vble puede usarse en el 
	*   procedimiento actual, en los sub-procedimientos (los proc que se ejecutan por 
	*   éste y en los niveles "siguientes"), y por los procedimientos "padre" (los que 
	*   llaman a éste)
	public a as int      && no decimales
	public b as numeric  && double (sí decimales)
	public c as logical
	public d as logical
	public e as character
	public f as character
	public g as date  && tal vez este tipo de daros no sea fácil de manejar en seudocódigo, a menos que....

    * Si una vble se declara como local (ejemplos sgtes), la vble sólo puede usarse en el 
	*   procedimiento actual, no existe en los procedimientos "hijos" de éste ni por los 
	*   procedimientos "padre" de éste)
	local aa as int      && no decimales
	local bb as numeric  && double (sí decimales)
	local cc as logical
	local dd as logical
	local ee as character
	local ff as character
	local gg as date
	
    * Si una vble se declara como privada (ejemplos sgtes), la vble puede usarse en el 
	*   procedimiento actual y en los sub-procedimientos (los proc que se ejecutan por 
	*   éste y en los niveles "siguientes"), pero no existe en los procedimientos 
	*   "padre" (los que llaman a éste)
	* para "private" la sintaxis documentada no utiliza "as"
	private aaa
	private bbb
	private ccc
	private ddd
	private eee
	private fff
	private ggg

	a = 1
	b = 2.5
	c = .t.   && verdadero
	d = .f.   && Falso
	e = "Hola"
	f = "Hola Mundo"
	g = date()    && revuelva la fecha de hoy
	g = date(2013,10,20)     && así se obtiene una fecha con valores constantes
	nAno=2013
	nMes=10
	nDia=20
	g = date(nAno,nMes,nDia) && así se obtiene una fecha con valores vbles
	
	g = "2013-10-20"  && tal vez las fechas se puedan manejar en este formato (character, string)
	    && aunque con la función DATE(--) quedan muy bien
		
	* asignación múltiple
	store  0  to nEdad, nAltura, nPeso, iCodigo, nSalario   && todos numéricos
	store ""  to cNombres, cApellidos, cCargo               && todos de caracteres, alfanuméricos (string)
	store {}  to dFechaNacim                                && todos fecha (date)
	store .f. to lActivo                                    && todos lógicos (boolean)
	
	clear
	* Si el programa mostrará datos en pantalla, se recomiendo limpiar la pantalla con CLEAR (cls en DOS)
	* Probablemente no sea necesario en seudocódigo
	
	* debemos inicializar P
	P=space(30)
	* Para "Leer p" se pueden usar
	@02,10 say "Escriba el valor de P: " get P
	read
	
	* Otra alternativa para "Leer p"
	@04,10 say "Escriba el valor de P"  && para "Leer p" no es necesario ésta línea (la de say)
	@04,35 get P  && para "Leer p" sí es necesaria ésta línea (la de get)... así como la de READ
	read

	* Para "Escribir p" se pueden usar las sgtes alternativas:
	@07,10 say "El valor de P es"  && para "Escribir p" no es necesario ésta primera línea
	@08,10 say P

	* quizá estas sean las mejores alternativas sgtes alternativas para SeudoCódigo:
	? P
	
	? "El valor de P es", P

**