#include "tgaimage.h"
#include "Matrix.h"

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <limits>
#include <sstream>

#define TAILLEIMAGE 500
#define PI 3.14159265359
#define angle 330
#define C 10000
#define LIGHTX 1
#define LIGHTY 1
#define LIGHTZ 0

#define LIGHTX2 0
#define LIGHTY2 1
#define LIGHTZ2 1

using namespace std;
using std::ifstream;

TGAImage nm;
TGAImage specImg;
const TGAColor white   = TGAColor(250, 250,  250,   255);
float spec = 0.;
float spec2 = 0.;

void normalise(float &a, float &b, float &c){
    float distance = sqrt(a*a + b*b + c*c);
    a /= distance;
    b /= distance;
    c /= distance;
}

vector<vector<float> > parsePoints(ifstream &fin, TGAImage &image){

    vector<vector<float> >nuage;
    vector<float> points;
    string ligne;

    /* lecture de tout le fichier */
    while(getline(fin,ligne,'\n')){

        /* pour chaque ligne qui commence par un 'v  ' */
        if(ligne[0]=='v'&& ligne[1]==' ' && ligne[2]== ' '){

            ligne = ligne.substr(3,ligne.size());
            stringstream ss(ligne);
            ss.str(ligne);
            double a, b, c;
            ss >> a >> b >> c;
            points.push_back(a);
            points.push_back(b);
            points.push_back(c);
            nuage.push_back(points);
            points.clear();
        }
  }
  return nuage;
}



vector<vector<float> > parseTextures(ifstream &fin, TGAImage &image){

vector<vector<float> >nuage;
    vector<float> points;
    string ligne;

    /* lecture de tout le fichier */
    while(getline(fin,ligne,'\n')){

        /* pour chaque ligne qui commence par un 'v  ' */
        if(ligne[0]=='v'&& ligne[1]=='t' && ligne[2]== ' '){
            ligne = ligne.substr(4,ligne.size());
            stringstream ss(ligne);
            ss.str(ligne);
            double a, b, c;
            ss >> a >> b >> c;
            points.push_back(a);
            points.push_back(b);
            points.push_back(c);
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

            points.push_back(x);
            points.push_back(y);
            points.push_back(z);
            nuage.push_back(points);
            points.clear();
        }
     }
     return nuage;
}



vector<vector<float> > parseTrianglesTextures(ifstream &fin, TGAImage &image){
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

            // récupération des trois premiers entiers de chaque triplet (x/entier1/x x/entier2/x x/entier3/x)
            while(ligne[i]!= '/')
                i++;
            i++;
            while(ligne[i]!= '/'){
                entier1 += ligne[i];
                i++;
            }
            while(ligne[i]!=' ')i++;
                i++;
            while(ligne[i]!= '/')
                i++;
            i++;
            while(ligne[i]!= '/'){
                entier2 += ligne[i];
                i++;
            }
            while(ligne[i]!=' ')i++;
                i++;
            while(ligne[i]!= '/')
                i++;
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

            points.push_back(x);
            points.push_back(y);
            points.push_back(z);
            nuage.push_back(points);
            points.clear();
        }
     }
     return nuage;
}



float reflection(float normeX, float normeY, float normeZ, float lightX, float lightY, float lightZ){

        float scalNormeLight = lightX*normeX + lightY*normeY + lightZ*normeZ;
        scalNormeLight *= 2;
        normeX *= scalNormeLight;
        normeY *= scalNormeLight;
        normeZ *= scalNormeLight;
        float rX = normeX - lightX;
        float rY = normeY - lightY;
        float rZ = normeZ - lightZ;
        normalise(rX,rY,rZ);
        return rZ;
}

