* Este ejemplo es para observar cómo se realiza la entrada y
* salida de cada tipo.
* Si hay varios mecanismos y alguno es común a todos los tipos
* se prefiere dicho mecanismo. Por ejemplo, en C++ los string se 
* pueden leer con cin>>s o getline(cin,s). Ambos generan problemas
* en casos particulares, pero siendo ambos imperfectos se prefiere
* el primero porque funciona también y sin problemas para los
* demás tipos. 
* En caso de haber problemas o limitaciones es las lecturas, 
* aclarar cuales son y cuando se presentan para que el código
* generado al exportar incluya esta aclaración como comentario.

** 02-Entrada-Salida.prg

	public a as int      && no decimales
	public b as numeric  && double (sí decimales)
	public c as character
	public d as logical
	public e as date

	* leer de a uno
	@01,10 say "Escriba el valor de a:" get a
	read
	@02,10 say "Escriba el valor de b:" get b
	read
	@03,10 say "Escriba el valor de c:" get c
	read
	@04,10 say "Escriba el valor de d:" get d
	read
	@05,10 say "Escriba el valor de e:" get e
	read

	* escribir de a uno
	? a
	? b
	? c
	? d
	? e
	
	* lecturas múltiples
	* leer a,b,c,d
	@11,10 say "Escriba el valor de a:" get a
	@12,10 say "Escriba el valor de b:" get b
	@13,10 say "Escriba el valor de c:" get c
	@14,10 say "Escriba el valor de d:" get d
	@15,10 say "Escriba el valor de e:" get e
	read
	* En usa sola pantalla se ven todos los espacios para escribir; 
	* se escribe un valor y se pulsa Enter para el sgte
	
	* escrituras múltiples
	*Escribir a,b,c,d
	? a,b,c,d
	
	* escritura sin salto de linea al final
	* Escribir sin saltar "Esta linea no lleva enter al final"
	??

**