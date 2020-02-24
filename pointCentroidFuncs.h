#include "standardIncludes.h"
#include "point.h"
using namespace std;
#define M 10
#define threshold 0

void initVector(const char* filename, vector<Point> &points){
  fstream input(filename);
  double x1, x2;
  int category;
  char comma;
  int point_counter = 0;
  while (input >> x1 >> comma >> x2 >> comma >> category)
  {
    Point point(x1,x2,category);
    //cout << "adding point ["<<point_counter<<"] to vector..." << "\n";
    points.push_back(point);
    point_counter++;
  }
}

void selectRandomCentroids(vector<Point> points, Point centroids [M] ){
  int random_point_index,i;
  srand(time(NULL));
  for (i=0; i<M; i++){
    random_point_index = rand() % points.size();
    centroids[i] = points.at(random_point_index);
    //cout << "Selected centroid " << i << " : P[" << random_point_index<<"]\n";
  }
}

int findClosestGroup(double point_centroids_distances [M]){
  int i;
  int min_index = 0;
  double min_distance = point_centroids_distances[0];

  for(i=1; i<M; i++){
    if(min_distance > point_centroids_distances[i]){
      min_distance = point_centroids_distances[i];
      min_index = i;
    }
  }
  return min_index;
}

double calculatePointCentroidDistance(Point point, Point centroid){
  return sqrt(pow((point.x1()-centroid.x1()),2) + pow((point.x2()-centroid.x2()),2));
}

bool notConverge(Point* centroids, Point* prevCentroids){
  for(int i=0;i<M;i++){
    if(abs(centroids[i].x1()-prevCentroids[i].x1())>threshold || abs(centroids[i].x2()-prevCentroids[i].x2())>threshold){

      //cout << abs(centroids[i].x1()-prevCentroids[i].x1()) << "\n";
      return true;
    }
  }
  return false;
}

void writeCentroidsToFile(Point* centroids) {
  int i;
  ofstream outputFile;
  outputFile.open("centroids.txt");
  for(i=0; i<M; i++){
    outputFile << centroids[i].x1() << "," << centroids[i].x2() << "\n" ;
  }
}

void calculateError(vector<Point> points, Point* centroids){
  double sum_error = 0;
  for (int i=0; i<points.size(); i++){
    sum_error += pow(abs(calculatePointCentroidDistance(points[i],centroids[points[i].category()])),2);
  }
  cout << "\n" << "Error : " << sum_error << "\n";
}