bool isPointDansTriangle(int px, int py, float ax, float ay, float az,float bx, float by, float bz,float cx, float cy,float cz, int buffer[][TAILLEIMAGE], TGAImage &texture, float xd, float yd, float xe, float ye, float xf, float yf,TGAColor &color, float &normeX, float &normeY, float &normeZ){
/* On calcule le produit matriciel suivant
   ( (Bx - Ax) (Cx - Ax) (Ax - Px) ) (u)
   ( (By - Ay) (Cy - Ay) (Ay - Py) ) (v)
                                     (1)
     (Bx - Ax)(u) + (Cx - Ax)(v) + (Ax - Px)(1) = 0
     (By - Ay)(u) + (Cy - Ay)(v) + (Ay - Px)(1) = 0*/
float txtX, txtY; //les coordonnées des points dans le fichier texture, calculés par la suite
float A00, A01, A10, A11;
float B00, B10;

B00 = px - ax;
B10 = py - ay;

float determinant = (( bx - ax)*(cy - ay)-(cx - ax)*(by - ay));
float coeff = 1./determinant;

// A-1
A00 = coeff*(cy-ay);
A01 = coeff*(ax-cx);
A10 = coeff*(ay-by);
A11 = coeff*(bx-ax);

float u = A00*B00 + A01*B10;
float v = A10*B00 + A11*B10;
float w = 1-u-v;

txtX = (xd*w + xe*u + xf*v)*texture.get_width();
txtY = (yd*w + ye*u + yf*v)*texture.get_height();

color = texture.get(txtX,txtY);
color = white;
//On prend en compte la profondeur pour ne pas tout dessiner
if(u>=-1e-5 && v>=-1e-5 && w>=-1e-5){ //on utilise un facteur d'approximation car des fois meme si c'est <0 on doit le prendre
    float pz = (w*az) + (u*bz) + (v*cz);
    if(buffer[px][py]>pz)return false;
    else {
        float lightX = LIGHTX;
        float lightY = LIGHTY;
        float lightZ = LIGHTZ;

        float lightX2 = LIGHTX2;
        float lightY2 = LIGHTY2;
        float lightZ2 = LIGHTZ2;

        buffer[px][py]=pz;
        TGAColor rgb = nm.get(txtX,txtY);
        normeX = rgb.r;
        normeY = rgb.g;
        normeZ = rgb.b;

        normalise(normeX,normeY,normeZ);
        float rZ = reflection(normeX, normeY, normeZ, lightX, lightY, lightZ);
        float rZ2 = reflection(normeX,normeY,normeZ,lightX2,lightY2,lightZ2);
        TGAColor col = specImg.get(txtX,txtY);
        spec = pow(fmax(0,rZ), col.b);
        spec2 = pow(fmax(0,rZ2), col.b);
    }return true;
}
else return false;

}


void setRemplissageTriangleBarycentric(float x1, float y1, float z1, float x2, float y2, float z2,float x3, float y3,float z3,TGAImage &image, int buffer[][TAILLEIMAGE], TGAImage &texture, float xd, float yd, float xe, float ye, float xf, float yf){

    float minX = min(x3,min(x1,x2));
    minX = fmax(minX,0.); // pour regler le probleme lorsque l'on fait des modifications de perspective, vuq eu le triangle change de taille
    float minY = min(y3,min(y1,y2));
    minY = fmax(minY,0.);
    float maxX = max(x3,max(x1,x2));
    maxX = fmin(maxX,TAILLEIMAGE);
    float maxY = max(y3,max(y1,y2));
    maxY = fmin(maxY,TAILLEIMAGE);

    float normeX, normeY, normeZ = 0;

    //définition du vecteur lumière
    float lightX = LIGHTX;
    float lightY = LIGHTY;
    float lightZ = LIGHTZ;


    //bonus : deuxieme vecteur lumière
    float lightX2 = LIGHTX2;
    float lightY2 = LIGHTY2;
    float lightZ2 = LIGHTZ2;

    normalise(lightX2,lightY2,lightZ2);
    normalise(lightX,lightY,lightZ);

    TGAColor color = TGAColor(rand()%255, rand()%255, 255, 255); //initialisation d'une couleur, qui va être modifiée par la méthode isPointDansTriangle()
    for(int px = minX ; px < maxX ; px++){
       for(int py = minY ; py < maxY ; py++){
           if(isPointDansTriangle(px,py,x1,y1,z1,x2,y2,z2,x3,y3,z3,buffer,texture,xd,yd,xe,ye,xf,yf,color,normeX,normeY,normeZ)){
             float intensity = fmax(0,normeX*lightX + normeY*lightY + normeZ*lightZ);
             float intensity2 = fmax(0,normeX*lightX2 + normeY*lightY2 + normeZ*lightZ2);
             color = TGAColor(fmin(5+color.r*(intensity2 + .6*spec2),255),fmin(5+color.g*(intensity + .6*spec),255),fmin(5+color.b*(intensity + .6*spec),255),0);
             image.set(px,py,color);
            }
        }
    }
}


