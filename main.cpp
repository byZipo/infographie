#include "tgaimage.h"
#include "Matrix.h"

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <limits>

#define TAILLEIMAGE 500
#define PI 3.14159265359
#define angle 330
#define C 10000

using namespace std;
using std::ifstream;

TGAImage nm;
TGAImage specImg;
float spec = 0.;

const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor white   = TGAColor(250, 250,  250,   255);

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



vector<vector<float> > parseTextures(ifstream &fin, TGAImage &image){

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

    /* pour chaque ligne qui commence par un 'vt ' */
    if(ligne[0]=='v'&& ligne[1]=='t' && ligne[2]== ' '){

    int i = 4;

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

    /*printf(" X= %f ",x);
    printf("| Y= %f",y);
    printf("| Z= %f",z);
    printf("\n");*/

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


vector<vector<float> >parseLissage(ifstream &fin){
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

            // récupération des trois derniers entiers de chaque triplet (x/x/entier1 x/x/entier2 x/x/entier3)
            while(ligne[i]!= '/')i++;
            i++;
            while(ligne[i]!='/')i++;
            i++;
            while(ligne[i]!= ' '){
                entier1 += ligne[i];
                i++;
            }
            i++;
            while(ligne[i]!= '/')i++;
            i++;
            while(ligne[i]!='/')i++;
            i++;
            while(ligne[i]!= ' '){
                entier2 += ligne[i];
                i++;
            }
            while(ligne[i]!= '/')i++;
            i++;
            while(ligne[i]!='/')i++;
            i++;
            while(ligne[i]!= ' '){
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


vector<vector<float> > parseVecteursNormaux(ifstream &fin){
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

    /* pour chaque ligne qui commence par un 'vn  ' */
    if(ligne[0]=='v'&& ligne[1]=='n' && ligne[2]== ' '){

    int i = 4;

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

bool isPointDansTriangle(int px, int py, float ax, float ay, float az,float bx, float by, float bz,float cx, float cy,float cz, int buffer[][500], TGAImage &texture, float xd, float yd, float xe, float ye, float xf, float yf,TGAColor &color, float &normeX, float &normeY, float &normeZ, float xg, float yg, float zg, float xh, float yh, float zh, float xi, float yi, float zi,float &lightX, float &lightY, float &lightZ){
/* On calcule le produit matriciel suivant
   ( (Bx - Ax) (Cx - Ax) (Ax - Px) ) (u)
   ( (By - Ay) (Cy - Ay) (Ay - Py) ) (v)
                                     (1)
     (Bx - Ax)(u) + (Cx - Ax)(v) + (Ax - Px)(1) = 0
     (By - Ay)(u) + (Cy - Ay)(v) + (Ay - Px)(1) = 0*/
float txtX, txtY; //les coordonnées des points dans le fichier texture, calculés par la suite
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
  Et A-1 est l'inverse de A, calculons le: */

float determinant = ((A[0][0]*A[1][1])-(A[0][1]*A[1][0]));
float coeff = 1./determinant;

// A-1
A[0][0] = coeff*(cy-ay);
A[0][1] = coeff*(ax-cx);
A[1][0] = coeff*(ay-by);
A[1][1] = coeff*(bx-ax);
/*
float u = a00*(px - ax) + a01*(py - ay);
float v = a10*(px - ax)+ a11*(py - ay);*/

float u = A[0][0]*B[0][0] + A[0][1]*B[1][0];
float v = A[1][0]*B[0][0] + A[1][1]*B[1][0];
float w = 1-u-v;

txtX = (xd*w + xe*u + xf*v)*texture.get_width();
txtY = (yd*w + ye*u + yf*v)*texture.get_height();

color = texture.get(txtX,txtY);
//color =  TGAColor(rand()%255, rand()%255, 255, 255);
//color = white;
//On prend en compte la profondeur pour ne pas tout dessiner
if(u>=-1e-5 && v>=-1e-5 && w>=-1e-5){ //on utilise un facteur d'approximation car des fois meme si c'est <0 on doit le prendre
    float pz = (w*az) + (u*bz) + (v*cz);
    if(buffer[px][py]>pz)return false;
    else {
        buffer[px][py]=pz;
        TGAColor rgb = nm.get(txtX,txtY);
        normeX = rgb.r;
        normeY = rgb.g;
        normeZ = rgb.b;

        float distance =  sqrt(normeX*normeX + normeY*normeY + normeZ*normeZ);
        normeX /= distance;
        normeY /= distance;
        normeZ /= distance;

        float cpyNormeX = normeX;
        float cpyNormeY = normeY;
        float cpyNormeZ = normeZ;

        float scalNormeLight = lightX*cpyNormeX + lightY*cpyNormeY + lightZ*cpyNormeZ;
        scalNormeLight *= 2;
        cpyNormeX *= scalNormeLight;
        cpyNormeY *= scalNormeLight;
        cpyNormeZ *= scalNormeLight;

        float rX = cpyNormeX - lightX;
        float rY = cpyNormeY - lightY;
        float rZ = cpyNormeZ - lightZ;

        float norme = sqrt(rX*rX + rY*rY + rZ*rZ);
        rX /= norme;
        rY /= norme;
        rZ /= norme;

        TGAColor col = specImg.get(txtX,txtY);
        spec = pow(fmax(0,rZ), col.b);


       // normeX = xg*w + xh*u + xi*v;
       // normeY = yg*w + yh*u + yi*v;
       // normeZ = zg*w + zh*u + zi*v;
    }return true;
}
else return false;

}


void setRemplissageTriangleBarycentric(float x1, float y1, float z1, float x2, float y2, float z2,float x3, float y3,float z3,TGAImage &image, int buffer[][500], TGAImage &texture, float xd, float yd, float xe, float ye, float xf, float yf, float xg, float yg, float zg, float xh, float yh, float zh, float xi, float yi, float zi){

    float minX = min(x3,min(x1,x2));
    minX = fmax(minX,0.); // pour regler le probleme lorsque l'on fait des modifications de perspective, vuq eu le triangle change de taille
    float minY = min(y3,min(y1,y2));
    minY = fmax(minY,0.);
    float maxX = max(x3,max(x1,x2));
    maxX = fmin(maxX,TAILLEIMAGE);
    float maxY = max(y3,max(y1,y2));
    maxY = fmin(maxY,TAILLEIMAGE);

    // normalisation
    float normeX = 0.;//y12*z13 - z12*y13;
    float normeY = 0.;//z12*x13 - x12*z13;
    float normeZ = 0.;//x12*y13 - y12*x13;

    //définition du vecteur lumière
    float lightX = 01;
    float lightY = 1;
    float lightZ = 0;

    float normLum = sqrt(lightX*lightX + lightY*lightY + lightZ*lightZ);
    lightX /= normLum;
    lightY /= normLum;
    lightZ /= normLum;

    TGAColor color = TGAColor(rand()%255, rand()%255, 255, 255); //initialisation d'une couleur, qui va être modifiée par la méthode isPointDansTriangle()
    for(int px = minX ; px < maxX ; px++){
       for(int py = minY ; py < maxY ; py++){
           if(isPointDansTriangle(px,py,x1,y1,z1,x2,y2,z2,x3,y3,z3,buffer,texture,xd,yd,xe,ye,xf,yf,color,normeX,normeY,normeZ,xg,yg,zg,xh,yh,zh,xi,yi,zi,lightX,lightY,lightZ)){
             float intensity = fmax(0,normeX*lightX + normeY*lightY + normeZ*lightZ);
             //printf("%f\n", intensity);1111
             //color = TGAColor(color.r*intensity,color.g*intensity,color.b*intensity,color.a*intensity);
             color = TGAColor(fmin(5+color.r*(intensity + .6*spec),255),fmin(5+color.g*(intensity + .6*spec),255),fmin(5+color.b*(intensity + .6*spec),255),0);
             image.set(px,py,color);
            }
        }
    }
}


void setTrianglesSurImage(vector<vector<float> > nuage, vector<vector<float> > triangles, TGAImage &image, vector<vector<float> > trianglesTextures, vector<vector<float> > textures, TGAImage texture,vector<vector<float> > &lissage,vector<vector<float> > &vectN) {
    int a,b,c; // les 3 sommets stockés dans "triangles"
    int d,e,f; // les 3 sommets des textures
    int g,h,i1; // vecteurs normaux pour le lissage
    float xd, yd, xe, ye, xf, yf;
    float xg, yg, zg, xh, yh, zh, xi, yi, zi;
    float x1,y1,x2,y2,x3,y3,z1,z2,z3;
    //on remplit le buffer avec +infini dans chaque case
    int buffer[500][500];
    for(int i=0;i<500;i++){
        for(int j=0;j<500;j++){
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

    /* matrice de tranformation */
    Matrix transformation(4,4);
    float alpha = angle*(PI/180);

    transformation[0][0] = cos(alpha);
    transformation[0][2] = sin(alpha);
    transformation[1][1] = 1;
    transformation[2][0] = -sin(alpha);
    transformation[2][2] = cos(alpha);
    transformation[3][3] = 1;

    Matrix perspective(4,4);
    perspective = perspective.identity(4);
    perspective[3][2] = -1./C;

    pivot = pivot*perspective*transformation;

	for (unsigned int i = 0; i < triangles.size(); ++i) {
        //printf("\n%i",i);
        a = triangles[i][0]-1; //on enleve 1 pour atteindre la bonne ligne dans le vector de points
        b = triangles[i][1]-1;
        c = triangles[i][2]-1;
        //données pour le calcul des textures
        d = trianglesTextures[i][0]-1;
        e = trianglesTextures[i][1]-1;
        f = trianglesTextures[i][2]-1;

        g = lissage[i][0]-1;
        h = lissage[i][1]-1;
        i1 = lissage[i][2]-1;

       // cout << g << h << i1 << endl;

        xd = textures[d][0];
        yd = textures[d][1];
        xe = textures[e][0];
        ye = textures[e][1];
        xf = textures[f][0];
        yf = textures[f][1];

        xg = vectN[g][0];
        yg = vectN[g][1];
        zg = vectN[g][2];
        xh = vectN[h][0];
        yh = vectN[h][1];
        zh = vectN[h][2];
        xi = vectN[i1][0];
        yi = vectN[i1][1];
        zi = vectN[i1][2];

        //cout << xg << yg << zg << xh << yh << zh << xi << yi << zi << endl;
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

        x1 = m1[0][0]/m1[3][0];
        x2 = m2[0][0]/m2[3][0];
        x3 = m3[0][0]/m3[3][0];

        y1 = m1[1][0]/m1[3][0];
        y2 = m2[1][0]/m2[3][0];
        y3 = m3[1][0]/m3[3][0];

        z1 = m1[2][0]/m1[3][0];
        z2 = m2[2][0]/m2[3][0];
        z3 = m3[2][0]/m3[3][0];

        setRemplissageTriangleBarycentric(x1,y1,z1,x2,y2,z2,x3,y3,z3,image,buffer,texture,xd, yd, xe, ye, xf, yf, xg, yg, zg, xh, yh, zh, xi, yi, zi);
	}
	image.flip_vertically();
	image.write_tga_file("Triangles.tga");
}


int main(int argc, char** argv) {
	TGAImage imageNuage(500, 500, TGAImage::RGB);
	TGAImage imageTriangles(500, 500, TGAImage::RGB);
	TGAImage texture(500,500,TGAImage::RGB);
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

	printf("Dessin du nuage de points dans le fichier Nuage.tga");
	setNuageSurImage(nuage, imageNuage);

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

    ifstream fin5;
    fin5.open("african_head.obj");
    printf("Recuperation Lissage (f x/x/G x/x/H x/x/I");
    vector<vector<float> > lissage = parseLissage(fin5);

    ifstream fin6;
    fin6.open("african_head.obj");
    printf("Recuperation vecteurs normaux");
    vector<vector<float> > vectN = parseVecteursNormaux(fin6);

    /*for(unsigned int i = 0 ; i < 1; i++){
        printf("%f %f %f",lissage[i][0], lissage[i][1], lissage[i][2]);
        printf("%        > %f %f %f\n", vectN[24][0], vectN[i][1],vectN[i][2]);
    }
    */
    //printf("%i", lissage.size());

    printf("Dessin de l'image finale dans le fichier Triangles.tga");
    setTrianglesSurImage(nuage, triangles, imageTriangles, trianglesTextures, textures, texture, lissage, vectN);

   // while(0==0){}
    return 0;
}




