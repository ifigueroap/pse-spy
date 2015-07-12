#include "SynCheck.h"
#include "global.h"
#include <deque>
#include <string>
#include "utils.h"
#include "new_evaluar.h"
#include "intercambio.h"
#include "new_programa.h"
#include "new_funciones.h"
#include "case_map.h"
using namespace std;

static int PSeudoFind(const string &s, char x, int from=0, int to=-1) {
	if (to==-1) to=s.size();
	if (x=='\"') x='\'';
	else if (x=='[') x='(';
	else if (x==']') x=')';
	int par=0; bool com=false;
	for (int i=from;i<to;i++) {
		char c=s[i];
		if (c=='\''||c=='\"') { 
			if (x=='\'') return i;
			com=!com;
		} else if (!com) {
			if (c=='('||c=='[') {
				if (par==0 && x=='(') return i;
				par++;
			} else if (c==')'||c==']') {
				if (par==0 && x==')') return i;
				par--;
			} else if (par==0 && c==x) 
				return i; 
		}
	}
	return -1;
}

// para checkear que las dimensiones de los arreglos no involucren variables
static bool IsNumericConstant(string &str) {
	for (unsigned int i=0;i<str.size();i++)
		if (EsLetra(str[i])) return false;
	return true;
}

// pasar todo a mayusculas, reemplazar tabs, comillas, word_operators, corchetes, y quita espacios extras
static void SynCheckAux1(string &cadena) {
	// corregir saltos de linea win/linux
	if (cadena.size()>0 && (cadena[cadena.size()-1]==13||cadena[cadena.size()-1]==10) ) cadena[cadena.size()-1]=' ';
	if (cadena.size()>1 && (cadena[cadena.size()-2]==13||cadena[cadena.size()-2]==10) ) cadena[cadena.size()-2]=' ';
	bool comillas=false;
	int len = cadena.size();
	// primero, todo a may�sculas y cambio de comillas y par�ntesis
	for (int tmp=0;tmp<len;tmp++) {
		char &c=cadena[tmp];
		if (!comillas && tmp>0 && c=='/' && cadena[tmp-1]=='/')
			{ cadena=cadena.substr(0,tmp-1); len=tmp-1; break; }
		if (c=='\"' || c=='\'') { 
			c='\''; comillas=!comillas;
		} else if (!comillas) {
			if (c=='[') c='(';
			else if (c==']') c=')';
			else if (c==9) c=' ';
			else if (c==' ' && tmp!=0 && cadena[tmp-1]==' ') { cadena.erase(--tmp,1); len--; }
			else c=ToUpper(c);
		}
	}
	// despues, word_operators
	if (lang[LS_WORD_OPERATORS]) {
		comillas=false;
		for (int tmp=0;tmp<len;tmp++) {
			char &c=cadena[tmp];
			if (c=='\'') comillas=!comillas;
			else if (!comillas) {
				if (c=='Y' && (tmp==0 || !parteDePalabra(cadena[tmp-1])) && (tmp==len-1 || !parteDePalabra(cadena[tmp+1])) )
					c='&';
				if (c=='O' && (tmp==0 || !parteDePalabra(cadena[tmp-1])) && (tmp==len-1 || !parteDePalabra(cadena[tmp+1])) )
					c='|';
				if (c=='O' && tmp>0 && cadena[tmp-1]=='N' && (tmp-1==0 || !parteDePalabra(cadena[tmp-2])) && (tmp+1==len || !parteDePalabra(cadena[tmp+1])) )
				{ cadena[tmp-1]='~'; cadena.erase(tmp,1); tmp--; len--; }
				if (c=='D' && tmp>1 && cadena[tmp-1]=='O' && cadena[tmp-2]=='M' && (tmp-2==0 || !parteDePalabra(cadena[tmp-3])) && (tmp+1==len || !parteDePalabra(cadena[tmp+1])) )
				{ cadena[tmp-2]='%'; cadena.erase(tmp-1,2); tmp-=2; len-=2;	}
			}
		}
	}
	// Borrar espacios en blanco al principio y al final
	while (cadena.size() && cadena[0]==' ') {cadena.erase(0,1); len--;}
	while (cadena.size() && cadena[cadena.size()-1]==' ') {cadena.erase(cadena.size()-1,1);; len--;}
	// agregar espacios para evitar que cosas como "SI(x<2)ENTONCES" generen un error
//	comillas=false;
//	for(int i=0;i<len;i++) {
//		if (cadena[i]=='\'') comillas=!comillas;
//		else if (!comillas) {
//			if (cadena[i]=='(' && i>0 && EsLetra(cadena[i-1])) { cadena.insert(i," "); len++; }
//			if (cadena[i]==')' && i+2<len && EsLetra(cadena[i+1])) { cadena.insert(i+1," "); len++; }
//		}
//	}
}

	
// reescribir condiciones coloquiales
static void SynCheckAux2(string &cadena) {
	struct coloquial_aux {
		string cond, pre, post, rep;
		int csize;
		coloquial_aux(){}
		coloquial_aux(string c, string pr, string re, string po) : cond(c),pre(pr),post(po),rep(re), csize(cond.size()){}
	};
	static coloquial_aux *coloquial_conditions_list=NULL;
	static int coloquial_conditions_list_size=0;
#ifdef _DEBUG
	if (coloquial_conditions_list && cadena=="<<<delete coloquial_conditions_list>>>") {
		delete []coloquial_conditions_list;
		coloquial_conditions_list=NULL;
		return;
	}
#endif
	if (!cadena.size() || !lang[LS_COLOQUIAL_CONDITIONS]) return;
	if (cadena[cadena.size()-1]!=' ') cadena+=" ";
	int comillas=-1;
	for (int y=0;y<int(cadena.size());y++) {
		if (cadena[y]=='\'' || cadena[y]=='\"') comillas=-comillas;
		else if (comillas<0) {
			if (y>0&&(cadena[y]==','||/*cadena[y]=='('||*/cadena[y]==';')&&cadena[y-1]!=' ')
				cadena.insert(y++," ");
		}
	}
	comillas=-1;
	
	if (!coloquial_conditions_list) {
		coloquial_conditions_list=new coloquial_aux[200]; int i=0;
		coloquial_conditions_list[i++]=coloquial_aux(" ES ENTERO ",				"(",	")=TRUNC(",		"<PRE>)"	);
		coloquial_conditions_list[i++]=coloquial_aux(" ES ENTERA ",				"(",	")=TRUNC(",		"<PRE>)"	);
		coloquial_conditions_list[i++]=coloquial_aux(" ES MENOR A ",			"(",	")<(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES MENOR QUE ",			"(",	")<(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES MAYOR A ",			"(",	")>(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES MAYOR QUE ",			"(",	")>(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES IGUAL | MAYOR A ",	"(",	")>=(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES IGUAL | MAYOR QUE ",	"(",	")>=(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES IGUAL | MENOR A ",	"(",	")<=(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES IGUAL | MENOR QUE ",	"(",	")<=(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES MAYOR | IGUAL A ",	"(",	")>=(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES MAYOR | IGUAL QUE ",	"(",	")>=(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES MENOR | IGUAL A ",	"(",	")<=(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES MENOR | IGUAL QUE ",	"(",	")<=(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES IGUAL A ",			"(",	")=(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES IGUAL QUE ",			"(",	")=(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES DISTINTO A ",			"(",	")<>(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES DISTINTO DE ",		"(",	")<>(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES DISTINTA A ",			"(",	")<>(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES DISTINTA DE ",		"(",	")<>(",			")"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES PAR ",				"(",	")%2=0 ",		""		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES IMPAR ",				"(",	")%2=1 ",		""		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES POSITIVO ",			"(",	")>0 ",			""			);
		coloquial_conditions_list[i++]=coloquial_aux(" ES POSITIVA ",			"(",	")>0 ",			""			);
		coloquial_conditions_list[i++]=coloquial_aux(" ES NEGATIVO ",			"(",	")<0 ",			""			);
		coloquial_conditions_list[i++]=coloquial_aux(" ES NEGATIVA ",			"(",	")<0 ",			""			);
		coloquial_conditions_list[i++]=coloquial_aux(" ES CERO ",				"(",	")=0 ",			""			);
		coloquial_conditions_list[i++]=coloquial_aux(" ES DIVISIBLE POR ",		"(",	") % (",		")=0"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES MULTIPLO DE ",		"(",	") % (",		")=0"		);
		coloquial_conditions_list[i++]=coloquial_aux(" ES ",					"(",	")=(",			")"		);
		coloquial_conditions_list_size=i;
	}
	for (int y=0;y<int(cadena.size());y++) {
		if (cadena[y]=='\'' || cadena[y]=='\"') comillas=-comillas;
		else if (comillas<0) {
			if (y+3<int(cadena.size()) && cadena[y]==' '&&cadena[y+1]=='E'&&cadena[y+2]=='S'&&cadena[y+3]==' ') {
				int cual=-1;
				bool negate=(y>1 && cadena[y-1]=='~' && cadena[y-2]==' ');
				for(int j=0;j<coloquial_conditions_list_size;j++) { // buscar si coincide con alguna expresion de la lista
					if (cadena.substr(y,coloquial_conditions_list[j].csize)==coloquial_conditions_list[j].cond) {
						cual=j;	break;
					}
				}
				if (cual!=-1) {
					if (negate) { cadena.erase(y-2,2); y-=2; }
					coloquial_aux &col=coloquial_conditions_list[cual];
					cadena.replace(y,col.csize,col.rep);
					// si era una expresion coloquial, agregar pre y post antes y despues de los operandos
					string pre;
					{
						int parentesis=0, yold=y--; bool comillas=false;
						while ( y>=0 && ( (parentesis>0||comillas) || (cadena[y]!='&' && cadena[y]!=',' && cadena[y]!='|' && cadena[y]!='~' && cadena[y]!='(') ) ) {
								if (cadena[y]=='\''||cadena[y]=='\"') comillas=!comillas;
								else if (!comillas) {
									if (cadena[y]==')') parentesis++;
									else if (cadena[y]=='(') parentesis--;
								}
								y--;
						}
						int y2=yold-1; while (y2>y && cadena[y2]==' ') y2--;
						pre=cadena.substr(y+1,y2-y);
						if (col.pre.size()) cadena.insert(y+1,col.pre);
						if (negate) { cadena.insert(y+1,"~"); yold++; }
						y=yold+col.pre.size();
					}
					if (col.post.size()) {
						int parentesis=0, yold=y,l=cadena.size(); y+=col.rep.size();  bool comillas=false;
						while ( y<l 
							&& ( (parentesis>0||comillas) || (cadena[y]!='&' && cadena[y]!=',' && cadena[y]!='|' && cadena[y]!='~' && cadena[y]!=')' && cadena[y]!=';'))
							&& (comillas||(cadena.substr(y,2)!="//"&&cadena.substr(y,10)!=" ENTONCES "&&cadena.substr(y,7)!=" HACER ")) ) {
								if (cadena[y]=='\''||cadena[y]=='\"') comillas=!comillas;
								else if (!comillas) {
									if (cadena[y]==')') parentesis--;
									else if (cadena[y]=='(') parentesis++;
								}
								y++;
						}
						while (y>yold && cadena[y-1]==' ') y--;
						string post=col.post;
						size_t n=post.find("<PRE>");
						if (n!=string::npos) post.replace(n,5,pre);
						cadena.insert(y,post);
						y=yold;
					}
				}
			}
		}
	}
}

enum what { w_null, w_operhand, w_operator, w_space };
enum what_extra { w_comma, w_other, 
	w_relational_op,w_logic_op,w_aritmetic_op,
	w_number_int, w_number_dec, w_string, w_id, w_expr,
	w_binary_op, w_unary_op, w_ambiguos_op, w_no_op };


// retorna 1 si es operador unario, 2 si es binario, 3 si puede ser cualquiera de los dos, y 0 si no es operador
// ademas, en len retorna cuando ocupa ese operador
int is_valid_operator(char act, char next, int &len, what_extra &type) {
	switch (act) {
		case '+': case '-': 
			len=1; type=w_aritmetic_op; return w_ambiguos_op; 
		case '*': case '/': case '^': case '%':
			len=1; type=w_aritmetic_op; return w_binary_op; 
		case '~':  
			len=1; type=w_logic_op; return w_unary_op;
		case '&': case '|':
			len=(next==act)?2:1; 
			type=w_logic_op; return w_binary_op;
		case '!':
			type=w_relational_op; 
			if (next=='=') { len=2; return w_binary_op; }
			else { len=1; return w_unary_op; }
		case '<': case '>': case '=':
			type=w_relational_op; 
			len=(next=='='||(act=='<'&&next=='>'))?2:1;
			return w_binary_op;
	}
	return w_no_op;
}

// verificar operadores, constantes y parentesis, y borrar los espacios en blanco que sobran entre ellos
static void SynCheckAux3(const int &x, string &cadena, int &errores,const  string &instruccion, int &flag_pyc) {
	bool allow_multiple_expresions=instruccion!=":" && instruccion!="<-";
	what w=w_null; what_extra wext=w_other;
	int parentesis=0, csize; bool comillas=false;
	for (int i=0;i<(csize=(int)cadena.size());i++) {
		char act=cadena[i]; 
		if (act=='\'') {
			if (!comillas && w==w_operhand) { SynError (304,"Falta operador (antes de la cadena de caracteres)."); errores++; }
			else if (comillas) { w=w_operhand; wext=w_string; }
			comillas=!comillas;
		} else if (!comillas) {
			if (act==' ') {
				char next=cadena[i+1]; bool next_es_letra=EsLetra(next);
				if (w==w_operator) {
					// solo puede seguir un operando (id, cte, o expresion)
					if (next_es_letra) {
						int j=i+2; while (EsLetra(cadena[j])) j++;
						const string word=cadena.substr(i+1,j-i-1);
						if (PalabraReservada(word) && word!=VERDADERO && word!=FALSO) 
							{SynError (237,"Falta operando (antes de "+cadena.substr(i+1,j-i-1)+")."); errores++; } // hola+ ;
						else { cadena.erase(i,1); i--; }
					} else if ((next>='0'&&next<='9') || next=='.' || next=='\'' || next=='(' || next==':' || next==';') {
						cadena.erase(i,1); i--;
					} else {
						what_extra type; int len; int ret=is_valid_operator(cadena[i+1],i+2<csize?cadena[i+2]:' ',len,type);
						if (ret!=w_unary_op&&ret!=w_ambiguos_op) 
							{ SynError (224,"Falta operando (despues de "+string(1,cadena[i-1])+")."); errores++; }
						else { cadena.erase(i,1); i--; }
					}
				} else if (w==w_operhand) {
					// si lo que sigue es otro operador, puede haber problemas a menos que se trate de "escribir" con sintaxis flexible
					if (next==')' || next==':' || next==';') {
						cadena.erase(i,1); i--;
					} else if (next_es_letra || (next>='0'&&next<='9') || next=='.' || next=='\'' || (next=='(' && allow_multiple_expresions)) {
						if (allow_multiple_expresions) {
							w=w_null;
//							cadena.erase(i,1); i--;
						} else {
							SynError (225,"Falta operador o coma entre operandos."); errores++;
						}
					} else {
						cadena.erase(i,1); i--;
					}
				} else {
					cadena.erase(i,1); i--;
				}
			} else if (act==';' || act==':') {
				if (act==':' && instruccion!=":") {SynError (226,"Operador no valido (:)."); errores++;}
				else if (w==w_operator) {
					SynError (227,"Falta operando (antes de "+string(1,act)+")."); errores++;
				}
				w=w_null; wext=w_other;
				if (act==';') {
					// todo: ver si realmente puede llegar esto hasta aca, o se corta en otro lado
					programa.Insert(x+1,cadena.substr(i+1));
					cadena.erase(i+1); flag_pyc+=1;
				}
			} else if (act==',') {
				if (w==w_operator) {
					SynError (228,"Falta operador."); errores++;
				}
				else if (w==w_null && wext==w_comma) {SynError (3,"Par�metro nulo."); errores++; } // 35+;
				w=w_null; wext=w_comma;
			
			} else if (act=='(') {
				if (w==w_operhand && wext!=w_id) {SynError (229,"Falta operador."); errores++; } // 123(21
				w=w_null; wext=w_other; parentesis++;
			
			} else if (act==')') {
				if (w==w_operator) {SynError (230,"Falta operando (antes de ')')."); errores++; }
				w=w_operhand; wext=w_expr; parentesis--;
			
			} else if (act>='0'&&act<='9') {
				if (w!=w_operhand) { w=w_operhand; wext=w_number_int; }
				else if (wext==w_string) { SynError (305,"Falta operador (despues de la cadena de caracteres)."); errores++; }
				else if (wext==w_expr) { SynError (239,"Falta operador (despues de ')')."); errores++; }
			
			} else if (act=='.') {
				if (w!=w_operhand) { w=w_operhand; wext=w_number_dec; }
				else if (wext!=w_number_int) {
					if (!ignore_logic_errors) { SynError (231,"Constante num�rica no v�lida."); errores++; }
				} else wext=w_number_dec;
			
			} else if (EsLetra(act)) {
				if (w==w_operhand && wext!=w_id) {
					if (wext==w_string)
						{ SynError (233,"Falta operando (despu�s de cadena de texto)."); errores++; }
					else if (wext==w_expr)
						{ SynError (307,"Falta operando (posiblemente despu�s de ')')."); errores++; }
					else
						if (!ignore_logic_errors) { SynError (238,"Constante num�rica no v�lida."); errores++; }
				}
				w=w_operhand; wext=w_id;
			
			} else {
				int len=1; char next=cadena[i+1];
				what_extra op_type;
				int ret=is_valid_operator(act,next,len,op_type);
				if (ret!=w_no_op) {
					if (ret==w_binary_op) {
						if (w!=w_operhand) { SynError (234,"Falta operando (antes de "+cadena.substr(i,len)+")."); errores++; }
						i+=len-1;
					} else {
						if (w==w_operator && wext==w_aritmetic_op) { SynError (235,"Falta operando (antes de "+cadena.substr(i,len)+")."); errores++; }
					}
					w=w_operator; wext=op_type;
				} else {SynError (68,"Caracter no valido."); errores++;}
			} 
		}
	}
	if (w==w_operator) { SynError (236,"Falta operando al final de la expresi�n"); errores++; }
	// Posibles errores encontrados
	if (parentesis<0) { SynError (35,"Se cerraron parentesis o corchetes dem�s."); errores++; }
	if (parentesis>0) { 
		SynError (36,"Falta cerrar parentesis o corchete."); errores++;
	}
	if (comillas) { SynError (37,"Falta cerrar comillas."); errores++; }
}

static bool RightCompareFix(string &s, string e) {
	int le=e.size(); int ls=s.size();
	bool fix=e[0]==' ';
	if (fix) { le--; e=e.substr(1); }
	if (ls<le) return false;
	else if (le==ls) {
		if (s==e) { if (fix) s.insert(0," "); return true;}
		else return false;
	}
	else if (s.substr(ls-le,le)!=e) return false;
	else if (s[ls-le-1]==')') { s.insert(ls-le," "); ls++; };
	return s[ls-le-1]==' ';
}

static bool LeftCompareFix(string &s, string e) {
	int ls=s.size(), le=e.size();
	bool fix=e[le-1]==' ';
	if (fix) { le--; e=e.substr(0,le); }
	if (ls<le) return false;
	else if (ls==le) {
		if (s==e) { if (fix) s+=" "; return true;}
		else return false;
	}
	else if (s.substr(0,le)!=e) return false;
	else if (s[le]=='(') s.insert(le," ");
	return s[le]==' ';
}

void InformUnclosedLoops(deque<Instruccion> &bucles, int &errores) {
	// Controlar Cierre de Bucles
	while (!bucles.empty())	{
		if (bucles.back()=="PARA") {SynError (114,"Falta cerrar PARA.",bucles.back().num_linea,bucles.back().num_instruccion); errores++;}
		else if (bucles.back()=="REPETIR") {SynError (115,"Falta cerrar REPETIR.",bucles.back().num_linea,bucles.back().num_instruccion); errores++;}
		else if (bucles.back()=="MIENTRAS") {SynError (116,"Falta cerrar MIENTRAS.",bucles.back().num_linea,bucles.back().num_instruccion); errores++;}
		else if (bucles.back()=="SI"||bucles.back()=="SINO") {SynError (117,"Falta cerrar SI.",bucles.back().num_linea,bucles.back().num_instruccion); errores++;}
		else if (bucles.back()=="SEGUN") {SynError (118,"Falta cerrar SEGUN.",bucles.back().num_linea,bucles.back().num_instruccion); errores++;}
		else if (bucles.back()=="PROCESO") {SynError (119,"Falta cerrar PROCESO.",bucles.back().num_linea,bucles.back().num_instruccion); errores++;}
		else if (bucles.back()=="SUBPROCESO") {SynError (119,"Falta cerrar SUBPROCESO.",bucles.back().num_linea,bucles.back().num_instruccion); errores++;}
		bucles.pop_back();
	}
}

static bool SirveParaReferencia(string &s) {
	int p=0, l=s.size(), parentesis=0;
	bool in_name=true; // si estamos en la primer parte (nombre) o segunda (indices si es un arreglo)
	while (p<l) {
		if (s[p]=='(') {
			if (in_name) {
				if (!CheckVariable(s.substr(0,p))||EsFuncion(s.substr(0,p))) return false;
				in_name=false;
			}
			parentesis++;
		} else if (s[p]==')') {
			parentesis--;
		} else if (parentesis==0 && !in_name) return false; // cualquier operador fuera de parentesis indica expresion y no es legal
		p++;
		
	}
	if (in_name && (!CheckVariable(s)||EsFuncion(s)) ) return false;
	return true;
}

//-------------------------------------------------------------------------------------------
// ********************* Checkear la Correcta Sintaxis del Archivo **************************
//-------------------------------------------------------------------------------------------

int SynCheck(int linea_from, int linea_to) {
	
	programa.SetRefPoint(linea_to);
	Memoria global_memory(NULL); // para usar al analizar instrucciones fuera de proceso/subprocesos
	memoria=&global_memory;
	SynErrores=0;
	deque <Instruccion> bucles; // Para controlar los bucles que se abren y cierran
//	bucles.push_front(Instruccion("CHECK",1,1)); // para que estaba esto? (para evitar acceder a una posicion no valida?)
	int errores=0; // Total de errores , y cant hasta la instruccion anterior
	int flag_pyc=0, tmp;
	bool in_process=false;
	tipo_var tipo;
	string cadena, instruccion, str;
	Funcion *current_func; // funcion actual, necesito el puntero para setear line_end cuando encuentre el FinProceso/FinSubProceso
	
	// Checkear sintaxis y reorganizar el codigo
	for (int x=linea_from;x<programa.GetRefPoint();x++){
		Inter.SetLineAndInstructionNumber(x);
		cadena=programa[x];
		int Lerrores=errores;
		
		// Ignorar lineas de comentarios
		{
			int comillas=-1;
			instruccion="";

			// puede haber que trimear las cadenas que surgieron de separar lineas con mas de una instruccion
			int pt1=0,pt2=cadena.size(), l=cadena.size();
			while (pt1<l && cadena[pt1]==' ') pt1++;
			while (pt2>0 && cadena[pt2-1]==' ') pt2--;
			if (pt1!=0||pt2!=l) cadena=cadena.substr(pt1,pt2-pt1);
			
			
			int len = cadena.size();
			if (lang[LS_LAZY_SYNTAX] && LeftCompare(cadena,"FIN ")) { cadena="FIN"+cadena.substr(4); len--; }
			if (LeftCompare(cadena,"BORRAR ")) { cadena="BORRAR"+cadena.substr(7); len--; }
			// poner un espacio al final para evitar casos especiales cuando no hay punto y coma y la instruccion es una sola palabra (ej: borrarpantalla; finalgo )
			if (len>2 && cadena[len-1]==';' && cadena[len-2]!=' ') { cadena.insert(len-1," "); len++; }
			else if (len>1 && cadena[len-1]!=';') { cadena+=" "; len++; } 
			
			
			// en esta parte (chorrera de if {} else if {} else if...) se identifica la instruccion... 
			//       primero se busca si empieza con alguna palabra clave
			//       sino, se mira si puede ser opcion de un segun viendo si estamos en un segun y hay : en la cadena
			//       sino, se mira si puede asignacion buscando alguno de los operadores de asignacion
			//       sino, se mira si puede ser la nueva definicion (x es entero), mirando si la anteultima palabra es ES o SON
			// si se identifica la instrucci�n, se quita del string cadena y se guarda en el string instruccion
			if (LeftCompare(cadena,"ENTONCES ")) {
				if (/*bucles.back()!="SI" || */!LeftCompare(programa[x-1],"SI "))
					{SynError (1,"ENTONCES mal colocado."); errores++;}
				str=cadena;
				instruccion="ENTONCES "; cadena="";
				if (str.size()>9) {
					str.erase(0,9);
					programa.Insert(x+1,str);
					flag_pyc+=1;
				}
			} else if (LeftCompare(cadena,"SINO ")) {
				if (bucles.back()!="SI")
					{SynError (2,"SINO mal colocado."); errores++;}
				else { bucles.pop_back(); bucles.push_back(programa.GetLoc(x,"SINO")); }
				str=cadena;
				instruccion="SINO "; cadena="";
				if (str.size()>0) {
					str.erase(0,5);
					programa.Insert(x+1,str);
					flag_pyc+=1;
				}
			} else if (LeftCompare(cadena,"ESCRIBIR ") || (lang[LS_LAZY_SYNTAX] && (LeftCompare(cadena,"IMPRIMIR ") || LeftCompare(cadena,"MOSTRAR ") || LeftCompare(cadena,"INFORMAR "))) ) {
				instruccion="ESCRIBIR "; cadena.erase(0,cadena.find(" ")+1);
				if (ReplaceIfFound(cadena,"SIN SALTAR","",true)||ReplaceIfFound(cadena,"SIN BAJAR","",true)||ReplaceIfFound(cadena,"SINBAJAR","",true)||ReplaceIfFound(cadena,"SINSALTAR","",true))
					instruccion="ESCRIBNL ";
			} else if (LeftCompare(cadena,"LEER ")) {
				instruccion="LEER "; cadena.erase(0,5);
			} else if (LeftCompareFix(cadena,"SI ") && !LeftCompare(cadena,"SI ES ")) {
				instruccion="SI "; cadena.erase(0,3);
				bucles.push_back(programa.GetLoc(x,"SI"));
				// cortar el entonces si esta en la misma linea
				comillas=-1;
				if (RightCompareFix(cadena," ENTONCES ")) {
					cadena.erase(cadena.size()-10,10);
					programa.Insert(x+1,"ENTONCES");
					flag_pyc+=1;
				} else
					for (int y=0; y<(int)cadena.size()-10;y++) {
						if(cadena[y]=='\"' || cadena[y]=='\'') comillas=-comillas;
						if (comillas<1 && MidCompareNC(" ENTONCES ",cadena,y)) {
							str=cadena;
							cadena.erase(y+1,cadena.size()-y);
							// borrar los espacios en medio
							while (cadena[cadena.size()-1]==' ' && cadena.size()!=0) cadena.erase(cadena.size()-1,1);
							str.erase(0,y+1);
							programa.Insert(x+1,str);
							flag_pyc+=1;
							break;
						}
					}
			} else if (LeftCompareFix(cadena,"MIENTRAS ")&&(!lang[LS_LAZY_SYNTAX]||!LeftCompareFix(cadena,"MIENTRAS QUE "))) { 
				instruccion="MIENTRAS "; cadena.erase(0,9);
				bucles.push_back(programa.GetLoc(x,"MIENTRAS"));
			} else if (LeftCompareFix(cadena,"SEGUN ")) {
				instruccion="SEGUN "; cadena.erase(0,6);
				bucles.push_back(programa.GetLoc(x,"SEGUN"));
			} else if (LeftCompare(cadena,"DE OTRO MODO: ") || LeftCompare(cadena,"DE OTRO MODO ")) {
				cadena.erase(0,13); programa.Insert(x+1,cadena); flag_pyc+=1;
				instruccion="DE OTRO MODO: "; cadena="";
			} else if (LeftCompare(cadena,"DIMENSION ")) {
				instruccion="DIMENSION "; cadena.erase(0,10);
			} else if (LeftCompareFix(cadena,"HASTA QUE ")) {
				instruccion="HASTA QUE "; cadena.erase(0,10);
			} else if (lang[LS_LAZY_SYNTAX]&&LeftCompareFix(cadena,"MIENTRAS QUE ")) {
				instruccion="MIENTRAS QUE "; cadena.erase(0,13);
			} else if (LeftCompare(cadena,"FINSI ")) {
				instruccion="FINSI "; cadena.erase(0,6);
			} else if (LeftCompare(cadena,"FINPARA ")) {
				instruccion="FINPARA "; cadena.erase(0,8);
			} else if (LeftCompare(cadena,"FINMIENTRAS ")) {
				instruccion="FINMIENTRAS "; cadena.erase(0,12);
			} else if (LeftCompare(cadena,"FINSEGUN ")) {
				instruccion="FINSEGUN "; cadena.erase(0,9);
			} else if (LeftCompare(cadena,"ESPERARTECLA ")) {
				instruccion="ESPERARTECLA"; cadena.erase(0,13);
			} else if (LeftCompare(cadena,"ESPERAR UNA TECLA ")) {
				instruccion="ESPERARTECLA"; cadena.erase(0,18);
			} else if (LeftCompare(cadena,"ESPERAR TECLA ")) {
				instruccion="ESPERARTECLA"; cadena.erase(0,14);
			} else if (LeftCompare(cadena,"ESPERAR ")) {
				instruccion="ESPERAR "; cadena.erase(0,8);
			} else if (LeftCompare(cadena,"LIMPIARPANTALLA ")) {
				instruccion="BORRARPANTALLA"; cadena.erase(0,16);
			} else if (LeftCompare(cadena,"BORRAR PANTALLA ")) {
				instruccion="BORRARPANTALLA"; cadena.erase(0,16);
			} else if (LeftCompare(cadena,"LIMPIAR PANTALLA ")) {
				instruccion="BORRARPANTALLA"; cadena.erase(0,17);
			} else if (LeftCompare(cadena,"BORRARPANTALLA ")) {
				instruccion="BORRARPANTALLA"; cadena.erase(0,15);
			} else if (LeftCompare(cadena,"PROCESO ")) {
				instruccion="PROCESO "; cadena.erase(0,8);
			} else if (LeftCompare(cadena,"SUBPROCESO ")) {
				instruccion="SUBPROCESO "; cadena.erase(0,11);
			} else if (LeftCompare(cadena,"FUNCION ")||LeftCompare(cadena,"FUNCI�N ")) {
				instruccion="SUBPROCESO "; cadena.erase(0,8);
			} else if (lang[LS_LAZY_SYNTAX] && LeftCompare(cadena,"PARACADA ")) {
				instruccion="PARACADA "; cadena.erase(0,9);
				bucles.push_back(programa.GetLoc(x,"PARACADA"));
			} else if (lang[LS_LAZY_SYNTAX] && LeftCompare(cadena,"PARA CADA ")) {
				instruccion="PARACADA "; cadena.erase(0,10);
				bucles.push_back(programa.GetLoc(x,"PARACADA"));
			} else if (LeftCompare(cadena,"PARA ")) {
				instruccion="PARA "; cadena.erase(0,5);
				bucles.push_back(programa.GetLoc(x,"PARA"));
				// si se puede asignar con igual, reemplazar aca
				if (lang[LS_OVERLOAD_EQUAL]) {
					int i=0, l=cadena.size();
					while (i<l && EsLetra(cadena[i])) i++;
					while (i<l && (cadena[i]==' '||cadena[i]=='\t')) i++;
					if (i<l&& cadena[i]=='=') cadena.replace(i,1,"<-");
				}
				// evitar problema de operador incorrecto al poner el signo al numero
				comillas=-1;
				// si dice "i desde 1" en lugar de "i<-1" se reemplaza " desde " por "<-"
				if (lang[LS_LAZY_SYNTAX] && cadena.find("<-",0)==string::npos && cadena.find(" DESDE ")!=string::npos) 
					cadena.replace(cadena.find(" DESDE "),7,"<-");
				if (cadena.find("<-",0)!=string::npos) {
					// se agregan parentesis al valor inicial para evitar problemas mas adelante (porque si el valor es negativo, con la flecha de asignacion queda un --)
					for (int y=cadena.find("<-",0)+3; y<(int)cadena.size();y++) {
						if(cadena[y]=='\"' || cadena[y]=='\'') comillas=-comillas;
						if (comillas<1 && (MidCompareNC(" HASTA ",cadena,y) || MidCompareNC(" CON PASO ",cadena,y))) {
							cadena.insert(y,")");
							cadena.insert(cadena.find("<-",0)+2,"(");
							break;
						}
					}
				}
			} else if (LeftCompareFix(cadena,"SEG�N ")) {
				instruccion="SEGUN "; cadena.erase(0,6);
				bucles.push_back(programa.GetLoc(x,"SEGUN"));
			} else if (LeftCompare(cadena,"FINSEG�N ")) {
				instruccion="FINSEGUN "; cadena.erase(0,9);
			} else if (LeftCompare(cadena,"FINPROCESO;")) {
				instruccion="FINPROCESO "; cadena.erase(0,10);
				if (!ignore_logic_errors) { SynError (315,"FINPROCESO no lleva punto y coma."); errores++; }
			} else if (LeftCompare(cadena,"FINPROCESO ")) {
				instruccion="FINPROCESO "; cadena.erase(0,11);
			} else if (LeftCompare(cadena,"FINFUNCION ")||LeftCompare(cadena,"FINFUNCI�N ")) {
				instruccion="FINSUBPROCESO "; cadena.erase(0,11);
			} else if (LeftCompare(cadena,"FINSUBPROCESO ")) {
				instruccion="FINSUBPROCESO "; cadena.erase(0,14);
			} else if (LeftCompare(cadena,"REPETIR ")) {
				instruccion="REPETIR "; cadena.erase(0,8);
			} else if (lang[LS_LAZY_SYNTAX] && LeftCompare(cadena,"HACER ")) {
				instruccion="REPETIR "; cadena.erase(0,6);
			} else if (LeftCompare(cadena,"DEFINIR ")) {
				instruccion="DEFINIR "; cadena.erase(0,8);
			} else {
				int flag_segun=0;
				if (!bucles.empty()) {
					// comentado el 20121013 para que siempre diga que lo que esta antes del : es una instruccion y lo que esta despues otra.
//					if (bucles.back()=="SEGUN") { // si esta en segun comprobar la condicion
						int pos_dp=PSeudoFind(cadena,':');
						if (pos_dp!=-1 && cadena[pos_dp+1]!='=') {
							// ver ademas si dise "OPCION o CASO al principio"
							if (lang[LS_LAZY_SYNTAX]) {
								if (cadena.size()>6 && cadena.substr(0,5)=="CASO ") {
									cadena=cadena.substr(5); pos_dp-=5;
								} else if (cadena.size()>6 && cadena.substr(0,5)=="SIES ") {
									cadena=cadena.substr(5); pos_dp-=5;
								} else if (cadena.size()>7 && cadena.substr(0,6)=="SI ES ") {
									cadena=cadena.substr(6); pos_dp-=6;
								} else if (cadena.size()>8 && cadena.substr(0,7)=="OPCION ") {
									cadena=cadena.substr(7); pos_dp-=7;
								} else if (cadena.size()>8 && cadena.substr(0,7)=="OPCI�N ") {
									cadena=cadena.substr(7); pos_dp-=7;
								}
							}
							instruccion=":";
							programa.Insert(x+1,cadena);
							programa[x+1].instruccion.erase(0,pos_dp+1);
							cadena.erase(pos_dp+1,cadena.size()-pos_dp-1);
							flag_pyc+=1; flag_segun=1;
							if (bucles.back()!="SEGUN") { SynError (241,"Opci�n fuera de SEGUN."); errores++; }
						}
//					}
				}
				if (flag_segun==0) {
					instruccion="Error?";
					// Ver si es asignacion
					int i=0, l=cadena.size();
					while (i<l && (cadena[i]=='\t'||cadena[i]==' ')) i++;
					int par=0;
					while (i<l && (par||
//						(cadena[i]>='a'&&cadena[i]<='z') || // no deberia hacer falta a esta altura
						EsLetra(cadena[i]) ||
						(cadena[i]>='0'&&cadena[i]<='9') ||
						cadena[i]=='_'||cadena[i]=='(')) {
							if (cadena[i]=='(') par++;
							else if (cadena[i]==')') par--;
							i++;
						}
					while (i<l && (cadena[i]=='\t'||cadena[i]==' ')) i++;
					if (i>0&&i<l) {
						if (i+1<l && cadena[i]==':' && cadena[i+1]=='=') {cadena[i]='<';cadena[i+1]='-';}
						else if (lang[LS_OVERLOAD_EQUAL] && cadena[i]=='=') cadena.replace(i,1,"<-");
						if (i+1<l&&cadena[i]=='<'&&cadena[i+1]=='-') {
							comillas=-1;
							for (int y=0; y<(int)cadena.find("<-",0);y++)
								if(cadena[y]=='\"' || cadena[y]=='\'') comillas=-comillas;
							if (comillas<1) {
								instruccion="<-";
								// evitar problema de operador incorrecto al poner el signo al numero
								cadena.insert(i+2,"(");
								cadena.insert(cadena.size(),") ");
								for (int y=i+3; y<(int)cadena.size();y++) {
									if(cadena[y]=='\"' || cadena[y]=='\'') comillas=-comillas;
									if (comillas<1 && cadena[y]==';') {
										cadena.insert(y,")");
										cadena.erase(cadena.size()-2,2);
										break;
									}
								}
							} 
						} 
					}
					if (lang[LS_LAZY_SYNTAX] && instruccion!="<-") { // definici�n de tipos alternativa (x es entero)
						size_t pos=cadena.rfind(' ',cadena.size()-(cadena[cadena.size()-1]==';'?3:2));
						if (pos!=string::npos) {
							pos=cadena.rfind(' ',pos-1);
							if (pos!=string::npos && cadena.substr(pos+1,4)=="SON ") {
								instruccion="DEFINIR "; cadena.replace(pos+1,3,"COMO");
							} else if (pos!=string::npos && cadena.substr(pos+1,3)=="ES ") {
								instruccion="DEFINIR "; cadena.replace(pos+1,2,"COMO");
							}
						}
					}
//					if (instruccion!="<-") {
//						int p=0, l=cadena.length();
//						while (p<l&&((cadena[p]>='A'&&cadena[p]<='Z')||cadena[p]=='_'||(cadena[p]>='0'&&cadena[p]<='9'))) p++;
//						const Funcion *func=EsFuncion(cadena.substr(0,p));
//						if (func) 
//							instruccion=string("INVOCAR ");
//					}
					if (instruccion!="<-" && instruccion!="DEFINIR ") {
						int p=0, l=cadena.length();
						while (p<l&&((cadena[p]>='A'&&cadena[p]<='Z')||cadena[p]=='_'||(cadena[p]>='0'&&cadena[p]<='9'))) p++;
						const Funcion *func=EsFuncion(cadena.substr(0,p));
						if (func) 
							instruccion=string("INVOCAR ");
					}
				}
			}
			
			// reescribir condiciones coloquiales
			SynCheckAux2(cadena);

			// verificar operadores
			SynCheckAux3(x,cadena,errores,instruccion,flag_pyc);
			
			// y si hay algo a continuacion del hacer tambien, asi que despues del hacer se corta como si fuera hacer; para que se pueda escribir por ejemplo un mientras en una sola linea
			comillas=-1; len=cadena.size();
			for (tmp=0;tmp<len;tmp++) {
				if (cadena[tmp]=='\'') comillas=-comillas;
				else if (comillas<0 && tmp+5<len && cadena.substr(tmp,6)=="HACER " /*&& cadena.substr(tmp,6)!="HACER;"*/) {
					programa.Insert(x+1,cadena.substr(tmp+5));
					cadena.erase(tmp+5); break;
				}
			}
			
			if (cadena.size()&&cadena[cadena.size()-1]==',')
				{ SynError (31,"Parametro nulo."); errores++; }
			while (cadena[0]==';' && cadena.size()>1) cadena.erase(0,1); // para que caso esta esto?
			// Controlar que el si siempre tenga un entonces
			if (x&&LeftCompare(programa[x-1],"SI "))
				if (instruccion!="ENTONCES " && instruccion!="" && instruccion!="Error?") {
					if (lang[LS_LAZY_SYNTAX]) {
						programa.Insert(x,"ENTONCES"); 
						programa[x].num_instruccion--;
						x++;
					} else 
						{SynError (32,"Se esperaba ENTONCES"); errores++;}
				}
			// si entro en segun comprobar que haya condicion
			if (!bucles.empty()) {
				if (bucles.back()=="SEGUN" && LeftCompare(programa[x-1],"SEGUN") && cadena!="") {
					if (instruccion!=":") { SynError (33,"Se esperaba <opcion>:."); errores++; }
				}
			}
			
			// Controlar el punto y coma
			bool lleva_pyc=instruccion=="DIMENSION " || instruccion=="DEFINIR " || instruccion=="ESCRIBIR "|| instruccion=="ESCRIBNL " || instruccion=="<-"  || instruccion=="LEER " || instruccion=="ESPERAR " || instruccion=="ESPERARTECLA" || instruccion=="BORRARPANTALLA" || instruccion=="INVOCAR ";
			if (lleva_pyc) {
				if (cadena[cadena.size()-1]!=';') {
					if (lang[LS_FORCE_SEMICOLON])
						{ SynError (38,"Falta punto y coma."); errores++; }
					cadena=cadena+';';
				}
			}
			// Pegar la instrucci�n
			if (instruccion!="Error?" && instruccion!="<-" && instruccion!=":")
				cadena=instruccion+cadena;
			if (cadena.size()&&cadena[cadena.size()-1]==' ') cadena.erase(cadena.size()-1,1); // Borrar espacio en blanco al final
			// Cortar instrucciones despues de sino o entonces
			while (cadena.size()&&cadena[cadena.size()-1]==' ') // Borrar espacios en blanco al final
				cadena.erase(cadena.size()-1,1);
			// arreglar problemas con valores negativos en para y mientras
			ReplaceIfFound(cadena," HASTA-"," HASTA -");
			ReplaceIfFound(cadena," PASO-"," PASO -");
			ReplaceIfFound(cadena," QUE-"," QUE -");
			
			
			// En esta parte, seg�n cada instrucci�n se verifican si los argumentos est�n bien. Los argumentos quedaron solos en cadena, la instrucci�n ya fu� cortada.
			if (instruccion=="SUBPROCESO " || instruccion=="PROCESO ") {
				if (in_process) InformUnclosedLoops(bucles,errores); in_process=true;
				bool es_proceso = instruccion=="PROCESO ";
				current_func=subprocesos[ExtraerNombreDeSubProceso(cadena.substr(es_proceso?8:11))];
				current_func->line_start=x;
				bucles.push_back(programa.GetLoc(x,es_proceso?"PROCESO":"SUBPROCESO"));
				current_func->userline_start=Inter.GetLineNumber();
				memoria=current_func->memoria=new Memoria(current_func);
			}
			if (!in_process && cadena!="") {SynError (43,lang[LS_ENABLE_USER_FUNCTIONS]?"Instrucci�n fuera de proceso/subproceso.":"Instrucci�n fuera de proceso."); errores++;}
			if ((cadena=="FINPROCESO" || cadena=="FINSUBPROCESO") ) {
				bool sub=cadena!="FINPROCESO"; in_process=false;
				if (!bucles.empty() && ( (!sub&&bucles.back()=="PROCESO")||(sub&&bucles.back()=="SUBPROCESO") ) ) {
					if (current_func) { current_func->userline_end=Inter.GetLineNumber(); current_func=NULL; }
					bucles.pop_back();
				} else {
					if (!bucles.empty() && ( (!sub&&bucles.front()=="PROCESO")||(sub&&bucles.front()=="SUBPROCESO") ) ) {
						bucles.pop_front();
						InformUnclosedLoops(bucles,errores);
					} else {
						SynError (308,sub?"FINSUBPROCESO mal colocado.":"FINPROCESO mal colocado."); errores++;
					}
				}
				memoria=&global_memory;
			}
			// Controlar correcta y completa sintaxis de cada instruccion
			if (instruccion=="DEFINIR "){  // ------------ DEFINIR -----------//
				if (cadena=="DEFINIR" || cadena=="DEFINIR ;")
					{SynError (44,"Faltan parametros."); errores++;}
				else {
					if (cadena[cadena.size()-1]!=';') {
						cadena=cadena+";";
						if (!ignore_logic_errors) { SynError (45,"Falta punto y coma."); errores++; }
					}
					size_t pos_como=cadena.find(" COMO ");
					string def_tipo; if (pos_como!=string::npos) {pos_como+=6; def_tipo=cadena.substr(pos_como); }
					// corregir acentos
					if (LeftCompare(def_tipo,"N�M")) { cadena[pos_como+1]='U'; def_tipo[1]='U'; }
					else if (LeftCompare(def_tipo,"L�G")) { cadena[pos_como+1]='O'; def_tipo[1]='O'; }
					else if (LeftCompare(def_tipo,"NUM�")) { cadena[pos_como+3]='E'; def_tipo[3]='E'; }
					else if (LeftCompare(def_tipo,"CAR�C")) { cadena[pos_como+3]='A'; def_tipo[3]='A'; }
						
					if (def_tipo=="ENTEROS;"||def_tipo=="ENTERAS;") cadena.replace(cadena.size()-8,8,"ENTERO;");
					else if (def_tipo=="ENTERA;") cadena.replace(cadena.size()-7,7,"ENTERO;");
					else if (def_tipo=="REALES;") cadena.replace(cadena.size()-7,7,"REAL;");
					else if (def_tipo=="NUMEROS;") cadena.replace(cadena.size()-8,8,"REAL;");
					else if (def_tipo=="NUMERO;") cadena.replace(cadena.size()-7,7,"REAL;");
					else if (def_tipo=="NUMERICA;"||def_tipo=="NUMERICO;") cadena.replace(cadena.size()-9,9,"REAL;");
					else if (def_tipo=="NUMERICAS;"||def_tipo=="NUMERICOS;") cadena.replace(cadena.size()-10,10,"REAL;");
					else if (def_tipo=="TEXTO;") cadena.replace(cadena.size()-6,6,"CARACTER;");
					else if (def_tipo=="CARACTERES;") cadena.replace(cadena.size()-11,11,"CARACTER;");
					else if (def_tipo=="TEXTOS;") cadena.replace(cadena.size()-7,7,"CARACTER;");
					else if (def_tipo=="CADENA;") cadena.replace(cadena.size()-7,7,"CARACTER;");
					else if (def_tipo=="CADENAS;") cadena.replace(cadena.size()-8,8,"CARACTER;");
					else if (def_tipo=="LOGICOS;"||def_tipo=="LOGICAS;") cadena.replace(cadena.size()-8,8,"LOGICO;");
					else if (def_tipo=="LOGICA;") cadena.replace(cadena.size()-7,7,"LOGICO;");
					
					if (!RightCompare(cadena," COMO REAL;") && !RightCompare(cadena," COMO ENTERO;") && !RightCompare(cadena," COMO CARACTER;") && !RightCompare(cadena," COMO LOGICO;") ) {
						if (!ignore_logic_errors) { SynError (46,"Falta tipo de dato o tipo no valido."); errores++;}
					} else {
						int largotipo=0; tipo_var tipo_def=vt_desconocido;;
						if (RightCompare(cadena," COMO REAL;")) { largotipo=11; tipo_def=vt_numerica; }
						if (RightCompare(cadena," COMO ENTERO;")) { largotipo=13; tipo_def=vt_numerica; }
						else if (RightCompare(cadena," COMO LOGICO;")) { largotipo=13; tipo_def=vt_logica; }
						else if (RightCompare(cadena," COMO CARACTER;")) { largotipo=15; tipo_def=vt_caracter; }
						cadena[cadena.size()-largotipo]=',';
						// evaluar los nombre de variables
						int tmp1=0,tmp2=0,tmp3=8;
						for (tmp1=8;tmp1<(int)cadena.size()-largotipo+1;tmp1++) {
							if (cadena[tmp1]=='(') tmp2++;
							if (cadena[tmp1]==')') tmp2--;
							if (tmp1>0 && tmp1<(int)cadena.size()-1)
								if (comillas<0 && cadena[tmp1]==' ' && cadena[tmp1-1]!='&' && cadena[tmp1-1]!='|'  && cadena[tmp1+1]!='&'  && cadena[tmp1+1]!='|')
								{SynError (47,"Se esperaba fin de expresion."); errores++;}
							if (tmp2==0 && cadena[tmp1]==',') { // comprobar validez
								str=cadena;
								str.erase(tmp1,str.size()-tmp1);
								str.erase(0,tmp3);
								if (str.find("(",0)==string::npos) {
									if (!CheckVariable(str,48)) errores++;
									else {
										if (memoria->EsArgumento(str) && !ignore_logic_errors) { SynError (222,"No debe redefinir el tipo de un argumento."); errores++; }
										memoria->DefinirTipo(str,tipo_def);
									}
								} else {
									str.erase(str.find("(",0),str.size()-str.find("(",0));
									if (!ignore_logic_errors) { SynError (212,string("No debe utilizar subindices (")+str+")."); errores++; }
								}
								tmp3=tmp1+1;
							}
						}
						cadena[cadena.size()-largotipo]=' ';
					}
				}
			}
			if (instruccion=="ESCRIBIR " || instruccion=="ESCRIBNL "){  // ------------ ESCRIBIR -----------//
				if (cadena=="ESCRIBIR" || cadena=="ESCRIBIR ;"||cadena=="ESCRIBNL" || cadena=="ESCRIBNL ;")
					{SynError (53,"Faltan parametros."); errores++;}
				else {
					if (cadena[cadena.size()-1]==';')
						cadena[cadena.size()-1]=',';
					else
						cadena=cadena+",";
					bool comillas=false; // cortar parametros
					int parentesis=0;
					for (int last_i=9, i=9;i<(int)cadena.size();i++) {
						if (cadena[i]=='\'') comillas=!comillas;
						else if (comillas) continue;
						if (cadena[i]=='(') parentesis++;
						if (cadena[i]==')') parentesis--;
						if (i>0 && i<(int)cadena.size()-1) {
							if (lang[LS_LAZY_SYNTAX] && cadena[i]==' ') cadena[i]=',';
							if (cadena[i]==' ' && cadena[i-1]!='&' && cadena[i-1]!='|'  && cadena[i+1]!='&'  && cadena[i+1]!='|')
								{SynError (54,"Se esperaba fin de expresion."); errores++;}
						}
						if (parentesis==0 && cadena[i]==',') { // comprobar validez
							str=cadena.substr(last_i,i-last_i);
							if (Lerrores==errores) EvaluarSC(str,tipo);
							last_i=i+1;
						}
					}
					cadena[cadena.size()-1]=';';
				}
			}
			if (instruccion=="ESPERAR "){  // ------------ ESCRIBIR -----------//
				if (cadena=="ESPERAR" || cadena=="ESPERAR ;")
					{SynError (217,"Faltan parametros."); errores++;}
				else {
					string str=cadena.substr(8);
					if (RightCompare(cadena," SEGUNDOS;")) str.erase(str.size()-10);
					else if (RightCompare(cadena," SEGUNDO;")) str.erase(str.size()-9);
					else if (RightCompare(cadena," MILISEGUNDOS;")) str.erase(str.size()-14);
					else if (RightCompare(cadena," MILISEGUNDO;")) str.erase(str.size()-13);
					else if (!ignore_logic_errors) {SynError (218,"Falta unidad o unidad desconocida."); errores++;}
					EvaluarSC(str,tipo,vt_numerica);
					if (!tipo.cb_num) {SynError (219,"La longitud del intervalo debe ser num�rica."); errores++;} else {
						for (int tmp1=0;tmp1<(int)str.size();tmp1++) if (str[tmp1]==' ') {SynError (240,"Se esperaba una sola expresi�n."); errores++;}
					}
				}
			}
			if (instruccion=="DIMENSION "){  // ------------ DIMENSION -----------//
				if (cadena=="DIMENSION" || cadena=="DIMENSION ;")
				{SynError (56,"Faltan parametros."); errores++;}
				else {
					if (cadena[cadena.size()-1]==';')
						cadena[cadena.size()-1]=',';
					else
						cadena=cadena+",";
					int tmp1=0,tmp2=0,tmp3=10;
					for (tmp1=10;tmp1<(int)cadena.size();tmp1++) {
						if (cadena[tmp1]=='(') tmp2++;
						if (cadena[tmp1]==')') tmp2--;
						if (tmp1>0 && tmp1<(int)cadena.size()-1)
							if (comillas<0 && cadena[tmp1]==' ' && cadena[tmp1-1]!='&' && cadena[tmp1-1]!='|'  && cadena[tmp1+1]!='&'  && cadena[tmp1+1]!='|')
							{SynError (57,"Se esperaba fin de expresion."); errores++;}
						if (tmp2==0 && cadena[tmp1]==',') { // comprobar validez
							str=cadena;
							str.erase(tmp1,str.size()-tmp1);
							str.erase(0,tmp3);
							if (str.find("(",0)==string::npos){ 
								if (!ignore_logic_errors) { SynError (58,"Faltan subindices para el arreglo."); errores++; }
								if (!CheckVariable(str,59)) errores++;
								else if (!memoria->EstaDefinida(str)) memoria->DefinirTipo(str,vt_desconocido); // para que aparezca en la lista de variables
							} else {
								string aname;
								str.erase(str.find("(",0),str.size()-str.find("(",0));
								if (!CheckVariable(str,60)) errores++;
								if (memoria->EsArgumento(str) && !ignore_logic_errors) { SynError (223,"No debe redimensionar un argumento."); errores++; }
								else aname=str; // para que aparezca en la lista de variables
								str=cadena;
								str.erase(tmp1,str.size()-tmp1);
								str.erase(0,str.find("(",tmp3)+1);
								if (str[str.size()-1]==')')
									str.erase(str.size()-1,1);
								str=str+",";
								
								// contar dimensiones y reservar espacio para el arreglo dims
								int parentesis=0, len=str.size(), ndims=0; bool comillas=false;
								for(int i=0;i<len;i++) { 
									if (str[i]=='\'') comillas=!comillas;
									else if (!comillas) {
										if (str[i]==',') ndims++;
										else if (str[i]=='(') parentesis++;
										else if (str[i]==')') parentesis--;
									}
								}
								int *dims=new int[ndims+1], idims=1; dims[0]=ndims;
								
								// comprobar los indices
								string str2,res_eval;
								while (str.find(",",0)!=string::npos){
									str2=str;
									str2.erase(str.find(",",0),str.size()-str.find(",",0));
									if (str2=="")
										{SynError (61,"Parametro nulo."); errores++;}
									if (Lerrores==errores) res_eval=EvaluarSC(str2,tipo,vt_numerica);
									if (tipo!=vt_error&&!tipo.cb_num) {
										{SynError (62,"No coinciden los tipos."); errores++;}
										dims[idims]=-1;
									} else {
										if (!IsNumericConstant(str2)) {
											dims[idims]=-1;
											if (!lang[LS_ALLOW_DINAMYC_DIMENSIONS])
												{SynError (153,"Las dimensiones deben ser constantes."); errores++;}
										} else {
											dims[idims]=(int)StrToDbl(res_eval);
										}
									}
									str.erase(0,str2.size()+1);
									idims++;
								}
								if (aname.size()) memoria->AgregarArreglo(aname,dims);
							}
							tmp3=tmp1+1;
						}
					}
				}
				cadena[cadena.size()-1]=';';
			}
			if (instruccion=="LEER "){  // ------------ LEER -----------//
				if (cadena=="LEER" || cadena=="LEER ;")
				{SynError (63,"Faltan parametros."); errores++;}
				else {
					if (cadena[cadena.size()-1]==';')
						cadena[cadena.size()-1]=',';
					else
						cadena=cadena+",";
					int tmp1=0,tmp2=0,tmp3=5;
					comillas=-1;
					for (tmp1=5;tmp1<(int)cadena.size();tmp1++) {
						if (cadena[tmp1]=='(') tmp2++;
						if (cadena[tmp1]==')') tmp2--;
						if (cadena[tmp1]=='\'') { comillas=-comillas; continue; }
						else if (comillas>0) continue;
						if (tmp1>0 && tmp1<(int)cadena.size()-1) {
							if (lang[LS_LAZY_SYNTAX] && cadena[tmp1]==' ') cadena[tmp1]=',';
							if (comillas<0 && cadena[tmp1]==' ' && cadena[tmp1-1]!='&' && cadena[tmp1-1]!='|'  && cadena[tmp1+1]!='&'  && cadena[tmp1+1]!='|')
								{SynError (64,"Se esperaba fin de expresion."); errores++;}
						}
						if (tmp2==0 && cadena[tmp1]==',') { // comprobar validez
							str=cadena;
							str.erase(tmp1,str.size()-tmp1);
							str.erase(0,tmp3);
							if (str.find("(",0)==string::npos) {
								if (!CheckVariable(str,65)) errores++;
								else {
									if (!memoria->EstaDefinida(str)) memoria->DefinirTipo(str,vt_desconocido); // para que aparezca en la lista de variables
									if (memoria->LeerDims(str) && !ignore_logic_errors) SynError(255,"Faltan subindices para el arreglo.");
								}
							} else if (!memoria->EsArgumento(str.substr(0,str.find("(",0)))) {
								bool name_ok=true;
								string aname=str.substr(0,str.find("(",0));
								if (!CheckVariable(aname,66)) { errores++; name_ok=false; }
								else if (!memoria->EstaDefinida(aname)) memoria->DefinirTipo(aname,vt_desconocido); // para que aparezca en la lista de variables
								if (!memoria->LeerDims(aname) && !ignore_logic_errors) { 
									SynError(256,"La variable ("+aname+") no es un arreglo."); name_ok=false;
								}
								str=cadena;
								str.erase(tmp1,str.size()-tmp1);
								str.erase(0,tmp3);
								str.erase(0,str.find("(",0));
								if (str[str.size()-1]==')')
									str.erase(str.size()-1,1);
								str.erase(0,1);
								str=str+",";
								string str2;
								// comprobar los indices
								int ca=0;
								while (str.find(",",0)!=string::npos){
									str2=str;
									str2.erase(str.find(",",0),str.size()-str.find(",",0));
									// if (str2=="") {SynError (67,"Parametro nulo."); errores++;}
									if (Lerrores==errores) EvaluarSC(str2,tipo,vt_numerica);
									if (tipo!=vt_error&&!tipo.cb_num)
										{SynError (154,"No coinciden los tipos."); errores++;}
									str.erase(0,str2.size()+1);
									ca++;
								}
								if (name_ok && memoria->LeerDims(aname)[0]!=ca && !ignore_logic_errors) {
									SynError(257,string("Cantidad de indices incorrecta para el arreglo (")+aname+(")"));
									return false;
								}
							}
							tmp3=tmp1+1;
						}
					}
				}
				cadena[cadena.size()-1]=';';
			}
			if (instruccion=="PARA "){  // ------------ PARA -----------//
				str=cadena; // cortar instruccion
				str.erase(0,5);
				if (str.find(" ",0)==string::npos) {SynError (70,"Faltan parametros."); errores++;}
				if (!RightCompareFix(str," HACER")) {
					if (lang[LS_LAZY_SYNTAX]) { str+=" HACER"; cadena+=" HACER";}
					else {SynError (71,"Falta HACER."); errores++;}
				}
				if (RightCompareFix(str," HACER")) { // comprobar asignacion
					str.erase(str.find(" ",0),str.size()-str.find(" ",0));
					if (str.find("<-",0)==string::npos) // Comprobar asignacion
						{SynError (72,"Se esperaba asignacion."); errores++;}
					else
						if (RightCompare(str,"<-HASTA") || RightCompare(str,"<-CON PASO") || LeftCompare(str,"<-"))
						{SynError (73,"Asignacion incompleta."); errores++;}
						else {
							str.erase(str.find("<-",0),str.size()-str.find("<-",0));
							if (!CheckVariable(str,74)) errores++; else {
								memoria->DefinirTipo(str,vt_numerica); // para que aparezca en la lista de variables
								if (Lerrores==errores) EvaluarSC(str,tipo,vt_numerica);
								if (!tipo.cb_num)
									{SynError (76,"No coinciden los tipos."); errores++;}
								str=cadena;
								str.erase(0,str.find("<-")+2);
								str.erase(str.find(" "),str.size()-str.find(" ",0));
								if (Lerrores==errores) EvaluarSC(str,tipo,vt_numerica);
								if (!tipo.cb_num)
									{SynError (77,"No coinciden los tipos."); errores++;}
								else { // comprobar hasta y variable final
									str=cadena;
									size_t pos_hasta=str.find(" ",8);
									str.erase(0,pos_hasta);
									if (lang[LS_LAZY_SYNTAX] && LeftCompare(str," CON PASO ")) { // si esta el "CON PASO" antes del "HASTA", dar vuelta
										size_t e=str.find(" ",10);
										if (e!=string::npos) { // si hay algo despues del "CON PASO"
											str.erase(e); // corta la parte de "CON PASO X"
											cadena=cadena.substr(0,pos_hasta)+cadena.substr(pos_hasta+e); // rearma la cadena sin el paso
											cadena.insert(cadena.size()-6,str); // inserta el paso
											str=cadena; // pone str como si no hubiese pasado nada
											str.erase(0,pos_hasta);
										}
									}
									if (!LeftCompare(str," HASTA ")) {
										if (LeftCompare(str," CON PASO ")) { 
											SynError (216,"CON PASO va despues de HASTA."); errores++;
										} else {
											SynError (78,"Falta HASTA."); errores++;
										}
									} else if (LeftCompare(str," HASTA HACER"))
										{SynError (79,"Falta el valor final del PARA."); errores++;}
									else {
										str.erase(0,7); str.erase(str.size()-6,6);
										if (str.find(" ",0)==string::npos) {
											if (Lerrores==errores) EvaluarSC(str,tipo,vt_numerica);
											if (!tipo.cb_num) {SynError (80,"No coinciden los tipos."); errores++;}
										} else {
											str.erase(str.find(" ",0),str.size()-str.find(" ",0));
											if (Lerrores==errores) EvaluarSC(str,tipo,vt_numerica);
											if (!tipo.cb_num) {SynError (81,"No coinciden los tipos."); errores++;}
											str=cadena; // comprobar con paso
											str.erase(0,str.find("HASTA",6)+6);
											str.erase(0,str.find(" ",0)+1);
											str.erase(str.size()-6,6);
											if (!LeftCompare(str,"CON PASO ")) {
												if (str=="CON PASO")
													{SynError (258,"Falta el valor del paso."); errores++;}
												else
													{SynError (82,"Se esparaba CON PASO o fin de instruccion."); errores++;}
											} else {
												str.erase(0,9);
												EvaluarSC(str,tipo,vt_numerica);
												if (!tipo.cb_num) {SynError (84,"No coinciden los tipos."); errores++;}
											}
										}
									}
								}
							}
						}
				}
			}
			
			if (instruccion=="PARACADA "){  // ------------ PARA CADA -----------//
				str=cadena; // cortar instruccion
				str.erase(0,9);
				if (str.find(" ",0)==string::npos)
				{SynError (70,"Faltan parametros."); errores++;}
				if (!RightCompareFix(str," HACER")) {
					if (lang[LS_LAZY_SYNTAX]) { str+=" HACER"; cadena+=" HACER";}
					else {SynError (71,"Falta HACER."); str+=" HACER"; errores++;}
				}
				if (RightCompareFix(str," HACER")) {
					int i=0; while (str[i]!=' ') i++;
					if (!CheckVariable(str.substr(0,i),259)) errores++;
					if (str.substr(i,4)==" EN ") cadena.replace(9+i,4," DE ");
					else if (str.substr(i,4)!=" DE ") {SynError (260,"Se esperaba DE o EN."); errores++;} 
					else if (!CheckVariable(str.substr(i+4,str.size()-i-10),261)) errores++;
				}
			}
			
			if (instruccion==":") {  // ------------ opcion del SEGUN -----------//
				int p;
				// permitir utiliza O para separar la posibles opciones
				if (lang[LS_LAZY_SYNTAX]) {
					while ((p=cadena.find(" O "))!=-1) cadena.replace(p,3,",");
					while ((p=cadena.find("|"))!=-1) cadena.replace(p,1,",");
				}
				cadena[cadena.size()-1]=',';
				int i=0;
				while ((p=PSeudoFind(cadena,',',i))!=-1) {
					tipo=vt_caracter_o_numerica;
					EvaluarSC(cadena.substr(i,p-i),tipo,lang[LS_INTEGER_ONLY_SWITCH]?vt_numerica:vt_caracter_o_numerica);
					if (!tipo.cb_num&&!lang[LS_LAZY_SYNTAX]&&tipo!=vt_error)
						SynError (203,"Las opciones deben ser de tipo numerico."); errores++;
					i=p+1;
				}
				cadena[cadena.size()-1]=':';
			}
			
			if (instruccion=="<-") {  // ------------ ASIGNACION -----------//
				str=cadena;
				str.erase(str.find("<-",0),str.size()-str.find("<-",0));
				if (str.size()==0)
				{SynError (85,"Asignacion incompleta."); errores++;}
				else {
					if (!CheckVariable(str,86)) errores++;
					string vname=str;
					str=cadena;
					str.erase(0,str.find("<-",0)+2);
					comillas=-1; int parentesis=0;
					for (int y=0;y<(int)str.size();y++){ // comprobar que se un solo parametro
						if (str[y]=='(') parentesis++;
						if (str[y]==')') parentesis--;
						if (str[y]=='\'') comillas=-comillas;
						if (y>0 && y<(int)str.size()-1)
							if (comillas<0 && str[y]==' ' && str[y-1]!='&' && str[y-1]!='|'  && str[y+1]!='&'  && str[y+1]!='|')
							{SynError (87,"Se esperaba fin de expresion."); errores++;}
						if (comillas<0 && parentesis==1 && str[y]==',')
						{SynError (88,"Demasiados parametros."); errores++;}
					}
					if (str.size()==3)
					{SynError (89,"Asignacion incompleta."); errores++;}
					else {
						str.erase(str.size()-1,1);
						if (Lerrores==errores) EvaluarSC(str,tipo);
						if (!memoria->LeerTipo(vname).can_be(tipo)) {
							SynError(125,"No coinciden los tipos."); errores++; 
							if (!memoria->EstaDefinida(str)) memoria->DefinirTipo(str,vt_desconocido); // para que aparezca en la lista de variables
						}
						else memoria->DefinirTipo(vname,tipo);
					}
				}
			}
			if (instruccion=="SI "){  // ------------ SI -----------//
				if (cadena=="SI")
					{ SynError (90,"Falta la condicion en la estructura Si-Entonces."); errores++; }
				else
					str=cadena; // Comprobar la condicion
				str.erase(0,3);
				// comprobar que no halla espacios
				comillas=-1;
				for (int tmp1=0;tmp1<(int)str.size();tmp1++) {
					if (str[tmp1]=='\'') comillas=-comillas;
					if (tmp1>0 && tmp1<(int)str.size()-1) {
						// si encuentra un espacio (que no saco SynCheckAux3) es porque habia una instruccion despues del si, faltaba el "ENTONCES"
						if (comillas<0 && str[tmp1]==' ' && str[tmp1-1]!='&' && str[tmp1-1]!='|'  && str[tmp1+1]!='&'  && str[tmp1+1]!='|') {
							if (lang[LS_LAZY_SYNTAX]) {
								programa.Insert(x+1,str.substr(tmp1)); flag_pyc++;
								cadena.erase(3+tmp1);
								break;
							} else
								SynError (91,"Se esperaba ENTONCES o fin de expresion."); errores++;
						}
					}
				}
				tipo=vt_logica;
				if (Lerrores==errores) EvaluarSC(str,tipo,vt_logica);
				if (!tipo.cb_log) { SynError (92,"No coinciden los tipos."); errores++; }
			}
			if (instruccion=="HASTA QUE "||instruccion=="MIENTRAS QUE "){  // ------------ HASTA QUE -----------//
				if (cadena=="HASTA QUE"||cadena=="MIENTRAS QUE")
				{ SynError (93,"Falta la condicion en la estructura Repetir."); errores++; cadena+=" "; }
				else {
					str=cadena; // Comprobar la condicion
					str.erase(0,instruccion=="HASTA QUE "?10:13);
					// comprobar que no halla espacios
					comillas=-1;
					for (int tmp1=0;tmp1<(int)str.size();tmp1++) {
						if (str[tmp1]=='\'') comillas=-comillas;
						if (tmp1>0 && tmp1<(int)str.size()-1)
							if (comillas<0 && str[tmp1]==' ' && str[tmp1-1]!='&' && str[tmp1-1]!='|'  && str[tmp1+1]!='&'  && str[tmp1+1]!='|')
							{SynError (94,"Se esperaba fin de expresion."); errores++;}
					}
					if (str[str.size()-1]==';') {
						str=str.substr(0,str.size()-1);
						cadena=cadena.substr(0,cadena.size()-1);
					}
					if (Lerrores==errores) EvaluarSC(str,tipo,vt_logica);
					if (!tipo.cb_log) { SynError (95,"No coinciden los tipos."); errores++; }
				}
			}
			if (instruccion=="SEGUN "){  // ------------ SEGUN -----------//
//				last_was_segun=true;
				if (cadena=="SEGUN HACER" || cadena=="SEGUN")
				{ SynError (96,"Falta la variable/expresi�n de control en la estructura Segun."); errores++; }
				else
					if (!RightCompareFix(cadena," HACER")) {
						if (lang[LS_LAZY_SYNTAX]) cadena+=" HACER";
						else { SynError (97,"Falta HACER."); errores++; }
					}
					if (RightCompareFix(str," HACER")) {
						str=cadena; // Comprobar la condicion
						str.erase(str.size()-6,6);
						str.erase(0,6);
						// comprobar que no halla espacios
						comillas=-1;
						for (int tmp1=0;tmp1<(int)str.size();tmp1++) {
							if (str[tmp1]=='\'') comillas=-comillas;
							if (tmp1>0 && tmp1<(int)str.size()-1)
								if (comillas<0 && str[tmp1]==' ' && str[tmp1-1]!='&' && str[tmp1-1]!='|'  && str[tmp1+1]!='&'  && str[tmp1+1]!='|')
								{SynError (98,"Se esperaba fin de expresion."); errores++;}
						}
						if (Lerrores==errores) EvaluarSC(str,tipo,lang[LS_INTEGER_ONLY_SWITCH]?vt_numerica:vt_caracter_o_numerica);
						if (!tipo.cb_num&&lang[LS_INTEGER_ONLY_SWITCH]) { SynError (100,"No coinciden los tipos."); errores++; }
					}
			}
			if (instruccion=="MIENTRAS ") { // ------------ MIENTRAS -----------//
				if (cadena=="MIENTRAS ")
				{ SynError (101,"Falta la condicion en la estructura Mientras."); errores++; }
				else
					if (RightCompare(cadena,";")) {
						if (!ignore_logic_errors) { SynError (262,"MIENTRAS no lleva punto y coma luego de la condicion."); errores++; }
						cadena.erase(cadena.size()-1,1);
					}
					if (!RightCompareFix(cadena," HACER")) {
						if (lang[LS_LAZY_SYNTAX]) cadena+=" HACER";
						else { SynError (102,"Falta HACER."); errores++; }
					}
					if (RightCompareFix(cadena," HACER")) {
						str=cadena; // Comprobar la condicion
						str.erase(str.size()-6,6);
						str.erase(0,9);
						if (str=="") { SynError (263,"Falta la condici�n en la estructura Mientras."); errores++; }
						// comprobar que no halla espacios
						comillas=-1;
						for (int tmp1=0;tmp1<(int)str.size();tmp1++) {
							if (str[tmp1]=='\'') comillas=-comillas;
							if (tmp1>0 && tmp1<(int)str.size()-1)
								if (comillas<0 && str[tmp1]==' ' && str[tmp1-1]!='&' && str[tmp1-1]!='|'  && str[tmp1+1]!='&'  && str[tmp1+1]!='|') {
									if (lang[LS_LAZY_SYNTAX]) {
										string aux=str.substr(tmp1); flag_pyc++;
										programa.Insert(x+1,aux);
										break;
									} else {
										SynError (103,"Se esperaba fin de expresion."); errores++;
									}
								}
						}
						if (Lerrores==errores) {
							EvaluarSC(str,tipo,vt_logica);
							if (tipo!=vt_error && !tipo.cb_log) { SynError (104,"No coinciden los tipos."); errores++; }
						}
					}
			}
			if (LeftCompare(instruccion,"FIN") || instruccion=="REPETIR "||instruccion=="BORRARPANTALLA"||instruccion=="ESPERARTECLA") {
				size_t sp=cadena.find(" ",0);
				if (sp!=string::npos && cadena.substr(sp)!=" ;") {
					SynError (105,"La instruccion no debe tener parametros."); errores++;
					cadena.erase(sp,cadena.size()-sp);
				}
			}
			if (instruccion=="Error?" && cadena!="" && cadena!=";") {
				if (LeftCompare(cadena,"FIN "))
				{SynError (99,"Instrucci�n no v�lida."); errores++;}
				else
					{SynError (106,"Instrucci�n no valida."); errores++;}
			}
			// llama directa a un subproceso
			if (instruccion=="INVOCAR ") {
				int p=8;
				string fname=NextToken(cadena,p);
				const Funcion *func=EsFuncion(fname);
				string args=cadena.substr(p);
				if (func->GetTipo(0)!=vt_error && !ignore_logic_errors) {SynError (310,string("La funci�n retorna un valor, debe ser parte de una expresion (")+fname+")."); errores++;}
				if (args==";") args="();"; // para que siempre aparezcan las llaves y se eviten as� problemas
				if (args=="();") {
					if (func->cant_arg!=0 && !ignore_logic_errors) {SynError (264,string("Se esperaban argumentos para el subproceso (")+fname+")."); errores++;}
				} else if (func->cant_arg==0) {
					if (args!="();" && !ignore_logic_errors) {SynError (265,string("El subproceso (")+fname+") no debe recibir argumentos."); errores++;}
				} else if (args[0]!='(' && !ignore_logic_errors) {SynError (266,"Los argumentos para invocar a un subproceso deben ir entre par�ntesis."); errores++;}
				else { // entonces tiene argumentos, y requiere argumentos, ver que la cantidad est� bien
					int args_last_pos=BuscarComa(args,1,args.length()-1,')');
					if (args_last_pos!=-1) { // si faltaban cerrar parentesis, el error salto antes
						int pos_coma=0, last_pos_coma=0, cant_args=0; tipo_var tipo;
						do {
							pos_coma=BuscarComa(args,pos_coma+1,args_last_pos,',');
							if (pos_coma==-1) pos_coma=args_last_pos;
							string arg_actual=args.substr(last_pos_coma+1,pos_coma-last_pos_coma-1);
							if (!SirveParaReferencia(arg_actual)) { // puede ser el nombre de un arreglo suelto, para pasar por ref, y el evaluar diria que faltan los subindices
								if (func->pasajes[cant_args+1]==PP_REFERENCIA && !ignore_logic_errors) { SynError(268,string("No puede utilizar una expresi�n en un pasaje por referencia (")+arg_actual+(")")); errores++; }
								else EvaluarSC(arg_actual,tipo,func->tipos[cant_args+1]);
							}
							cant_args++; last_pos_coma=pos_coma;
						} while (pos_coma!=args_last_pos);
						if (cant_args!=func->cant_arg && !ignore_logic_errors) { SynError(268,string("Cantidad de argumentos incorrecta para el subproceso (")+fname+(")")); errores++; }
						else if (args_last_pos!=int(args.length())-2) {SynError (269,"Se esperaba fin de instrucci�n."); errores++;} // el -2 de la condicion es por el punto y coma
					}
				}
			}
			// Controlar Cierre de Bucles
			if (cadena=="REPETIR") 
			{bucles.push_back(programa.GetLoc(x,"REPETIR"));}
			if (cadena=="FINSEGUN") {
				if (!bucles.empty() && bucles.back()=="SEGUN") {
					bucles.pop_back();
				} else {
					SynError (107,"FINSEGUN mal colocado."); errores++;}
			}
			if (cadena=="FINPARA") {
				if (!bucles.empty() && (bucles.back()=="PARA"||bucles.back()=="PARACADA")) {
					bucles.pop_back();
				} else {
					SynError (108,"FINPARA mal colocado."); errores++;}
			}
			if (cadena=="FINMIENTRAS") {
				if (!bucles.empty() && (bucles.back()=="MIENTRAS")) {
					bucles.pop_back();
				} else {
					SynError (109,"FINMIENTRAS mal colocado."); errores++;}
			}
			if (cadena=="FINSI") {
				if (!bucles.empty() && (bucles.back()=="SI"||bucles.back()=="SINO")) {
					bucles.pop_back();
				} else {
					SynError (110,"FINSI mal colocado."); errores++;}
			}
			if (LeftCompare(cadena,"HASTA QUE ")) {
				if (!bucles.empty() && bucles.back()=="REPETIR") {
					bucles.pop_back();
				} else {
					SynError (111,"HASTA QUE mal colocado."); errores++;}
			}
			if (lang[LS_LAZY_SYNTAX] && LeftCompare(cadena,"MIENTRAS QUE ")) {
				if (!bucles.empty() && bucles.back()=="REPETIR") {
					bucles.pop_back();
				} else {
					SynError (270,"MIENTRAS QUE mal colocado."); errores++;}
			}
			if ( (x>0 && cadena=="SINO" && LeftCompare(programa[x-1],"SI "))
				|| (x>0 && cadena=="SINO" && LeftCompare(programa[x-1],"ENTONCES")) )
			{
				if (!ignore_logic_errors) { SynError (113,"Debe haber acciones en la salida por verdadero."); errores++;}
			}
			
			// Actualiza los vectores
			programa[x]=cadena;
			
			if (flag_pyc==0) /*LineNumber++*/; else flag_pyc-=1;
			// Borra cadenas vacias
			if (cadena.size()==0 || cadena==";") {programa.Erase(x);x--;}
		}
	}
	
	InformUnclosedLoops(bucles,errores);
	
	return SynErrores;
}

int ParseInspection(string &cadena) {
	SynCheckAux1(cadena); // acomodar caracteres
	if (cadena.size() && cadena[cadena.size()-1]==';') cadena.erase(cadena.size()-1,1);
	SynCheckAux2(cadena); // word_operators
	int errores=0, flag_pyc=0;
	SynCheckAux3(-1,cadena,errores,"<-",flag_pyc); // verificar operadores
	if (flag_pyc) { SynError (271,"No puede haber m�s de una expresi�n."); errores++; }
	return errores+flag_pyc;
}

int SynCheck() {
	programa.PushBack(""); // linea en blanco al final, para que era?
	programa.Insert(0,""); // linea en blanco al principio, para que era?
	int errores=0;
	
	if (case_map) for(int i=0;i<programa.GetSize();i++) CaseMapFill(programa[i].instruccion);
	
	// pasar todo a mayusculas, reemplazar tabs, comillas, word_operators, corchetes, y trimear
	for(int i=0;i<programa.GetSize();i++) SynCheckAux1(programa[i].instruccion);
	
	// parsear primero las funciones/subprocesos (j=0), luego el proceso (j=1)
//	bool have_proceso=false;
//	for(int j=0;j<2;j++) {
//		bool era_proceso=false;
//		for(int i0=1,i=0;i<programa.GetSize();i++) {
//			string &s=programa[i].instruccion;
//			if (i==programa.GetSize()-1 || // para que tome el �ltimo proceso/funcion
//				s=="FUNCION" || LeftCompare(s,"FUNCION ") || 
//				s=="FUNCI�N" || LeftCompare(s,"FUNCI�N ") || 
//				s=="PROCESO" || LeftCompare(s,"PROCESO ") || 
//				s=="SUBPROCESO" || LeftCompare(s,"SUBPROCESO ")) {
//					bool es_proceso=(s=="PROCESO" || LeftCompare(s,"PROCESO "));
//					if (j==1 && es_proceso) {
//						if (have_proceso) { Inter.SetLineAndInstructionNumber(i); SynError (272,"Solo puede haber un Proceso."); errores++;}
//						have_proceso=true;
//					}
//					if (i0!=i && era_proceso==(j==1)) {
//						int i1=i;
//						errores+=SynCheck(i0,i1);
//						i=programa.GetRefPoint(); // lo setea el SynCheck, es porque va a agregar y sacar lineas, entonces i ya no ser� i
//						
//						if (era_proceso) { // los cambios de lineas pueden afectar a funciones ya registradas en la pasada anterior
//							map<string,Funcion*>::iterator it1=subprocesos.begin(), it2=subprocesos.end();
//							while (it1!=it2) { 
//								if (!it1->second->func && it1->second->line_start>=i1) 
//									it1->second->line_start+=i-i1; 
//								++it1; 
//							} 
//						}
//						
//					}
//					i0=i; era_proceso=es_proceso;
//				}
//		}
//	}
//	if (!have_proceso) { Inter.SetLineAndInstructionNumber(1); SynError (273,"Debe haber un Proceso."); errores++;}
	
	bool have_proceso=false;
	for(int i=0;i<programa.GetSize();i++) {
		string &s=programa[i].instruccion;
		string fw=FirstWord(s);
		if (fw=="FUNCION"||fw=="FUNCI�N"||fw=="PROCESO"||fw=="SUBPROCESO") {
				Inter.SetLineAndInstructionNumber(i);
				bool es_proceso=(fw=="PROCESO");
				if (s==fw) s+=" ";
				Funcion *func=ParsearCabeceraDeSubProceso(s.substr(fw.size()+1),es_proceso,errores);
				func->userline_start=Inter.GetLineNumber();
				subprocesos[func->id]=func;
				if (es_proceso) { // si es el proceso principal, verificar que sea el unico, y guardar el nombre en main_process_name para despues saber a cual llamar
					if (have_proceso) { SynError (272,"Solo puede haber un Proceso."); errores++;}
					main_process_name=func->id;
					have_proceso=true;
				} else if (!lang[LS_ENABLE_USER_FUNCTIONS])
					{ SynError (309,"Este perfil no admite SubProcesos."); errores++;}
			}
	}
	errores=SynCheck(0,programa.GetSize());
	
	if (!have_proceso) { Inter.SetLineAndInstructionNumber(0); SynError (273,"Debe haber un Proceso."); errores++;}
	
#ifdef _DEBUG
	string del_ccl="<<<delete coloquial_conditions_list>>>";
	SynCheckAux2(del_ccl);
#endif
	
	return errores;
}
