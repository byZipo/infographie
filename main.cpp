#include "tgaimage.h"
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;
using std::ifstream;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue   = TGAColor(0, 255,   0,   255);
const TGAColor green   = TGAColor(0, 0,   255,   255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}

vector<vector<float> > parsePoints(ifstream &fin, TGAImage &image){

    vector<vector<float> >nuage;
    vector<float> points;

    std::string ligne;

    string entier1,entier2,entier3;
    float x,y,z;

    /* lecture de tout le fichier */
    while(getline(fin,ligne,'\n')){
        entier1="";
        entier2="";
        entier3="";

    /* pour chaque ligne qui commence par un 'v  ' */
    if(ligne[0]=='v'&& ligne[1]==' ' && ligne[2]== ' '){

    int i = 3;

    /* récuperation des 3 string x,y,z du fichier */
    while(ligne[i]!= ' '){
        entier1 += ligne[i];
        i++;
    }
    i++;

    while(ligne[i]!= ' '){
        entier2 += ligne[i];
        i++;
    }
    i++;

    while(ligne[i]!= ' '){
        entier3 += ligne[i];
        if(!((int)ligne.size()-1==i)){
            i++;
        }else break;
    }
      /* conversion de ces strings en float */
    x = atof(entier1.c_str());
    y = atof(entier2.c_str());
    z = atof(entier3.c_str());

    points.push_back(x);
    points.push_back(y);
    points.push_back(z);
    nuage.push_back(points);
    points.clear();

    }
  }
  return nuage;
}



vector<vector<float> > parseTriangles(ifstream &fin, TGAImage &image){

    vector<vector<float> >nuage;
    vector<float> points;

    std::string ligne;

    string entier1,entier2,entier3;
    float x,y,z;

     while(getline(fin,ligne,'\n')){
        entier1="";
        entier2="";
        entier3="";
        // pour chaque ligne qui commence par f
        if (ligne[0] == 'f' && ligne[1] == ' '){
            int i = 2;

            // récupération des trois premiers entiers de chaque triplet (entier1/x/x entier2/x/x entier3/x/x)
            while(ligne[i]!= '/'){
                entier1 += ligne[i];
                i++;
            }
            while(ligne[i]!=' ')i++;
            i++;
            while(ligne[i]!= '/'){
                entier2 += ligne[i];
                i++;
            }
            while(ligne[i]!=' ')i++;
            i++;
            while(ligne[i]!= '/'){
                entier3 += ligne[i];
                if(!((int)ligne.size()-1==i)){
                    i++;
                }else break;
            }
            // conversion de ces entiers en float
            x = atof(entier1.c_str());
            y = atof(entier2.c_str());
            z = atof(entier3.c_str());


            //printf("--> %f ",x);
           // printf("%f ",y);
           // printf("%f ",z);
           // printf("\n");

            points.push_back(x);
            points.push_back(y);
            points.push_back(z);
            nuage.push_back(points);
            points.clear();
        }
     }
     return nuage;
}



void setNuageSurImage(vector<vector<float> > nuage, TGAImage &image) {
    /* on ne se préoccupe pas de z, on dessine en 2D */
    float x,y;
	for (unsigned int i = 0; i < nuage.size(); ++i) {
        /* on doit decaller les points pour passer en positif pour chaque valeur */
		x = nuage[i][0]+image.get_width()/2;
		y = nuage[i][1]+image.get_height()/2;
		image.set(x, y, red);
	}
	image.flip_vertically();
	image.write_tga_file("Nuage.tga");
}

void setTrianglesSurImage(vector<vector<float> > nuage, vector<vector<float> > triangles, TGAImage &image) {
    int a,b,c; // les 3 sommets stockés dans "triangles"
    int x1,y1,x2,y2,x3,y3;

	for (unsigned int i = 0; i < triangles.size(); ++i) {
        a = triangles[i][0]-1; //on enleve 1 pour atteindre la bonne ligne dans le vector de points
        b = triangles[i][1]-1;
        c = triangles[i][2]-1;

        x1 = nuage[a][0]+image.get_width()/2;
        y1 = nuage[a][1]+image.get_height()/2;

        x2 = nuage[b][0]+image.get_width()/2;
        y2 = nuage[b][1]+image.get_height()/2;

        x3 = nuage[c][0]+image.get_width()/2;
        y3 = nuage[c][1]+image.get_height()/2;

        line(x1,y1,x2,y2,image,red);
        line(x2,y2,x3,y3,image,blue);
        line(x1,y1,x3,y3,image,green);

	}
	image.flip_vertically();
	image.write_tga_file("Triangles.tga");
}

int main(int argc, char** argv) {
	TGAImage imageNuage(500, 500, TGAImage::RGB);
	TGAImage imageTriangles(500, 500, TGAImage::RGB);

	ifstream fin;
	fin.open("african_head.obj");
	vector<vector<float> > nuage = parsePoints(fin, imageNuage);
	printf("Nuage de points");
	setNuageSurImage(nuage, imageNuage);

	ifstream fin2;
    fin2.open("african_head.obj");
	vector<vector<float> > triangles = parseTriangles(fin2, imageTriangles);
	printf("Triangles");
    setTrianglesSurImage(nuage, triangles, imageTriangles);

	return 0;
}