void setTrianglesSurImage(vector<vector<float> > nuage, vector<vector<float> > triangles, TGAImage &image, vector<vector<float> > trianglesTextures, vector<vector<float> > textures, TGAImage texture) {
    int a,b,c; // les 3 sommets stockés dans "triangles"
    int d,e,f; // les 3 sommets des textures
    float xd, yd, xe, ye, xf, yf;
    float x1,y1,x2,y2,x3,y3,z1,z2,z3;
    //on remplit le buffer avec +infini dans chaque case
    int buffer[TAILLEIMAGE][TAILLEIMAGE];
    for(int i=0;i<TAILLEIMAGE;i++){
        for(int j=0;j<TAILLEIMAGE;j++){
            buffer[i][j] = numeric_limits<int>::min();
        }
    }
    Matrix m1(4,1);
    Matrix m2(4,1);
    Matrix m3(4,1);
    /* viewport */
    Matrix pivot(4,4);
    pivot[0][0] = pivot[0][3] = pivot[1][1] =  pivot[1][3] =   pivot[2][2] =  pivot[2][3] = TAILLEIMAGE/2;
    pivot[3][3] = 1;

    /* matrice de rotation */
    Matrix transformation(4,4);
    float alpha = angle*(PI/180);

    transformation[0][0] = cos(alpha);
    transformation[0][2] = sin(alpha);
    transformation[1][1] = 1;
    transformation[2][0] = -sin(alpha);
    transformation[2][2] = cos(alpha);
    transformation[3][3] = 1;

    /* matrice de projection */
    Matrix perspective(4,4);
    perspective = perspective.identity(4);
    perspective[3][2] = -1./C;

    pivot = pivot*perspective*transformation;

	for (unsigned int i = 0; i < triangles.size(); ++i) {
        a = triangles[i][0]-1; //on enleve 1 pour atteindre la bonne ligne dans le vector de points
        b = triangles[i][1]-1;
        c = triangles[i][2]-1;
        //données pour le calcul des textures
        d = trianglesTextures[i][0]-1;
        e = trianglesTextures[i][1]-1;
        f = trianglesTextures[i][2]-1;

        xd = textures[d][0];
        yd = textures[d][1];
        xe = textures[e][0];
        ye = textures[e][1];
        xf = textures[f][0];
        yf = textures[f][1];

        m1[0][0] = nuage[a][0];
        m1[1][0] = nuage[a][1];
        m1[2][0] = nuage[a][2];

        m2[0][0] = nuage[b][0];
        m2[1][0] = nuage[b][1];
        m2[2][0] = nuage[b][2];

        m3[0][0] = nuage[c][0];
        m3[1][0] = nuage[c][1];
        m3[2][0] = nuage[c][2];
        m1[3][0] = m2[3][0] = m3[3][0] = 1;

        m1 = pivot*m1;
        m2 = pivot*m2;
        m3 = pivot*m3;

       /* homogeneisation (useless car la ligne vaut 1) */
        x1 = m1[0][0]/m1[3][0];
        x2 = m2[0][0]/m2[3][0];
        x3 = m3[0][0]/m3[3][0];

        y1 = m1[1][0]/m1[3][0];
        y2 = m2[1][0]/m2[3][0];
        y3 = m3[1][0]/m3[3][0];

        z1 = m1[2][0]/m1[3][0];
        z2 = m2[2][0]/m2[3][0];
        z3 = m3[2][0]/m3[3][0];

        setRemplissageTriangleBarycentric(x1,y1,z1,x2,y2,z2,x3,y3,z3,image,buffer,texture,xd, yd, xe, ye, xf, yf);
	}
	image.flip_vertically();
	image.write_tga_file("Triangles.tga");
}


int main(int argc, char** argv) {
	TGAImage imageNuage(TAILLEIMAGE, TAILLEIMAGE, TGAImage::RGB);
	TGAImage imageTriangles(TAILLEIMAGE, TAILLEIMAGE, TGAImage::RGB);
	TGAImage texture(TAILLEIMAGE,TAILLEIMAGE,TGAImage::RGB);
    texture.read_tga_file("african_head_diffuse.tga");
    nm.read_tga_file("african_head_nm.tga");
    specImg.read_tga_file("african_head_spec.tga");
    specImg.flip_vertically();
    nm.flip_vertically();
    texture.flip_vertically();

	ifstream fin;
	fin.open("african_head.obj");
	printf("Recuperation Points\n");
	vector<vector<float> > nuage = parsePoints(fin, imageNuage);

    ifstream fin2;
    fin2.open("african_head.obj");
    printf("Recuperation Triangles (f A/x/x B/x/x C/x/x )\n");
	vector<vector<float> > triangles = parseTriangles(fin2, imageTriangles);

    ifstream fin3;
    fin3.open("african_head.obj");
    printf("Recuperation Triangles textures (f x/D/x x/E/x x/F/x)\n");
    vector<vector<float> > trianglesTextures = parseTrianglesTextures(fin3, imageTriangles);

    ifstream fin4;
    fin4.open("african_head.obj");
    printf("Recuperation Textures\n");
    vector<vector<float> > textures = parseTextures(fin4, imageTriangles);

    printf("Dessin de l'image finale dans le fichier Triangles.tga");
    setTrianglesSurImage(nuage, triangles, imageTriangles, trianglesTextures, textures, texture);

   // while(0==0){}
    return 0;
}




