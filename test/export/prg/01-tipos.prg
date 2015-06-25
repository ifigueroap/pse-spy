* Este ejemplo es para ver c�mo se declaran y definen variables 
* y constantes de distintos tipos. En pseudoc�digo flexible no hace 
* falta explicitar el tipo, pero en muchos lenguajes s�. 
* Si el lenguaje permite abreviar las acciones de declarar y asignar
* el valor en una sola, es preferible que la traducci�n no utilice
* ese recurso. Ej: en C++ preferir�a "int a; a=1;" antes que "int a=1;"
* Tambi�n es para observar c�mo se escriben las constantes
* de los diferentes tipos, y c�mo se realizan las asignaciones.
* Si el lenguaje permite diferenciar entero de real, a debe ser entero
* y b real.
* Si hay varias formas de operar con cadenas, pensar con cual es
* m�s f�cil realizar la entrada/salida, y aplicar los operadores.
* Por ejemplo, en C++ usando la clase string, los operadores se
* comportan como en pseudoc�digo, mientras que usando arreglos
* de chars hay que invocar funciones.
* Por �ltimo, el caso de la variable p, es un caso donde
* no se puede determinar el tipo, ya que depender� de qu� ingrese
* el usuario cuando ejecute el programa. Indicar si el lenguaje es
* tipado o permite hacer esto. Si es tipado, indicar si existe
* algun mecanismo simple para que el programa compile asumiendo que
* es un n�mero, pero permita al usuario ver f�cilmente el problema
* y cambiar el tipo. Por ejemplo, en C/C++ uso "#define SIN_TIPO float"
* para luego declarar las variables como "SIN_TIPO x;".

** 01-Tipos.prg

    * FoxPro no es un lenguaje fuertemente tipado
	* Lo sgte est� en el lenguaje para ayudar s�lo en el Intellisense
	
    * Si una vble se declara como p�blica (ejemplos sgtes), la vble puede usarse en el 
	*   procedimiento actual, en los sub-procedimientos (los proc que se ejecutan por 
	*   �ste y en los niveles "siguientes"), y por los procedimientos "padre" (los que 
	*   llaman a �ste)
	public a as int      && no decimales
	public b as numeric  && double (s� decimales)
	public c as logical
	public d as logical
	public e as character
	public f as character
	public g as date  && tal vez este tipo de daros no sea f�cil de manejar en seudoc�digo, a menos que....

    * Si una vble se declara como local (ejemplos sgtes), la vble s�lo puede usarse en el 
	*   procedimiento actual, no existe en los procedimientos "hijos" de �ste ni por los 
	*   procedimientos "padre" de �ste)
	local aa as int      && no decimales
	local bb as numeric  && double (s� decimales)
	local cc as logical
	local dd as logical
	local ee as character
	local ff as character
	local gg as date
	
    * Si una vble se declara como privada (ejemplos sgtes), la vble puede usarse en el 
	*   procedimiento actual y en los sub-procedimientos (los proc que se ejecutan por 
	*   �ste y en los niveles "siguientes"), pero no existe en los procedimientos 
	*   "padre" (los que llaman a �ste)
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
	g = date(2013,10,20)     && as� se obtiene una fecha con valores constantes
	nAno=2013
	nMes=10
	nDia=20
	g = date(nAno,nMes,nDia) && as� se obtiene una fecha con valores vbles
	
	g = "2013-10-20"  && tal vez las fechas se puedan manejar en este formato (character, string)
	    && aunque con la funci�n DATE(--) quedan muy bien
		
	* asignaci�n m�ltiple
	store  0  to nEdad, nAltura, nPeso, iCodigo, nSalario   && todos num�ricos
	store ""  to cNombres, cApellidos, cCargo               && todos de caracteres, alfanum�ricos (string)
	store {}  to dFechaNacim                                && todos fecha (date)
	store .f. to lActivo                                    && todos l�gicos (boolean)
	
	clear
	* Si el programa mostrar� datos en pantalla, se recomiendo limpiar la pantalla con CLEAR (cls en DOS)
	* Probablemente no sea necesario en seudoc�digo
	
	* debemos inicializar P
	P=space(30)
	* Para "Leer p" se pueden usar
	@02,10 say "Escriba el valor de P: " get P
	read
	
	* Otra alternativa para "Leer p"
	@04,10 say "Escriba el valor de P"  && para "Leer p" no es necesario �sta l�nea (la de say)
	@04,35 get P  && para "Leer p" s� es necesaria �sta l�nea (la de get)... as� como la de READ
	read

	* Para "Escribir p" se pueden usar las sgtes alternativas:
	@07,10 say "El valor de P es"  && para "Escribir p" no es necesario �sta primera l�nea
	@08,10 say P

	* quiz� estas sean las mejores alternativas sgtes alternativas para SeudoC�digo:
	? P
	
	? "El valor de P es", P

**