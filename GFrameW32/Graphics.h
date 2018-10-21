#pragma once

// Graphics processing handler declarations
bool gfInitScene();
void gfDrawScene();
void gfCleanupScene();
void gfOnLMouseClick( int x, int y );
void gfOnRMouseClick( int x, int y );
void gfOnKeyDown( UINT key );
void gfOnKeyUp( UINT key );
void DrawPolygon (int x0, int y0, int x1, int y1, RGBPIXEL color);