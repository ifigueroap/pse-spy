#ifndef NEW_MEMORIA_INTER_H
#define NEW_MEMORIA_INTER_H

#define LV_LOGICA 1
#define LV_NUMERICA 2
#define LV_CARACTER 4
// LV_DEFINIDA sera 8 cuando quiera diferenciarlas, pero por ahora no
#define LV_DEFINIDA 0
#define LV_MAX ((LV_LOGICA|LV_NUMERICA|LV_CARACTER|LV_DEFINIDA)+1)
#define LV_BASE_CHAR 'A'

#endif
