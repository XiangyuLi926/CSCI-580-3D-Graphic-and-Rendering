// FractTerrain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <stdlib.h>

#define W 65
#define H 65

using namespace std;

class Point
{
public:
	double x;
	double y;
	double z;
	double normX;
	double normY;
	double normZ;
	double u;
	double v;

	Point()
	{
		x = 0; y = 0; z = 0;
	}

	Point(double a, double b, double c)
	{
		x = a; y = b; z = c;
	}
};
class Triangle
{
public:
	Point a, b, c;

	Triangle(Point x, Point y, Point z)
	{
		a = x; b = y; c = z;
	}
};

void genNormal(float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3,
	float* normal)
{
	float   len;
	float   v1x, v1y, v1z;
	float   v2x, v2y, v2z;


	v1x = x2 - x1;
	v1y = y2 - y1;
	v1z = z2 - z1;
	v2x = x3 - x1;
	v2y = y3 - y1;
	v2z = z3 - z1;

	normal[0] = v1y*v2z - v1z*v2y;
	normal[1] = v1z*v2x - v1x*v2z;
	normal[2] = v1x*v2y - v1y*v2x;

	len = (float)sqrt(normal[0] * normal[0] + normal[1] * normal[1] +
		normal[2] * normal[2]);

	normal[0] /= len;
	normal[1] /= len;
	normal[2] /= len;
}



class Program
{
public:
	double matrix[W][H];

	Program()
	{
		for (int i = 0; i < W; i++)
		{
			for (int j = 0; j < H; j++)
				matrix[i][j] = 1;
		}
	}

	void iterate()
	{
		int counter = 0;
		double smoothness = 1;
		//int numIteration = (int)(Math.Log(matrix.GetLength(0)) / 0.693); //Natural log of 2 = 0.693
		int numIteration = 6;
		while (counter <= numIteration)
		{
			diamond(counter, smoothness);
			square(counter, smoothness);
			counter += 1;
		}

	}

	double squareHelper(int counter, double smoothness, double a, double b, double c, double d)
	{
		double average = (a + b + c + d) / 4;
		// FIXME: May be cause of range problem

		double random = ((double)rand() / (double)RAND_MAX);
		int sign = ((double)rand() / (double)RAND_MAX) > 0.5 ? 1 : -1;
		// cout << random << "\n";
		//double offset = random*(150 / (counter + 1));// -counter;

		//double sign = random.NextDouble() > 0.5 ? 1 : -1;
		double reduce = 1;
		for (int i = 0; i < counter; ++i)
		{
			reduce *= pow(2, -smoothness);
		}
		double offset = sign * random * reduce;





		return (average + offset);

	}

	void diamond(int counter, double smoothness)
	{
		int length = W;
		int terrainSize = length - 1;
		double numberSegements = pow(2, (counter - 1));
		int span = terrainSize / (int)(numberSegements + 0.5);
		int half = span / 2;
		for (int x = 0; x < terrainSize; x += span)
		{
			for (int y = 0; y < terrainSize; y += span)
			{
				//  (x, y)
				//    \
								                //     a---b---c
				//     |   |   |
				//     d---e---f
				//     |   |   |
				//     g---h---i
				// 
				//     \___ ___/
				//         V
				//       span 
				// 
				double va = matrix[x][y];
				double vc = matrix[x + span][y];
				double ve = matrix[x + half][y + half];
				double vg = matrix[x][y + span];
				double vi = matrix[x + span][y + span];

				// set center height
				matrix[x + half][y + half] = squareHelper(counter, smoothness, va, vc, vg, vi);

			}
		}
	}
	void square(int counter, double smoothness)
	{
		int length = W;
		int terrainSize = length - 1;
		double numberSegements = pow(2, (counter - 1));
		int span = terrainSize / (int)(numberSegements + 0.5);
		int half = span / 2;

		// enumerate sub-dimaonds 
		for (int x = 0; x < terrainSize; x += span)
		{
			for (int y = 0; y < terrainSize; y += span)
			{
				// for each sub-square, the height of the center is caculated
				// by averaging the height of its four vertices plus a random offset.
				// for example, 
				//       h = avg(g, c, i, m) + random;
				//       f = avg(a, g, k, i) + random;
				//       j = f;
				//
				//  (x, y)
				//    \
								                //     a---b---c---d---e
				//     | \ | / | \ | / |
				//     f---g---h---i---j
				//     | / | \ | / | \ |
				//     k---l---m---n---o
				//     | \ | / | \ | / |
				//     p---q---r---s---t
				//     | / | \ | / | \ |
				//     u---v---w---x---y
				// 
				//     \___ ___/
				//         V
				//       span 
				// 
				double va = matrix[x][y];
				double vb = matrix[x + half][y];
				double vc = matrix[x + span][y];
				double vf = matrix[x][y + half];
				double vg = matrix[x + half][y + half];
				double vh = matrix[x + span][y + half];
				double vk = matrix[x][y + span];
				double vl = matrix[x + half][y + span];
				double vm = matrix[x + span][y + span];

				// right of h
				int _vhr[] = { (x + half * 3), (y + half) };

				if (_vhr[0] > terrainSize)
					_vhr[0] = half;

				// left of f
				int _vfl[] = { (x - half), (y + half) };
				if (_vfl[0] < 0)
					_vfl[0] = terrainSize - half;

				// under l
				int _vlu[] = { (x + half), (y + half * 3) };
				if (_vlu[1] > terrainSize)
					_vlu[1] = half;

				// above b
				int _vba[] = { (x + half), (y - half) };
				if (_vba[1] < 0)
					_vba[1] = terrainSize - half;


				double vhr = matrix[_vhr[0]][_vhr[1]];
				double vfl = matrix[_vfl[0]][_vfl[1]];
				double vlu = matrix[_vlu[0]][_vlu[1]];
				double vba = matrix[_vba[0]][_vba[1]];

				matrix[x][y + half] = squareHelper(counter, smoothness, va, vg, vk, vfl); //vf
				matrix[x + half][y] = squareHelper(counter, smoothness, va, vba, vc, vg); //vb
				matrix[x + span][y + half] = squareHelper(counter, smoothness, vc, vhr, vm, vg); //vh
				matrix[x + half][y + span] = squareHelper(counter, smoothness, vk, vg, vm, vlu); //vl
			}
		}

		// set the elevations of the rightmost and bottom vertices to 
		// equal the leftmost and topmost ones'.
		for (int y = 0; y < terrainSize; y += span)
		{
			matrix[y][terrainSize] = matrix[y][0];
		}
		for (int x = 0; x < terrainSize; x += span)
		{
			matrix[terrainSize][x] = matrix[0][x];
		}
	}
};

