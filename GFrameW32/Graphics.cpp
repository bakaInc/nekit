#include "StdAfx.h"
#include "GF.h"

#ifndef M_PI
const double M_PI = 3.1415926535897932384626433832795;
#endif



void DrawLine( int x0, int y0, int x1, int y1, RGBPIXEL color ){
	
	    int t, distаnce = 0;
		int xerr = 0, yerr = 0, deltaX, deltaY;
		int x, y;

		deltaX = x1 - x0;
		deltaY = y1 - y0;
		/* определение направления шага,
		шаг вычисляется либо по вертикальной, либо горизонтальной
		линии */
		if (deltaX>0) x = 1;
		else if (deltaX == 0) x = 0;
		else x = -1;
		if (deltaY>0) y = 1;
		else if (deltaY == 0) y = 0;
		else y = -1;
		/* определение какое расстояние больше */
		deltaX = abs (deltaX);
		deltaY = abs (deltaY);
		if (deltaX > deltaY) { distаnce = deltaX; }
		else { distаnce = deltaY; }
		/* вычерчивание линии */
		for (t = 0; t <= distаnce + 1; t++) {
			gfSetPixel (x0, y0, color);
			xerr += deltaX;
			yerr += deltaY;
			if (xerr>distаnce) {
				xerr -= distаnce;
				x0 += x;
			}
			if (yerr>distаnce) {
				yerr -= distаnce;
				y0 += y;
			}
		}

}


void DrawPolygon (double *x, double *y, int count, RGBPIXEL color) {

	for (int i = 0; i < count - 1; i++) {
		DrawLine (x[i], y[i], x[i + 1], y[i + 1], color);
	}
	DrawLine (x[count-1], y[count-1], x[0], y[0], color);
	
}


int ClassifyLeftRight (double x1, double y1, double x2, double y2, double x, double y) {
	double ax = x2 - x1;
	double ay = y2 - y1;
	double bx = x - x1;
	double by = y - y1;

	double s =  ax*by - bx*ay;
	if (s < 0) { return -1; }
	if (s > 0) { return 1; }
	return 0;
}



int Classify (double x1, double y1, double x2, double y2, double x, double y) {
	double ax = x2 - x1;
	double ay = y2 - y1;
	double bx = x - x1;
	double by = y - y1;
	double s = ax * by - bx * ay;

	if (s < 0) { return -1; }
	if (s > 0) { return 1; }

	if ((ax*bx < 0) || (ay*by) < 0) { return  2; }	//BEHIND
	if ((ax*ax + ay*ay < 0) < (bx*bx + by*by) < 0) { return 3; }	//BEYOND
	if ((x == x1) && (y == y1)) { return 4; }	//ORIGIN
	if ((x == x2) && (y == y2)) { return 5; }	//DESTINATIONAL

	return 0;

}


#define SWOLLEN 1;
#define NOT_SWOLLEN 0;
int getTypePolygon (double *x, double *y, int count) {
	int sum = 0;
	for (int i = 0; i < count - 1; i++) {
		for (int j = 0; j < count; j++) {
			int temp = ClassifyLeftRight (x[i], y[i], x[i + 1], y[i + 1], x[j], y[j]); 
			sum += temp;
		}
		if (abs (sum) != count - 2) { return NOT_SWOLLEN; }// Не выпухлый
		sum = 0;
	}
	sum = 0;
	for (int j = 0; j < count; j++) {
		sum += ClassifyLeftRight (x[0], y[0], x[count-1], y[count-1], x[j], y[j]);
	}
	if (abs (sum) != count - 2) { return NOT_SWOLLEN; } //Не Выпухлый

	return SWOLLEN; //Выпухлый
}


