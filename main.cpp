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

void parse(ifstream &fin, TGAImage &image, TGAColor color){
  
 
  std::string ligne;
  // vector<vector<int> > v;
  int cmptLigne = 0;
  
  /* lecture de tout le fichier */
  while(!fin.eof()){
    
    getline(fin,ligne);
    /* pour chaque ligne qui commence par un 'v  ' */
    if(ligne.at(0)=='v'&& ligne.at(1)==' ' && ligne.at(2)== ' '){
      /*vector<int> colonne;
	v.push_back(colonne);*/
      string entier1;
      string entier2;
      string entier3;
      int i = 3;

      /* récuperation des 3 string x,y,z du fichier */
      while(ligne.at(i)!= ' '){
	entier1 += ligne.at(i);
	i++;
      }
      i++;
      
      while(ligne.at(i)!= ' '){
	entier2 += ligne.at(i);
	i++;
      }
      i++;
      
      while(ligne.at(i)!= ' '){
	entier3 += ligne.at(i);
	if(!((int)ligne.size()-1==i)){
	  i++;
	}else {
	  break;
	}
      } 

      printf("%s ",entier1.c_str());
      printf("%s ",entier2.c_str());
      printf("%s ",entier3.c_str());
      
      /* conversion de ces strings en float */
      float x = atof(entier1.c_str());
      float y = atof(entier2.c_str());
      float z = atof(entier3.c_str());
     
      printf("--> %f ",x);
      printf("%f ",y);
      printf("%f ",z);
     
      printf("\n");
      /*      v[cmptLigne].push_back(ligne.at(3));
      v[cmptLigne].push_back(ligne.at(4));
      v[cmptLigne].push_back(ligne.at(5));
      //      printf("%s\n",ligne.c_str());
      printf("%i\n",v[cmptLigne][0]);*/
    }
    cmptLigne++;
  }

}

int main(int argc, char** argv) {
	TGAImage image(100, 100, TGAImage::RGB);
	//image.set(52, 41, red);
	line(10,20,30,50,image,red);
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	ifstream fin;
	fin.open("african_head.obj");
	parse(fin, image, red);
	return 0;
}




