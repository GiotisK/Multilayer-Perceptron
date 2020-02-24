#include "standardIncludes.h"
#include "pointCentroidFuncs.h"
#include "neuron.h"
#include <iomanip>

using namespace std;

vector<Point> test_points;
vector<Point> noisy_points;

Neuron input_layer[d];
Neuron first_layer[H1];
Neuron second_layer[H2];
Neuron output_layer[K];

Neuron b1, b2, b3;

double total_error = 0;
int L;
int activation_func = -1;

void transformToVector(int category,int *vector){

  if(category == 1){
    vector[0] = 1;  //vector : [1,0,0]
  }else if(category == 2){
    vector[1] = 1;  //vector : [0,1,0]
  }else{
    vector[2] = 1;   //vector : [0,0,1]
  }
}

void forwardPass(int current, int hidden_layer_2_func, string type){
  int i,j;

  //ys for input neurons
  if(type.compare("training")){
    input_layer[0].setY_out(noisy_points[current].x1());
    input_layer[1].setY_out(noisy_points[current].x2());
  }else if(type.compare("testing")){
    input_layer[0].setY_out(test_points[current].x1());
    input_layer[1].setY_out(test_points[current].x2());
  }

  
  //ys for hidden 1 layer
  for (i=0; i<H1; i++){
    double u = 0;
    double y_out = 0;

    for (j=0; j<d; j++){
      u += input_layer[j].y_out() * input_layer[j].weights(i);
    }
    u += b1.y_out() * b1.weights(i); //bias

    y_out = first_layer[i].g(u);
    first_layer[i].setY_out(y_out);
  }

  //ys for hidden 2 layer
  for (i=0; i<H2; i++){
    double u = 0;
    double y_out = 0;

    for (j=0; j<H1; j++){
      u += first_layer[j].y_out() * first_layer[j].weights(i);
    }

    u += b2.y_out() * b2.weights(i); //bias

    y_out = second_layer[i].g(u, hidden_layer_2_func);
    second_layer[i].setY_out(y_out);
  }

  //ys for output layer
  for (i=0; i<K; i++){
    double u = 0;
    double y_out = 0;

    for (j=0; j<H2; j++){
      u += second_layer[j].y_out() * second_layer[j].weights(i);
    }

    u += b3.y_out() * b3.weights(i); //bias

    y_out = output_layer[i].g(u); 
    output_layer[i].setY_out(y_out);
  }
}

void backPropagation(int current,int hidden_layer_2_func){

  double u,delta,sum,error;
  int i,j;
  int true_output [K] = {0,0,0};
  double example_error = 0;
 
  forwardPass(current, hidden_layer_2_func, "training");

  // Start from the end of the neural network

  //output layer
  for(i=0;i<K;i++){

    transformToVector(noisy_points[current].category(), true_output);
    delta = output_layer[i].g_d() * ( output_layer[i].y_out() - true_output[i] );
    output_layer[i].setDelta(delta);
    b3.setErrors(i, delta);

    for (j=0; j<H2; j++){
      error = delta * second_layer[j].y_out();
      second_layer[j].setErrors(i, error);
    }
    example_error += pow( (output_layer[i].y_out() - true_output[i]), 2);


  }
  total_error = total_error + (0.5 *(example_error));

  //second hidden layer
  for(i=0;i<H2;i++){
    sum = 0;

    for (j=0; j<K; j++){
      sum += second_layer[i].weights(j) * output_layer[j].delta();
    }
    delta = second_layer[i].g_d(hidden_layer_2_func) * sum;
    second_layer[i].setDelta(delta);
    b2.setErrors(i, delta);

    for (j=0; j<H1; j++){
      error = delta * first_layer[j].y_out();
      first_layer[j].setErrors(i, error);
    }
  }

  //first hidden layer
  for(i=0;i<H1;i++){
    u = 0;
    sum = 0;

    for (j=0; j<H2; j++){
      sum += first_layer[i].weights(j) * second_layer[j].delta();
    }
    delta = first_layer[i].g_d() * sum;
    first_layer[i].setDelta(delta);
    b1.setErrors(i, delta);

    for (j=0; j<d; j++){
      error = delta * input_layer[j].y_out();
      input_layer[j].setErrors(i, error);
    }
  }
}

void resetErrors() {
  int i;
  for(i=0; i<d; i++){
    input_layer[i].resetErrors();
  }

  for(i=0; i<H1; i++){
    first_layer[i].resetErrors();
  }

  for(i=0; i<H2; i++){
    second_layer[i].resetErrors();

  }
  //biases
  b1.resetErrors();
  b2.resetErrors();
  b3.resetErrors();
}

void initializeWeights(){
  int i;
 
  for(i=0; i<d; i++){
    input_layer[i].randomizeWeights();
  }
  for(i=0; i<H1; i++){
    first_layer[i].randomizeWeights();
  }
  for(i=0; i<H2; i++){
    second_layer[i].randomizeWeights();
  }

  //biases
  b1.randomizeWeights();
  b2.randomizeWeights();
  b3.randomizeWeights();
}

