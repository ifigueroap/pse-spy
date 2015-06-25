#include <iostream>
#include <cstdlib>
#include "GLstuff.h"
#include "Entity.h"
#include "Global.h"
#include "Draw.h"
using namespace std;

static int bk_x0,bk_x1,bk_y0,bk_y1;

static void DrawRectangle(const float *color, int x, int y, int wl, int wr, int h) {
	glColor3fv(color);
	glBegin(GL_LINE_LOOP);
	glVertex2i(x-wl,y);
	glVertex2i(x-wl,y-h);
	glVertex2i(x+wr,y-h);
	glVertex2i(x+wr,y);
	glEnd();
}

static void DrawSolidRectangle(const float *color, int x, int y, int wl, int wr, int h) {
	glColor3fv(color);
	glBegin(GL_QUADS);
	glVertex2i(x-wl,y);
	glVertex2i(x-wl,y-h);
	glVertex2i(x+wr,y-h);
	glVertex2i(x+wr,y);
	glEnd();
}

static void DrawBackground() {
	int bk_xm=(bk_x0+bk_x1)/2, bk_w=(bk_x1-bk_x0)/2;
	DrawSolidRectangle(color_shape[ET_COUNT],bk_xm,bk_y1,bk_w,bk_w,bk_y1-bk_y0);
	bk_x0=bk_x1=start->d_x;
	bk_y0=bk_y1=start->d_y;
}

static void DrawTextNS(const float *color, int x, int y, string label) {
	glColor3fv(color);
	glPushMatrix();
	glTranslated(x,y,0);
	glScaled(.105,.15,.1);
	for (unsigned int i=0;i<label.size();i++)
		dibujar_caracter(label[i]);
	glPopMatrix();
}

void Entity::DrawNassiSchne(bool force) {
	if (this==start) DrawBackground();
	if (mouse==this) DrawSolidRectangle(color_shape[ET_COUNT],d_fx,d_fy,d_bwl,d_bwr,d_bh);
	if (type!=ET_AUX_PARA&&type!=ET_OPCION)
		if (!nolink || mouse==this) //todo: comentar esto y ver porque se estiran tanto los hijos
			DrawRectangle(color_border,d_fx,d_fy,d_bwl,d_bwr,d_bh);
	// guardar bb para el gran retangulo blanco de fondo
	if (d_fy>bk_y1) bk_y1=d_fy;
	if (d_fy-d_h<bk_y0) bk_y0=d_fy-d_h;
	if (d_fx-d_bwl<bk_x0) bk_x0=d_fx-d_bwl;
	if (d_fx+d_bwr>bk_x1) bk_x1=d_fx+d_bwr;
	if (this==mouse)
		DrawRectangle(color_shadow,d_dx+fx,d_dy+fy,bwl,bwr,bh);
	if (!nolink) {
		if (type==ET_OPCION) {
			glColor3fv(color_border);
			glBegin(GL_LINES);
//			if (edit_on) { glVertex2i(d_x-d_bwl+flecha_w,d_y); glVertex2i(d_x-d_bwl+flecha_w,d_y-h); }
			int px=parent->child_dx[child_id]-bwl;
			int px0=-parent->bwl, px1=parent->child_dx[parent->n_child-1]-parent->child[parent->n_child-1]->bwl;
			int ax=px1-px; if (ax<0) ax=-ax;
			if (px1==px0) px1++;
			int ah=ax*5*h/(px1-px0)/2;
			glVertex2i(d_x-d_bwl,d_y-h); glVertex2i(d_x-d_bwl,d_y-h/2+ah);
			if (!child[0]) { glVertex2i(d_x-d_bwl,d_y-h); glVertex2i(d_x+d_bwr,d_y-h); }
			glEnd();
//			glVertex2i((child[0]?child[0]->d_x:d_x),d_y-d_h); glVertex2i((child[0]?child[0]->d_x:d_x),d_y-d_h-flecha_h);
		} else if (type==ET_SEGUN) {
			glColor3fv(color_border);
			glBegin(GL_LINE_STRIP);
			glVertex2i(d_x-d_bwl,d_y);
			int px=d_x;
			px+=child_dx[n_child-1]-child[n_child-1]->d_bwl;
			glVertex2i(px,d_y-5*h/2);
			glVertex2i(d_x+d_bwr,d_y);
			glEnd();
			glBegin(GL_LINES);
			for(int i=0;i<n_child-1;i++) { 
				glVertex2i(d_x+child_dx[i]+child[i]->bwr,child[i]->d_y-child[i]->d_h);
				glVertex2i(d_x+child_dx[i]+child[i]->bwr,d_y-d_bh);
			}
			glEnd();
			
		} else 
		if (type==ET_SI) {
			DrawTextNS(color_arrow,d_x-d_bwl+10,d_y-2*h,"Si");
			DrawTextNS(color_arrow,d_x+d_bwr-35,d_y-2*h,"No");
			glColor3fv(color_border);
			glBegin(GL_LINE_STRIP);
			glVertex2i(d_x-d_bwl,d_y);
			int px=d_x;
			if (child[1]) px+=child_dx[1]+child[1]->d_bwr;
			else if (child[0]) px+=child_dx[0]-child[0]->d_bwl;
			glVertex2i(px,d_y-2*h-margin);
			glVertex2i(d_x+d_bwr,d_y);
			glEnd();
			glBegin(GL_LINES);
			glVertex2i(d_x-d_bwl,d_y-2*h-margin);
			glVertex2i(d_x+d_bwr,d_y-2*h-margin);
			glEnd();
		}
	}

	if (type==ET_OPCION) { // + para agregar opciones
		if (edit_on && mouse!=this) {
			glBegin(GL_LINES);
			glColor3fv(color_label);
			glVertex2i(d_x-d_bwl+3*flecha_w/4,d_y-d_h/2); glVertex2i(d_x-d_bwl+1*flecha_w/4,d_y-d_h/2);
			glVertex2i(d_x-d_bwl+flecha_w/2,d_y-1*d_h/3); glVertex2i(d_x-d_bwl+flecha_w/2,d_y-2*d_h/3);
			glEnd();
		}
	}
//	// texto;
	DrawText();
}

