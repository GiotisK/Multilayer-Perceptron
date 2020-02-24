//implementation of Point Class
#define d 2 // total input neurons
#define K 3
#define H1 8
#define H2 5
#define TANH 0
#define LINEAR 1
#define n 0.001

class Neuron {
  private:
    int id;
    int layer;
    double u_in; // neuron input
    double y_outVal;// neuron output
    double* weightsArr; // edges from this neuron to all others
    double deltaVal;
    double* errorsArr;
    int weightsArrSize;

  public:
    Neuron(int id=0,   int layer=0) {
      this->id = id;
      this->layer = layer;
      allocateWeigthsArraySize();
    }

    void setY_out(double y_out) { y_outVal = y_out; }
    double y_out() { return y_outVal; }
    double getUin() { return u_in; }
    double weights(int i) { return weightsArr[i]; }

    void allocateWeigthsArraySize() {
        switch(layer){
            case 0:
                weightsArr = (double*)malloc(H1 * sizeof(double));
                errorsArr = (double*)malloc(H1 * sizeof(double));
                weightsArrSize = H1;
                break;

            case 1:
                weightsArr = (double*)malloc(H2 * sizeof(double));
                errorsArr = (double*)malloc(H2 * sizeof(double));
                weightsArrSize = H2;
                break;

            case 2:
                weightsArr = (double*)malloc(K * sizeof(double));
                errorsArr = (double*)malloc(K * sizeof(double));
                weightsArrSize = K;
                break;
        }
    }

    double randfrom(double min, double max){
        double range = (max - min);
        double div = RAND_MAX / range;
        return min + (rand() / div);
    }

    void updateWeights() {
        int i;
        int size = weightsArrSize;

        for(i=0; i<size; i++){
            weightsArr[i] = weightsArr[i] - ( n* errorsArr[i] );
        }
    }

    void randomizeWeights(){//fills with random [-1,1]
        int i;
        int size = weightsArrSize;

        for(i=0; i<size; i++){
            weightsArr[i] = randfrom(-1, 1);
        }
    }

    void printErrors(){
        int i;
        int size = weightsArrSize;

        for(i=0; i<size; i++){
            // cout <<"Error: " << i <<"->" << errorsArr[i] << "\n";
        }
    }
    //sinartisi energopoihshs
    double g(double x, int function_type = -1){

        switch(layer) {
            case 1://layer 1 and layer 3 -> sigmoig

            case 3:
                //cout << "\t\t (Calculating g(u) using SIGMOID)\n";
                return ( 1/ ( 1 + exp(-x) ) );

            case 2://layer 2 -> TANH OR LINEAR
                if(function_type == TANH){
                    // cout << "\t\t (Calculating g(u) using TANH) , input: "<< x <<" , result : " << ( exp(x)-exp(-x) ) / ( exp(x)+exp(-x) ) <<"\n";
                    return ( ( exp(x)-exp(-x) ) / ( exp(x)+exp(-x) ) );
                }else if(function_type == LINEAR){
                    //cout << "\t\t (Calculating g(u) using LINEAR)\n";
                    return x;
                }
        }
    }

    double g_d(int function_type = -1){  //derivative of g function (g')
        switch(layer) {
            case 1://layer 1 and layer 3 -> sigmoig
            case 3:
                return y_outVal*(1-y_outVal);

            case 2://layer 2 -> TANH OR LINEAR
                if(function_type == TANH){
                    return ( 1 - pow(y_outVal,2) );
                }else if(function_type == LINEAR){
                    return 1;
                }
        }
    }

    double delta(){
        return deltaVal;
    }
    void setDelta(double delta){
        deltaVal=delta;
    }

    void resetErrors() {
        int i;
        int size = weightsArrSize;
        for(i=0; i<size; i++){
            errorsArr[i] = 0;
        }
    }

    double errors(int i) { return errorsArr[i]; }
    void setErrors(int i, double error) { errorsArr[i] += error; }

};
