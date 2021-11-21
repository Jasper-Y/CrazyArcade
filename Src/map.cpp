#include "map.h"
using namespace std;
using namespace chrono;


class CharBitmap
{
protected:
	// Initialization can be done in the RAII style (writing initial values in here),
	// or can be done in the constructor.  Up to the student.
	int wid = 0, hei = 0;
	char* pix = nullptr;
public:
	CharBitmap();
	~CharBitmap();
	void CleanUp(void);
	void Create(int w, int h);
	void SetPixel(int x, int y, int p);

	// The return data type of GetPixel can be any integral data type.
	char GetPixel(int x, int y) const;

	void Draw(YsRawPngDecoder* png1) const;
};

CharBitmap::CharBitmap()
{
	wid = 0;
	hei = 0;
	pix = nullptr;
}

CharBitmap::~CharBitmap()
{
	CleanUp();
}

void CharBitmap::CleanUp(void)
{
	wid = 0;
	hei = 0;
	if (nullptr != pix)
	{
		delete[] pix;
	}
	pix = nullptr;
}

void CharBitmap::Create(int w, int h)
{
	CleanUp();

	pix = new char[w * h];
	wid = w;
	hei = h;

	for (int i = 0; i < wid * hei; ++i)
	{
		pix[i] = 0;
	}
}

void CharBitmap::SetPixel(int x, int y, int p)
{
	if (0 <= x && x < wid && 0 <= y && y < hei)
	{
		pix[y * wid + x] = p;
		cout << p<<"setpix";
	}
}

char CharBitmap::GetPixel(int x, int y) const
{
	if (0 <= x && x < wid && 0 <= y && y < hei)
	{
		return pix[y * wid + x];
	}
	return 0;
}

void CharBitmap::Draw(YsRawPngDecoder *png1) const
{
	int k = 60;
	

	for (int y = 0; y < hei; ++y)
	{
		for (int x = 0; x < wid; ++x)
		{
			auto pix = GetPixel(x, y);
		
			
			switch (pix)
			{
			case 1:
				
				glRasterPos2d(x*60, (y+1)*60);
				glDrawPixels(png1[0].wid, png1[0].hei, GL_RGBA, GL_UNSIGNED_BYTE, png1[0].rgba);
				break;
			case 2:
				glRasterPos2d(x * 60, (y + 1) * 60);
				glDrawPixels(png1[1].wid, png1[1].hei, GL_RGBA, GL_UNSIGNED_BYTE, png1[1].rgba);
				break;
			/*case 3:
				glDrawPixels(png1[2].wid, png1[2].hei, GL_RGBA, GL_UNSIGNED_BYTE, png1[2].rgba);
				break;
			case 4:
				glDrawPixels(png1[3].wid, png1[3].hei, GL_RGBA, GL_UNSIGNED_BYTE, png1[3].rgba);
				break;*/
			case 0:
				
				break;
			default:
				break;
			}
			// If you use switch & case, that's ok, too.
		

			
		}
	}

	
}


int main(void)
{
	int wid = 0, hei = 0;

	wid = 15;
	hei = 13;
	CharBitmap bmp;
	bmp.Create(wid, hei);
	int k = 60;
	FsOpenWindow(0, 0, wid * k, hei * k, 1);


	YsRawPngDecoder pngbackgroud;
	if (YSOK == pngbackgroud.Decode("background.png"))
	{
		pngbackgroud.Flip();
		printf("%d x %d\n", pngbackgroud.wid, pngbackgroud.hei);
	}
	else
	{
		printf("Could not read the image.\n");
	}


	



	YsRawPngDecoder png[2];
	if (YSOK == png[0].Decode("solid.png"))
	{
		png[0].Flip();
		printf("%d x %d\n", png[0].wid, png[0].hei);
	}
	else
	{
		printf("Could not read the image.\n");
	}

	if (YSOK == png[1].Decode("breakable.png"))
	{
		png[1].Flip();
		printf("%d x %d\n", png[1].wid, png[1].hei);
	}
	else
	{
		printf("Could not read the image.\n");
	}

	/*if (YSOK == png[2].Decode("001000.png"))
	{
		png[2].Flip();
		printf("%d x %d\n", png[2].wid, png[2].hei);
	}
	else
	{
		printf("Could not read the image.\n");
	}

	if (YSOK == png[3].Decode("001000.png"))
	{
		png[3].Flip();
		printf("%d x %d\n", png[3].wid, png[3].hei);
	}
	else
	{
		printf("Could not read the image.\n");
	}*/
	/*if (YSOK == png[4].Decode("001000.png"))
	{
		png[2].Flip();
		printf("%d x %d\n", png[4].wid, png[4].hei);
	}
	else
	{
		printf("Could not read the image.\n");
	}*/


	






	for (;;)
	{
		FsPollDevice();
		auto key = FsInkey();

		int lb, mb, rb, mx, my;
		auto evt = FsGetMouseEvent(lb, mb, rb, mx, my);

		if (FSKEY_ESC == key)
		{
			break;
		}
		if (FSKEY_0 <= key && key <= FSKEY_7)
		{
			int px = mx / k;
			int py = my / k;
			int colorCode = key - FSKEY_0;
			bmp.SetPixel(px, py, colorCode);
		}


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glRasterPos2d(0, 779);
		
		glDrawPixels(pngbackgroud.wid, pngbackgroud.hei, GL_RGBA, GL_UNSIGNED_BYTE, pngbackgroud.rgba);
		glRasterPos2d(100, 100);
		bmp.Draw(png);

		//glColor3ub(255, 255, 255); // Make sure the lattice is white.
		//glBegin(GL_LINES);
		//// Since the window size must match the bitmap size, the loop condition can be <= or <.
		//// The last line won't be visible anyway.
		//for (int x = 0; x <= wid; ++x)
		//{
		//	glVertex2i(x * k, 0);
		//	glVertex2i(x * k, hei * k);
		//}
		//for (int y = 0; y <= hei; ++y)
		//{
		//	glVertex2i(0, y * k);
		//	glVertex2i(wid * k, y * k);
		//}
		//glEnd();

		
		FsSwapBuffers();

		FsSleep(25);
	}
}