#define PARALLEL 1;
#define COLLINIAR -1;
#define INTERSECT 11;//Пересекаются
int Interset (double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy) {
	double nx = dx - cx;
	double ny = dy - cy;
	double r = (ay - cy)*nx - (ax - cx)*ny;
	double s = (bx - ax)*ny - (by - ay)*nx;

	if (s < 0.00001) { return PARALLEL; }//PARALLEL
	if (r < 0.00001) { return COLLINIAR; }//COLLINIAR
	double denum = r / s;
	
	double kk = (ay - cy)*(bx - ax) - (ax - cx)*(by - ay);
	double num = kk / s;

	if (denum > 0 && denum < 1 && num >0 && num < 1) { return INTERSECT; }


	return false;
}

#define COMPLEX 1;
#define SIMPLE 0;
int getDifficultyOfPolygon (double *x, double *y, int count) {

	for (int i = 0; i < count - 1; i++) {
		for (int j = 0; j < count - 1; j++) {
			double t[8] = { x[i], y[i], x[i + 1], y[i + 1], x[j], y[j], x[j + 1], y[j + 1] };
		}
	}
	for (int i = 0; i < count - 1; i++) {
		for (int j = 0; j < count - 1; j++) {
			if (i == j) { continue; }
			int reg = Interset (x[i], y[i], x[i + 1], y[i + 1], x[j], y[j], x[j + 1], y[j + 1]);
			if(reg == 11) { return COMPLEX; }//Сложный полигон
		}
	}
	for (int j = 1; j < count - 2; j++) {
		if (Interset (x[count-1], y[count-1], x[0], y[0], x[j], y[j], x[j + 1], y[j + 1]) == 11) { return COMPLEX; }//Сложный полигон

	}

	return SIMPLE;//простой полигон
	
}


#define CROSSLEFT 1;
#define CROSSRIGHT -1;
#define INESENTIAL 0;
#define TOUCHING 2;
int EdjeType (double xa, double ya, double xc, double yc, double xd, double yd) {
	
	int type = Classify (xc, yc, xd, yd, xa, ya);
	
		if (type == 1) {//LEFT
			if (ya > yc && ya <= yd) { return CROSSLEFT; }
			else { return INESENTIAL; }
		}
		if (type == -1) {//RIGHT
			if (ya > yd && yc >= ya) { return CROSSRIGHT; }
			else { return INESENTIAL; }
		}
		if (type == 2 || type == 3 || type == 4 || type == 5) {//BETWEEN ORIGIN DESTINATIONAL ...
			return TOUCHING;
		}
		return INESENTIAL;
}

#define INSIDE 1;
#define OUTSIDE 0;
int Polygon_Even_Odd (double x, double y, double *px,double *py,int n) {
	int EO = 0;
	for (int i = 0; i < n; i++) {
		switch (EdjeType (x, y, px[i], py[i], px[(i + 1) % n], py[(i + 1) % n])) {
			case -1: EO++; break;   //CROSSRIGHT
			case 1: EO++; break;  //CROSSLEFT
			default: break;
		}
	}
	if (EO & 1) { return INSIDE; }//Четное,нечетное
	return OUTSIDE;
}

int Polygon_NON_ZERO_WINDING (double x, double y, double *px, double *py, int n) {
	int EO = 0;
	for (int i = 0; i < n; i++) {
		switch (EdjeType (x, y, px[i], py[i], px[(i + 1) % n], py[(i + 1) % n])) {
		case -1: EO++; break;//CROSSRIGHT
		case 1: EO--; break;//CROSSLEFT
		default: break;
		}
	}

	if (EO != 0) { return INSIDE; }
	return OUTSIDE;
}

void Draw_Polygon_NON_ZERO_WINDING ( double *px, double *py, int n, RGBPIXEL color) {
	int maxX = gfGetWindowWidth ();
	int maxY = gfGetWindowHeight ();
	for (int i = 0; i < maxX; i++) {
		for (int j = 0; j < maxY; j++) {
			if (Polygon_NON_ZERO_WINDING (i, j, px, py, n)) { gfSetPixel (i, j, color); };//INSIDE
		}
	}
}