void Entity::CalculateNassiSchne() { // calcula lo propio y manda a calcular al siguiente y a sus hijos, y acumula en gw,gh el tama�o de este item (para armar el tama�o del bloque)
	
	// calcular tama�os de la forma segun el texto
	if (!t_w) w=margin*6; else { w=t_w; w+=2*margin; } h=t_h+2*margin; 
	t_dy=t_dx=0; fx=x; fy=y; bh=h; bwr=bwl=w/2; // esto es si fuera solo la forma
	// si son estructuras de control...
	if (!nolink && n_child) {
		if (type==ET_OPCION) {
			bwr=bwl=(w=t_w+2*margin)/2;
			if (edit_on) 
			{ bwr+=flecha_w; bwl+=flecha_w; } // el + para agregar opciones
			child_dx[0]=0; child_bh[0]=0;
			if (child[0]) {
				child[0]->x=x; child[0]->y=y-bh;
				int cwl=0,cwr=0,ch=0;
				child[0]->Calculate(cwl,cwr,ch);
				bh+=ch; 
				if (cwl+cwr>bwl+bwr) bwl=bwr=(cwl+cwr)/2;
				else child[0]->ResizeW(bwl+bwr,false);
				child_dx[0]-=(cwr-cwl)/2;
				child[0]->MoveX(child_dx[0]);
			}
			// el ancho se lo define el segun padre
			w=bwl+bwr;
		} else
		if (type==ET_SEGUN) {
			bh+=h;
			bwr=bwl=(w=t_w*2)/2;
			int sw=0, sh=0;
			for (int i=0;i<n_child;i++) {
				int cwl=0,cwr=0,ch=0;
				child[i]->x=0; child[i]->y=y-2*h;
				child[i]->Calculate(cwl,cwr,ch);
				child_bh[i]=ch; child_dx[i]=sw+cwl;
				sw+=cwl+cwr;
				if (ch>sh) sh=ch;
			}
			if (sw>w) w=sw; 
			bwr=bwl=w/2; 
			bh+=sh;
			for (int i=0;i<n_child;i++) {
				child_dx[i]-=w/2;
				child[i]->MoveX(child_dx[i]);
			}
			Entity *dom=child[n_child-1];
			if (dom->x+dom->bwr<x+bwr) {
				int dif=(x+bwr)-(dom->x+dom->bwr);
				dom->bwl+= dif/2;
				dom->bwr+= dif/2;
				dom->w+= dif;
				dom->MoveX(dif/2);
				child_dx[n_child-1]+=dif/2;
			}
		} else 
		if (type==ET_SI) {
			bh*=2;
			bh+=margin;
			// calcular tama�o hijo por verdadero
			int c1l=0,c1r=0,c1h=0;
			if (child[0]) {
				child[0]->y=y-bh; child[0]->x=x;
				child[0]->Calculate(c1l,c1r,c1h);
			} else {
				c1r=c1l=c1h=20;
			}
			// calcular tama�o hijo por falso
			int c2l=0,c2r=0,c2h=0;
			if (child[1]) {
				child[1]->y=y-bh; child[1]->x=x;
				child[1]->Calculate(c2l,c2r,c2h);
			} else {
				c2r=c2l=c2h=20;
			}
			// ajustar tama�o propio
			if (c1r+c1l+c2r+c2l>w) {
				bwl=bwr=(c1r+c1l+c2r+c2l)/2;
			} else  {
				int dw=(w-(c1r+c1l+c2r+c2l))/2;
				if (child[0]) child[0]->ResizeW(c1r+c1l+dw,false);
				if (child[1]) child[1]->ResizeW(c2r+c2l+dw,false);
				c1l+=dw/2; c1r+=dw/2; c2l+=dw/2; c2r+=dw/2;
			}
			bh += (c1h>c2h?c1h:c2h);
			child_bh[0]=c1h; child_bh[1]=c2h;
			// mover hijos horizontalmente
			child_dx[0] = bwr-c1r;
			child_dx[1] = -bwl+c2l;
			if (child[0]) child[0]->MoveX(child_dx[0]);
			if (child[1]) child[1]->MoveX(child_dx[1]);
		} else
		if (type==ET_MIENTRAS||type==ET_PARA||type==ET_REPETIR) {
			if (type==ET_PARA) {
				child[1]->CalculateNassiSchne();
				child[2]->CalculateNassiSchne();
				child[3]->CalculateNassiSchne();
				t_dx=child[1]->t_w+child[2]->t_w+child[3]->t_w;
				w=w+t_dx; bwr=bwl=w/2; t_dx=-t_dx/2;
			}
			// calcular tama�o hijo por verdadero
			int c1l=0,c1r=0,c1h=0;
			if (child[0]) {
				child[0]->y=y-(type==ET_REPETIR?0:bh); child[0]->x=x;
				child[0]->Calculate(c1l,c1r,c1h);
			} else {
				c1r=c1l=c1h=20;
			}
			// ajustar tama�o propio
			if (c1r+c1l+40>w) 
				bwl=bwr=(c1r+c1l)/2+20;
			else
				if (child[0]) child[0]->ResizeW(w-40,false);
			bh+=c1h;
			child_bh[0]=0;
			if (child[0]) child[0]->MoveX(child_dx[0]=20);
			if (type==ET_REPETIR) t_dy=-c1h;
			else if (type==ET_PARA) { // acomodar las etiquetas "desde .. hasta .. con paso .."
				child[1]->y=child[1]->fy=fy;
				child[1]->x=child[1]->fx=fx+t_w/2+t_dx+child[1]->t_w/2;
				child[3]->y=child[3]->fy=fy;
				child[3]->x=child[3]->fx=child[1]->fx+child[1]->t_w/2+child[1]->t_dx+child[3]->t_w/2;
				child[2]->y=child[2]->fy=fy;
				child[2]->x=child[2]->fx=child[3]->fx+child[3]->t_w/2+child[3]->t_dx+child[2]->t_w/2;
			}
		} 
	}
	if (!next && prev) {
		int max=bwl+bwr;
		Entity *et=prev;
		while (et) {
			if (et->bwl+et->bwr>max)
				max=et->bwl+et->bwr;
			et=et->prev;
		}
		ResizeW(max,true);
	}
}
