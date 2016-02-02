#include "tgaimage.h"
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <limits>
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

bool isPointDansTriangle(int px, int py, int ax, int ay, int az,int bx, int by, int bz,int cx, int cy,int cz, int buffer[][500]){
/*
On calcule le produit matriciel suivant
   ( (Bx - Ax) (Cx - Ax) (Ax - Px) ) (u)
   ( (By - Ay) (Cy - Ay) (Ay - Py) ) (v)
                                     (1)
     (Bx - Ax)(u) + (Cx - Ax)(v) + (Ax - Px)(1) = 0
     (By - Ay)(u) + (Cy - Ay)(v) + (Ay - Px)(1) = 0
*/
float** A =  new float*[2];
for(int i = 0; i<2 ; ++i){
    A[i] = new float[2];
}

// initialisation de base de A
A[0][0] = bx - ax;
A[0][1] = cx - ax;
A[1][0] = by - ay;
A[1][1] = cy - ay;

float** B = new float*[2];
for(int i = 0; i<2 ; ++i){
    B[i] = new float[1];
}
B[0][0] = px - ax;
B[1][0] = py - ay;


/* On sait que (u) = (A-1 * B)
               (v)
  Et A-1 est l'inverse de A, calculons le
*/

float determinant = ((A[0][0]*A[1][1])-(A[0][1]*A[1][0]));
float coeff = 1./determinant;
//printf("determinant = %f",coeff);
// A-1
A[0][0] = coeff*(cy-ay);
A[0][1] = coeff*(ax-cx);
A[1][0] = coeff*(ay-by);
A[1][1] = coeff*(bx-ax);
/*
float a00 = coeff*(cy-ay);
float a01 = coeff*(ax-cx);
float a10 = coeff*(ay-by);
float a11 = coeff*(bx-ax);

float u = a00*(px - ax) + a01*(py - ay);
float v = a10*(px - ax)+ a11*(py - ay);*/

float u = A[0][0]*B[0][0] + A[0][1]*B[1][0];
float v = A[1][0]*B[0][0] + A[1][1]*B[1][0];

//printf("U = %f\n",u);
//printf("V = %f\n",v);
//printf("1-U-V = %f\n",(1-u-v));
float toto = 1-u-v;

//On prend en compte la profondeur pour ne pas tout dessiner
if(u>=-1e-5 && v>=-1e-5 && toto>=-1e-5){ //on utilise un facteur d'approximation car des fois meme si c'est <0 on doit le prendre
    float pz = (u*az) + (v*bz) + (toto*cz);
    if(buffer[px][py]>pz)return false;
    else buffer[px][py]=pz;
    return true;
}
else return false;

}


void setRemplissageTriangleBarycentric(int x1, int y1, int z1, int x2, int y2, int z2,int x3, int y3,int z3,TGAImage &image, int buffer[][500]){

    int minX = min(x3,min(x1,x2));
    int minY = min(y3,min(y1,y2));
    int maxX = max(x3,max(x1,x2));
    int maxY = max(y3,max(y1,y2));

    TGAColor rndcolor = TGAColor(rand()%255, rand()%255, 255, 255);
    for(int px = minX ; px < maxX ; px++){
       for(int py = minY ; py < maxY ; py++){
           if(isPointDansTriangle(px,py,x1,y1,z1,x2,y2,z2,x3,y3,z3,buffer)){
              image.set(px,py,rndcolor);

            }
        }
    }
}

void setRemplissageTriangleLineSweep(int x1, int y1, int x2, int y2, int x3, int y3,TGAImage &image){


    // On trie les sommets par ordre croissant selon l'axe y
    if(y1>y2){
        swap(y1,y2);
        swap(x1,x2);
    }
    if(y1>y3){
        swap(y1,y3);
        swap(x1,x3);
    }
    if(y2>y3){
        swap(y2,y3);
        swap(x2,x3);
    }

    //On coupe le triangle en deux sous-triangles : partie basse, partie haute
    // Dessin de la partie basse (y entre y1 et y2)
    int hauteur = y3 - y1;
    for(int a = y1; a<y2 ; a++){
        int semiHauteur = y2-y1;
        float alpha = (float)(a-y1)/hauteur;
        float beta = (float) (a-y1)/semiHauteur;
        int ax = x1 + (x3-x1)*alpha;
        int bx = x1 + (x2-x1)*beta;

        if(ax>bx)swap(ax,bx);
        for(int b = ax;b<bx;b++){
            image.set(b,a,white);
        }
    }
    for(int a = y2; a<y3 ; a++){
        int semiHauteur = y3-y2;
        float alpha = (float)(a-y1)/hauteur;
        float beta = (float) (a-y2)/semiHauteur;
        int ax = x1 + (x3-x1)*alpha;
        int bx = x2 + (x3-x2)*beta;

        if(ax>bx)swap(ax,bx);
        for(int b = ax;b<bx;b++){
            image.set(b,a,white);
        }
    }
}

void setTrianglesSurImage(vector<vector<float> > nuage, vector<vector<float> > triangles, TGAImage &image) {
    int a,b,c; // les 3 sommets stockés dans "triangles"
    int x1,y1,x2,y2,x3,y3,z1,z2,z3;
    //on remplit le buffer avec +infini dans chaque case
    int buffer[500][500];
    for(int i=0;i<500;i++){
        for(int j=0;j<500;j++){
            buffer[i][j] = numeric_limits<int>::min();
        }
    }

	for (unsigned int i = 0; i < triangles.size(); ++i) {
        a = triangles[i][0]-1; //on enleve 1 pour atteindre la bonne ligne dans le vector de points
        b = triangles[i][1]-1;
        c = triangles[i][2]-1;

        x1 = nuage[a][0]+image.get_width()/2. + .5 ;
        y1 = nuage[a][1]+image.get_height()/2. + .5 ;
        z1 = nuage[a][2]+image.get_height()/2.+.5;


        x2 = nuage[b][0]+image.get_width()/2. + .5 ;
        y2 = nuage[b][1]+image.get_height()/2. + .5 ;
        z2 = nuage[b][2]+image.get_height()/2.+.5;


        x3 = nuage[c][0]+image.get_width()/2. + .5 ;
        y3 = nuage[c][1]+image.get_height()/2. + .5 ;
        z3 = nuage[c][2]+image.get_height()/2.+.5;


        //line(x1,y1,x2,y2,image,white);
       // line(x2,y2,x3,y3,image,white);
       // line(x1,y1,x3,y3,image,white);

        setRemplissageTriangleBarycentric(x1,y1,z1,x2,y2,z2,x3,y3,z3,image,buffer);

        //setRemplissageTriangleLineSweep(x1,y1,x2,y2,x3,y3, image);
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

    TGAImage imageTest(500, 500, TGAImage::RGB);

    line(60,100,200,300,imageTest,red);
    line(60,100,500,400,imageTest,red);
    line(200,300,500,400,imageTest,red);


    /*for(int px = 0; px < 500 ; px++){
            for(int py = 0;py <500; py++){
                if(isPointDansTriangle(px,py,60,100,0,200,300,0,500,400,0,)){
                    imageTest.set(px,py,white);
                }
            }
        }*/
    //printf("%s",isPointDansTriangle(250,250,60,100,200,300,500,400)?"true":"false");
   // imageTest.set(250,250,white);
    imageTest.flip_vertically();
	imageTest.write_tga_file("Test.tga");




    //while(0==0){}
    return 0;
}




