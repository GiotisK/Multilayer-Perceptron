//implementation of Point Class

class Point {
  private:
    int categoryVal;
    double x1val, x2val;
  public:
    Point(double x1 = 0.0, double x2 = 0.0, int category = 0) {
      x1val = x1;
      x2val = x2;
      categoryVal = category;
    }

    double x1() { return x1val; }
    double x2() { return x2val; }

    void setx1(double x1) { x1val = x1; }
    void setx2(double x2) { x2val = x2; }

    void setCategory(int c) { categoryVal = c; }
    int category() { return categoryVal; }
    // optimized cpp findCategory in point bugfix
    void findCategory(){

      if ( ( pow(x1val-1,2) + pow((x2val-1),2) ) <= 0.49 ) {
        setCategory(2);
      }else if ( ( pow((x1val+1),2) + pow((x2val+1),2) ) <= 0.49 ) {
        setCategory(2);
      }else if ( ( pow((x1val+1),2) + pow((x2val-1),2) ) <= 0.49 ) {
        setCategory(3);
      }else if ( ( pow((x1val-1),2) + pow((x2val+1),2) ) <= 0.49 ) {
        setCategory(3);
      }
      else { setCategory(1); }
    }
};
