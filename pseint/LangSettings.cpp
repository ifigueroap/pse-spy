#include "LangSettings.h"
#include <iostream>
using namespace std;

LangSettings::aux_struct LangSettings::data[LS_COUNT];
bool LangSettings::init_done=false;

void LangSettings::init() {
	
	data[LS_FORCE_DEFINE_VARS		 ].Set("force_define_vars",			false,	
		"Obligar a definir los tipos de variables",
			"Si esta opci�n est� deshabilitada no se puede utilizar una variable sin previamente definir su tipo. "
			"Para esto se debe utilizar la palabra clave DEFINIR. Ej: \"DEFINIR X COMO REAL\". Los tipos posibles "
			"son ENTERO, NUMERICO/REAL, LOGICO, CARACTER/TEXTO."
		);
	data[LS_FORCE_INIT_VARS			 ].Set("force_init_vars",			false,
		"No permitir utilizar variables o posiciones de arreglos sin inicializar",
			"Si esta opci�n est� deshabilitada se puede utilizar variables sin incializar (que nunca fueron leidas o asignadas) "
			"en expresiones o para mostrar en pantalla. En este caso asumen el valor por defecto 0, \"\", o FALSO segun el tipo "
			"que corresponda. Si esta opci�n est� seleccionada escribir o evaluar una expresi�n que involucra una variable sin "
			"inicializar genera un error en tiempo de ejecuci�n."
		);
	data[LS_FORCE_SEMICOLON			 ].Set("force_semicolon",			false,
		"Controlar el uso de ; al final de sentencias secuenciales",
			"Si esta opci�n est� activada obliga a colocar punto y coma (;) al final de las instrucciones secuenciales, como se hace "
			"en lenguajes como C++ o Pascal (util para acostumbrarse antes de dar el salto desde el pseudoc�digo a un lenguaje real). "
			"si est� desactivada el uso del punto y coma es opcional."
		);
	data[LS_BASE_ZERO_ARRAYS		 ].Set("base_zero_arrays",			false,
		"Utilizar indices en arreglos y cadenas en base 0",
			"Si esta opci�n esta activada, el primer elemento de un arreglo de N elementos ser� el 0 y el �ltimo el N-1, mientras "
			"que en caso contrario el primero ser� el 1 y el �ltimo el N"
		);
	data[LS_ALLOW_CONCATENATION		 ].Set("allow_concatenation",		true,
		"Permitir concatenar variables de texto con el operador +",
			"Si esta opci�n esta activada se puede concatenar el contenido de dos variables de tipo caracter con el operador +. "
			"Por ejemplo: NombreCompleto <- Nombre+\" \"+Apellido;"
		);
	data[LS_USE_NASSI_SCHNEIDERMAN	 ].Set("use_nassi_schneiderman",	false,
		"Usar diagramas de Nassi-Schneiderman",
			"Con esta opci�n activada, el editor de diagramas utilizar� el formato de Nassi-Schneiderman\n"
			"en lugar del formato cl�sico de diagrama de flujo."
		);
	data[LS_USE_ALTERNATIVE_IO_SHAPES].Set("use_alternative_io_shapes",	false,
		"Usar formas alternativas para Leer y Escribir en el diagrama",
			"Con esta opci�n activada, si se utiliza el diagrama de flujo cl�sico (no Nassi-Schneiderman), los bloques para las instrucciones"
			"Leer y Escribir ser�n diferentes entre s�, siguiendo una convenci�n alternativa"
		);
	data[LS_ALLOW_DINAMYC_DIMENSIONS ].Set("allow_dinamyc_dimensions",	true,
		"Permitir utilizar variables para dimensionar arreglos",
			"Si esta opci�n est� seleccionada se puede utilizar una variable o una expresi�n que involucre variables para dimensionar un "
			"arreglo (por ejemplo, se puede pedir al usuario del programa el tama�o leyendo un numero y utilizarlo luego para crear el "
			"arreglo). Este suele ser el caso de los lenguajes interpretados, mientras que los lenguajes compilados suelen exigir que el "
			"tama�o de los arreglos est�ticos sea una constante (por ejemplo, C y C++)."
		);
	data[LS_OVERLOAD_EQUAL			 ].Set("overload_equal",			false,
		"Permitir asignar con el signo igual (=)",
			"Esta opcion habilita la asignaci�n con el signo igual (Ej: x=0;). En muchos casos esta sintaxis de asignaci�n no se"
			"permite, ya que en muchos lenguajes no se utilia el mismo operador para asignar y comparar, como sucede al activar"
			"esta opci�n. En cualquier caso, las otras dos sintaxis de asignaci�n (con <- y con :=) siguen siendo v�lidas."
		);
	data[LS_COLOQUIAL_CONDITIONS	 ].Set("coloquial_conditions",		true,
		"Permitir condiciones en lenguaje coloquial",
			"Esta opcion permite expresar las condiciones en un lenguaje m�s coloquial con construcciones como \"X ES PAR\", "
			"\"X NO ES MULTIPLO DE 5\", \"X ES IGUAL A Y\", \"X ES ENTERO\", etc. Esta opci�n activa adem�s el uso de palabras "
			"clave para reemplazar operadores."
		);
	data[LS_LAZY_SYNTAX				 ].Set("lazy_syntax",				true,
		"Utilizar sintaxis flexible",
			"Esta opcion habilita variaciones opcionales en la sintaxis de ciertas instrucciones y estructuras de control. Por ejemplo"
			"omitir la palabra HACER en un bucle MIENTRAS o PARA, utilizar la palabra DESDE para indicar el valor de inicio de un ciclo "
			"PARA, separar la expresiones/variables en una lectura/escritura con espacios en lugar de comas, escribir FinProceso como Fin "
			"Proceso, FinSi como Fin Si, etc.."
		);
	data[LS_WORD_OPERATORS			 ].Set("word_operators",			true,
		"Permitir las palabras Y, O, NO y MOD para los operadores &&, |, ~ y %",
			"Con esta opci�n habilitada PSeInt acepta las palabras clave Y, O, NO, y MOD como sin�nimos de los operadores &&, |, ~ y % respectivamente. "
			"Notar que en este caso estas palabras ser�n palabras reservadas y no se podr�n utilizar como nombres de variables."
		);
	data[LS_ENABLE_USER_FUNCTIONS	 ].Set("enable_user_functions",		true,
		"Permitir definir funciones/subprocesos",
			"Con esta opci�n activada se permite definir subprocesos/funciones en pseudoc�digo para mediante la palabra clase SubProceso."
		);
	data[LS_ENABLE_STRING_FUNCTIONS	 ].Set("enable_string_functions",	true,
		"Habilitar funciones para el manejo de cadenas",
			"Esta opci�n habilita un conjunto de funciones predefinidas que sirven para operar sobre cadenas de "
			"caracteres. Las funciones son: Longitud, SubCadena, Mayusculas, Minusculas y Concatenar)."
		);
	data[LS_INTEGER_ONLY_SWITCH].Set("integer_only_switch",	false,
		"Limitar la estructura Seg�n a variables de control num�ricas",
			"Muchos lenguajes solo permiten utilizar n�meros enteros para las expresiones de control de la estructura de "
			"selecci�n m�ltiple (\"Seg�n\" en PSeInt). Si habilita esta opci�n, PSeInt aplicar� esta restricci�n. En caso "
			"contrario, podr� utilizar tambi�n variables de tipo caracter."
		);
	data[LS_DEDUCE_NEGATIVE_FOR_STEP].Set("integer_only_switch",	true,
		"Permitir omitir el paso -1 en ciclos Para",
			"Con esta opci�n activa, si no se especifica el valor del \"paso\" en una estructura de tipo \"Para\", se utiliza +1 o -1 "
			"seg�n corresponda. Se determina comparando los valores iniciales y finales, si el primero es mayor al segundo +1, o -1 "
			"en caso contrario. Si se desactiva esta opci�n, se utilizar� siempre +1 como paso por defecto."
		);
#ifdef DEBUG
	for(int i=0;i<LS_COUNT;i++) { 
		if (!data[i].nombre) cerr<<"ERROR!!! campo no inicializado en LangSettings::data["<<i<<"]"<<endl;
	}
#endif
	init_done=true;
};


