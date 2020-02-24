#include "standardIncludes.h"
#include "point.h"
using namespace std;

vector<Point> points;

double randfrom(double min, double max){
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

void createSquareDataset(const char* filename,int datasize,double bound1,double bound2,double bound3,double bound4,bool append){
  double random_value;
  double x1,x2;
  ofstream myfile;
  
  //choose if you rewrite or append the file
  if(append==true){
    myfile.open(filename,fstream::app);
  }
  else if(append==false){
    myfile.open(filename);
  }

  srand(time(NULL));
  for(int i=0; i<datasize; i++){

    random_value = randfrom(-2,2);
    x1 = randfrom(bound1,bound2);
    x2 = randfrom(bound3,bound4);

    Point point(x1,x2);
    points.push_back(point);
    //finds category as you create the points
    point.findCategory();
    myfile << x1 << "," << x2  << "," << point.category() << "\n";
  }
}

void createNoisyData(){
  ofstream myfile;
  myfile.open("noisy.txt");
  fstream input("train.txt");
  double number,x1,x2;
  int category;
  char comma;

  while (input >> x1 >> comma >> x2 >> comma >> category)
  {
    Point point(x1,x2,category);
    if(point.category()==2 || point.category()==3){
      number = rand() % 100+1;
      if(number<= 10){ //0.1 chance
        point.setCategory(1);
      }
    }
    myfile << x1 << "," << x2  << "," << point.category() << "\n";
  }
}

int main(){
  // S1
  createSquareDataset("train.txt",3000,-2,2,-2,2,false);
  createSquareDataset("test.txt",3000,-2,2,-2,2,false);
  // S1 Noisy
  createNoisyData();
  return 0;
}
