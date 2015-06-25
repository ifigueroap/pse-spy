* Este ejemplo es para observar cómo trabajar con arreglos.
* El perfil flexible de pseint trabaja con índices en base 1.
* Las asignaciones son para observar si los índices van en base 1 o 0.
* Si el lenguaje no permite utilizar variables para dimensionar un
* arreglo, aclarar.

* El FoxPro/VFP la primera posición es 1 ("en base 1").

** 04-arreglos.prg

	*Dimension a[10];
	*Dimension b[20,30];
	*Dimension c[40],d[50,5];
	*Leer n;
	*Dimension e[n];
	*Leer a[1];
	*a[10]<-a[1];
	*Escribir a[10];

	dimension a[10]
	dimension b[20,30]
	dimension c[40], d[50,5]
	
	clear
	
    * Las vbles deben ser inicializadas con algún valor antes de "pedirlas"
    n = 11
    a[1] = space(10)
	
	@03,10 say "Escriba el valor de n (mayor que cero):" get n
	read
	Dimension e[n]
	
	@05,10 say "Digite a[1]:" get a[1]
	read
	
	a[10] = a[1]
	? a[10]

**