* Este ejemplo es para observar la estructura para/for y su 
* alternativa para arreglos para cada/for each
* La estructura para cada itera por todos los elementos de un
* arreglo (puede ser multidimensional), generando en cada iteración
* un alias al elemento del arreglo con el nombre que se indique luego 
* de la palabra cada. Puede que el lenguaje no tenga esta estructura. 
* Si hay una forma de simularla, puede utilizarla, sino aclarar que no
* es posible. Por ejemplo, C++99/03 no tiene esta estructura de control,
* pero dado que los elementos de una matriz están contiguos en memoria
* se pueden recorrer con un puntero de una forma generalizable.
* Si el for del lenguaje tiene partes opcionales, puede omitirlas donde
* no sean estrictamente necesarias (como el paso en pseint cuando es 1).

** 05-para
	
	clear

	dimension a[10]
	
	for i=1 to 10
		a[i]=i*10
	endfor
	
	** el ciclo para/for se puede cerrar con "endfor" o con next"
	
	for each Elemento in a
		? Elemento
	next
	
	dimension b[3,6]
	c=0
	for each elemento in b
		c=c+1
		elemento=c
	endfor  && next
	** Supongo que lo que se pretende es darle valor a cada elemento del arreglo;
	** pero este algoritmo parece no funcionar: al mostrar el contenido (con el ciclo 
	** más adelante), los elementos siguen valiendo vacío (lo lamento: para que los 
	** elementos se "llenen" con algún valor, lo logré de la sgte forma:)
	c=0
	for i = 1 to 3
		for j = 1 to 5
		    c=c+1
			b[i,j]=c
		next
	next
	
	for i = 3 to 1 step -1
		for j = 1 to 5 step 2
			? i, j, b[i,j]
		next
	next

	?
	for i = 1 to 3
		for j = 1 to 6
			? i,j,b[i,j]
		next
	next
	
**