void updateWeights() {
   int i;

  for(i=0; i<d; i++){
    input_layer[i].updateWeights();
  }

  for(i=0; i<H1; i++){
    first_layer[i].updateWeights();
  }

  for(i=0; i<H2; i++){
    second_layer[i].updateWeights();
  }

  b1.updateWeights();
  b2.updateWeights();
  b3.updateWeights();
}

void gradientDescent(int L,int hidden_layer_2_func){
  int i,k;
  int batch_counter = 0;
  double prev_epoch_total_error = 0;
  int max_epochs = 1000;

  initializeWeights();
  resetErrors();

  for (k = 0; k < max_epochs; k++){
    resetErrors();
    

    for(i=0; i<noisy_points.size(); i++){
      batch_counter += 1;
      backPropagation(i,hidden_layer_2_func);
      if(batch_counter == L){
        updateWeights();
        batch_counter = 0;
      }
    }

    //Run at least 500 epochs and stop when the error between 2 epochs is smaller than 0.1 
    if(abs(prev_epoch_total_error-total_error)<0.1 && k>=499){
      cout << setw(10)<< k << setw(10) << total_error;
      total_error = 0;
      return;
    }
    //This print is for the case that the MLP keeps training for the max amount epochs (1000)
    if(k==max_epochs-1){
      cout << setw(10)<< k << setw(10) << total_error;
    }
    
    prev_epoch_total_error = total_error;

    total_error = 0;
  }
  
}

void initNeuronLayers(){
  int i,j;
  b1 = Neuron(-1, 0);
  b2 = Neuron(-1, 1);
  b3 = Neuron(-1, 2);

  b1.setY_out(1);
  b2.setY_out(1);
  b3.setY_out(1);
  //create input layer
  for (i=0; i<d; i++){
    Neuron neuron = Neuron(i, 0);
    input_layer[i] = neuron;
  }

  //create hidden 1 layer
  for (i=0; i<H1; i++){
    Neuron neuron = Neuron(i, 1);
    first_layer[i] = neuron;
  }

  //create hidden 2 layer
  for (i=0; i<H2; i++){
    Neuron neuron = Neuron(i, 2);
    second_layer[i] = neuron;
  }

  //create output layer
  for (i=0; i<K; i++){
    Neuron neuron = Neuron(i, 3);
    output_layer[i] = neuron;
  }
}

void testNetwork(int hidden_layer_2_func){
  double correct_results = 0.0;
  int i,j,t;

  ofstream outputFile;
  outputFile.open("mlp_result_points.txt");

  for(t=0; t<test_points.size(); t++){

    forwardPass(t, hidden_layer_2_func, "testing");

    float output_0 = output_layer[0].y_out();
    float output_1 = output_layer[1].y_out();
    float output_2 = output_layer[2].y_out();
    

    if(output_0>=0.5){
      output_0 = 1;
    }else{
      output_0 = 0;
    }

    if(output_1>=0.5){
      output_1 = 1;
    }else{
      output_1 = 0;
    }

    if(output_2>=0.5){
      output_2 = 1;
    }else{
      output_2 = 0;
    }


    int true_category = test_points[t].category();
    int true_category_vector[K] = {0,0,0};
    transformToVector(true_category , true_category_vector);
   

    if(output_0==true_category_vector[0] && output_1==true_category_vector[1] && output_2==true_category_vector[2]){
      correct_results++;
      outputFile << test_points[t].x1() << "," << test_points[t].x2() << ",+" <<"\n";
    }else{
      outputFile << test_points[t].x1() << "," << test_points[t].x2() << ",-"<<"\n";
    }
  }
  cout << setw(25)<< correct_results/test_points.size() <<"\n";


}


int main(){
  int L_ARRAY[] = { 1,300,30,3000 };
  initVector("test.txt", test_points);
  initVector("noisy.txt",noisy_points);
  initNeuronLayers();
  
  cout << "\n    H1 : " <<H1 <<" , " << "H2 : " <<H2 << "  (number of neurons)"<<"\n\n";
  cout << setw(15) << "H2 function" << setw(10) << "L"<< setw(10) <<"Epochs"<<setw(10)<<"Error"<<setw(25)<<"Correct Decisions (%)"<<"\n";
  cout <<"    ------------------------------------------------------------------\n";

  //testing for optimal network configuration
  /*for(int i=0; i<4 ; i++){
    L = L_ARRAY[i];
    cout << setw(15) <<"TANH"<<setw(10) <<L;
    gradientDescent(L,TANH);
    testNetwork(TANH);
    cout << setw(15) <<"LINEAR"<<setw(10) <<L;
    gradientDescent(L,LINEAR);
    testNetwork(LINEAR);
  }*/

  gradientDescent(300,TANH);
  testNetwork(TANH);
  cout <<"    ------------------------------------------------------------------\n";
  return 0;
}