void  Draw_Polygon_EO ( double *px, double *py, int n, RGBPIXEL color) {
	int maxX = gfGetWindowWidth ();
	int maxY = gfGetWindowHeight ();
	for (int i = 0; i < maxX; i++) {
		for (int j = 0; j < maxY; j++) {
			if (Polygon_Even_Odd (i, j, px, py, n)) { gfSetPixel (i, j, color); };//INSIDE
		}
	}
}

// Вызывается один раз в самом начале при инициализации приложения
bool gfInitScene()
{
	gfSetWindowSize (900, 900);
	gfClearScreen (RGBPIXEL::Black ());

	double erX[4] = { 10,150,150,10 };
	double erY[4] = { 10,10,150,150 };

	int typeDiff0 = getDifficultyOfPolygon (erX, erY, sizeof (erX) / sizeof (erX[0]));  //Сложный - 1 Простой - 0
	int typeT0 = getTypePolygon (erX, erY, sizeof (erX) / sizeof (erX[0]));  // Выпухлый - 1, нет - 0


	double x[5] = { 50,250,450,50,450 };
	double y[5] = { 450,50,450,200,200 };
	
	int typeDiff1 = getDifficultyOfPolygon (x, y, sizeof (x) / sizeof (x[0]));  //Сложный - 1 Простой - 0
	int typeT1 = getTypePolygon (x, y, sizeof (x) / sizeof (x[0]));  // Выпухлый - 1, нет - 0

	double xtest[6] = { 50,70,120,330,350,200 };
	double ytest[10] = { 300,200,100,200,250,300 };
	
	int typeDif2f = getDifficultyOfPolygon (xtest, ytest, sizeof (xtest) / sizeof (xtest[0]));
	int typeT2 = getTypePolygon (xtest, ytest, sizeof (xtest) / sizeof (xtest[0]));


	//DrawLine (x[0], y[0], x[1], y[1], RGBPIXEL::Red ());
	//DrawLine (x[2], y[2], x[3], y[3], RGBPIXEL::Blue ());


	DrawPolygon (x, y,sizeof(x)/sizeof(x[0]), RGBPIXEL::Blue ());
	
	//Draw_Polygon_NON_ZERO_WINDING (x, y, sizeof (x) / sizeof (x[0]), RGBPIXEL (255, 255, 0));
	Draw_Polygon_EO (x, y, sizeof (x) / sizeof (x[0]), RGBPIXEL (255, 255, 0));
	

    return true;
}

// Вызывается в цикле до момента выхода из приложения.
// Следует использовать для создания анимационных эффектов
void gfDrawScene()
{
	
    //static int x = 0;
    //gfDrawRectangle(x, 100, x + 50, 130, RGBPIXEL::Blue());
    //x = (x + 1) % gfGetWindowWidth() ;

    //int x = gfGetMouseX(),
    //    y = gfGetMouseY();
    //gfDrawRectangle(x - 10, y - 10, x + 10, y + 10, RGBPIXEL::Green());
}

// Вызывается один раз перед выходом из приложения.
// Следует использовать для освобождения выделенных
// ресурсов (памяти, файлов и т.п.)
void gfCleanupScene()
{
}

// Вызывается когда пользователь нажимает левую кнопку мыши
void gfOnLMouseClick( int x, int y )
{
    x; y;
    gfDrawRectangle(x - 10, y - 10, x + 10, y + 10, RGBPIXEL::Green());
}

// Вызывается когда пользователь нажимает правую кнопку мыши
void gfOnRMouseClick( int x, int y )
{
    x; y;
}

// Вызывается когда пользователь нажимает клавишу на клавиатуре
void gfOnKeyDown( UINT key )
{
    key;

    if( key == 'A' )
        gfDisplayMessage( "'A' key has been pressed" );
}

// Вызывается когда пользователь отжимает клавишу на клавиатуре
void gfOnKeyUp( UINT key )
{
    key;

    //if( key == 'B' )
    //    gfDisplayMessage( "'B' key has been un-pressed" );
}