#ifdef FOR_WXPSEINT
#include <wx/string.h>
#include <wx/textfile.h>
#include "../wxPSeInt/Logger.h"
#include "../wxPSeInt/version.h"


bool LangSettings::Load (const wxString &fname) {
	_LOG("LangSettings::Load "<<fname);
	wxTextFile fil(fname); if (!fil.Exists()) return false;
	fil.Open(); if (!fil.IsOpened()) return false;
	Reset(0); // reset va despues de los "return false" para evitar resetear el perfil personalizado cuando se llama desde el ConfigManager
	for ( wxString str = fil.GetFirstLine(); !fil.Eof(); str = fil.GetNextLine() )
		ProcessConfigLine(str.c_str());
	fil.Close();
	Fix();
	return true;
}

bool LangSettings::Save (const wxString &fname) {
	wxTextFile fil(fname);
	if (!fil.Exists()) fil.Create();
	fil.Open(); if (!fil.IsOpened()) return false;
	fil.Clear();
	wxString tmp=descripcion.c_str(); tmp.Replace("\r",""); tmp.Replace("\n","\ndesc=");
	fil.AddLine(wxString("desc=")<<tmp);
	fil.AddLine(wxString("version=")<<LS_VERSION);
	for(int i=0;i<LS_COUNT;i++) 
		fil.AddLine(GetConfigLine(i).c_str());
	fil.Write();
	fil.Close();
	return true;
}

void LangSettings::Log ( ) {
	_LOG("Profile: "<<VERSION<<" "<<GetAsSingleString().c_str());
}
#else // FOR_WXPSEINT
#include <fstream>
bool LangSettings::Load(const std::string &fname) {
	ifstream fil(fname.c_str());
	if (!fil.is_open()) return false; 
	Reset(0); string str; 
	while (getline(fil,str)) ProcessConfigLine(str);
	Fix();
	return true;
}
#endif // FOR_WXPSEINT

std::string LangSettings::GetAsSingleString() {
	std::string retval(LS_COUNT,'?');
	for(int i=0;i<LS_COUNT;i++) retval[i]=settings[i]?'1':'0';
	return retval;
}


void LangSettings::Fix ( ) {
	if (version<20150304) { // antes LS_INTEGER_ONLY_SWITCH y LS_DEDUCE_NEGATIVE_FOR_STEP eran parte de LS_LAZY_SYNTAX
		settings[LS_INTEGER_ONLY_SWITCH]=!settings[LS_LAZY_SYNTAX];
		settings[LS_DEDUCE_NEGATIVE_FOR_STEP]=settings[LS_LAZY_SYNTAX];
	}
	if (settings[LS_COLOQUIAL_CONDITIONS]) // no se puede usar LS_COLOQUIAL_CONDITIONS sin LS_WORD_OPERATORS
		settings[LS_WORD_OPERATORS]=true;
	version=LS_VERSION; // colocar version nueva, para que el fix ya no actualice el perfil en la pr�xima carga
}

bool LangSettings::SetFromSingleString (const std::string & str) {
	if (str.size()!=LS_COUNT) return false;
	for(int i=0;i<LS_COUNT;i++) settings[i] = str[i]=='1';
	return true;
}