int main(void)
{
	Program p;
	vector<Triangle> TriangleList;
	p.iterate();
	int rows = H;
	int cols = W;
	//X : i value of matrix, Y: j value of matrix, Z: value at (i,j) of matrix
	for (int i = 0; i < cols - 1; i++)
	{
		for (int j = 0; j < rows - 1; j++)
		{
			// a: 0,0; b: 0,1; c: 1,1; d: 1,0
			Point* a = new Point(i, j, p.matrix[i][j]);
			Point* b = new Point(i, j + 1, p.matrix[i][j + 1]);
			Point* c = new Point(i + 1, j + 1, p.matrix[i + 1][j + 1]);
			Point* d = new Point(i + 1, j, p.matrix[i + 1][j]);

			//Split rectangle into two triangles
			Triangle* firstTriangle = new Triangle(*a, *b, *c);
			Triangle* secondTriangle = new Triangle(*a, *c, *d);

			TriangleList.push_back(*firstTriangle);
			TriangleList.push_back(*secondTriangle);
		}
	}


	filebuf fb;
	fb.open("important.asc", ios::out);
	ostream os(&fb);

	int skipOffset = 1;


	for (int i =skipOffset; i < (W - skipOffset); i += 2)
	{
	for (int j = skipOffset; j < (H - skipOffset); j += 2)
	{
	double X = 50;
	os << i << " " << j << " " << X * p.matrix[i][j] << "\n";               // writer.WriteLine(i + " " + j + " " + p.matrix[i][j]);
	os << i << " " << j + 1 << " " << X * p.matrix[i][j + 1] << "\n";             // writer.WriteLine(i + " " + (j + 1) + " " + p.matrix[i][j + 1]);
	os << i + 1 << "," << j << " " << X * p.matrix[i + 1][j] << "\n";             // writer.WriteLine((i + 1) + " " + j + " " + p.matrix[i + 1][j]);

	os << i + 1 << " " << j + 1 << " " << X * p.matrix[i + 1][j + 1] << "\n";             // writer.WriteLine((i + 1) + " " + (j + 1) + " " + p.matrix[i + 1, j + 1]);

	}
	}


	filebuf fb2;
	fb2.open("important-email.asc", ios::out);
	ostream os2(&fb2);

	float *normal = new float(3);

	for (int i = 0; i < W - 1; i++)
	{
		for (int j = 0; j < H - 1; j++)
		{
			double X = 50;
			float u = 0, v = 0;

			

			// 1st Trinagle
			os2 << "triangle...\n";
			genNormal(i, j, p.matrix[i][j],
				i, j + 1, p.matrix[i][j + 1],
				i + 1, j, p.matrix[i + 1][j],
				normal);
			// 1st Point
			os2 << i << "\t" << j << "\t" << X * p.matrix[i][j] << "\t" << normal[0] << "\t" << normal[1] << "\t" << normal[2] << "\t" << v << "\t" << v << "\n";
			// 2nd Point
			os2 << i << "\t" << j + 1 << "\t" << X * p.matrix[i][j + 1] << "\t" << normal[0] << "\t" << normal[1] << "\t" << normal[2] << "\t" << v << "\t" << v << "\n";
			// 3rd Point
			os2 << i + 1 << "\t" << j << "\t" << X *  p.matrix[i + 1][j] << "\t" << normal[0] << "\t" << normal[1] << "\t" << normal[2] << "\t" << v << "\t" << v << "\n";

			// 2nd Trinagle
			os2 << "triangle...\n";
			genNormal(i, j + 1, p.matrix[i][j + 1],
				i + 1, j, p.matrix[i + 1][j],
				i + 1, j + 1, p.matrix[i + 1][j + 1],
				normal);
			// 1st Point
			os2 << i << "\t" << j + 1 << "\t" << X *  p.matrix[i][j + 1] << "\t" << normal[0] << "\t" << normal[1] << "\t" << normal[2] << "\t" << v << "\t" << v << "\n";
			// 2nd Point
			os2 << i + 1 << "\t" << j << "\t" << X * p.matrix[i + 1][j] << "\t" << normal[0] << "\t" << normal[1] << "\t" << normal[2] << "\t" << v << "\t" << v << "\n";
			// 2nd Point
			os2 << i + 1 << "\t" << j + 1 << "\t" << X * p.matrix[i + 1][j + 1] << "\t" << normal[0] << "\t" << normal[1] << "\t" << normal[2] << "\t" << v << "\t" << v << "\n";


		}
	}

	fb.close();
	fb2.close();
	return 0;
}
