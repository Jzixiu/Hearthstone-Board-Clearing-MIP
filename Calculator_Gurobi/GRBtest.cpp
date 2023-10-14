#include "gurobi_c++.h"
using namespace std;

#define var_M (10000)

int main()
{
  try {
    // Create an environment
    GRBEnv env = GRBEnv();
    env.set(GRB_IntParam_OutputFlag,0);
    env.start();

    // Create an empty model
    GRBModel model = GRBModel(env);

    // Create variables
    
    int m=3;
    int n=2;
    int A[3+1]={0,2,2,2};
    int B[3+1]={0,1,1,1};
    int P[2+1]={0,2,4};
    int Q[2+1]={0,3,4};
    

    GRBVar x11 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x11");
    GRBVar x12 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x12");
    GRBVar x21 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x21");
    GRBVar x22 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x22");
    GRBVar x31 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x31");
    GRBVar x32 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x32");
    GRBVar y1 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y1");
    GRBVar y2 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y2");
    GRBVar y3 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y3");
    GRBVar z1 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "z1");
    GRBVar z2 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "z2");
    GRBVar s1 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, "s1");
    GRBVar s2 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, "s2");
    GRBVar s3 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, "s3");
    
    GRBLinExpr C1=x11*A[1]+x21*A[2]+x31*A[3];
    GRBLinExpr C2=x12*A[1]+x22*A[2]+x32*A[3];

    GRBLinExpr D1=x11*P[1]+x12*P[2];
    GRBLinExpr D2=x21*P[1]+x22*P[2];
    GRBLinExpr D3=x31*P[1]+x32*P[2];

    // Set objective: maximize x + y + 2 z
    model.setObjective(s1 + s2 + s3, GRB_MAXIMIZE);

    model.addConstr(x11+x12 <= 1, "row1");
    model.addConstr(x21+x22 <= 1, "row2");
    model.addConstr(x31+x32 <= 1, "row3");

    model.addConstr(D1+var_M*y1 >= B[1], "row4_1");
    model.addConstr(D1+var_M*y1 <= B[1] + var_M-1, "row4_2");
    model.addConstr(D2+var_M*y2 >= B[2], "row5_1");
    model.addConstr(D2+var_M*y2 <= B[2] + var_M-1, "row5_2");
    model.addConstr(D3+var_M*y3 >= B[3], "row6_1");
    model.addConstr(D3+var_M*y3 <= B[3] + var_M-1, "row6_2");

    model.addConstr(C1+var_M*z1 >= Q[1], "row7_1");
    model.addConstr(C1+var_M*z1 <= Q[1] + var_M-1, "row7_2");
    model.addConstr(C2+var_M*z2 >= Q[2], "row8_1");
    model.addConstr(C2+var_M*z2 <= Q[2] + var_M-1, "row8_2");
    
    model.addConstr(s1 >= 0, "row9");
    model.addConstr(s2 >= 0, "row10");
    model.addConstr(s3 >= 0, "row11");

    model.addConstr(s1-y1*var_M <= 0, "row12");
    model.addConstr(s2-y2*var_M <= 0, "row13");
    model.addConstr(s3-y3*var_M <= 0, "row14");

    model.addConstr(s1+A[1]*D1-y1*var_M >= A[1]*B[1]-var_M,"row15");
    model.addConstr(s2+A[2]*D2-y2*var_M >= A[2]*B[2]-var_M,"row16");
    model.addConstr(s3+A[3]*D3-y3*var_M >= A[3]*B[3]-var_M,"row17");

    model.addConstr(s1+A[1]*D1+y1*var_M <= A[1]*B[1]+var_M,"row18");
    model.addConstr(s2+A[2]*D2+y2*var_M <= A[2]*B[2]+var_M,"row19");
    model.addConstr(s3+A[3]*D3+y3*var_M <= A[3]*B[3]+var_M,"row20");

    model.addConstr(P[1]*z1+P[2]*z2 <= 2,"row21");
    // Optimize model
    model.optimize();

    cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
    cout<<x11.get(GRB_DoubleAttr_X);
    cout<<x12.get(GRB_DoubleAttr_X);
    cout<<x21.get(GRB_DoubleAttr_X);
    cout<<x22.get(GRB_DoubleAttr_X);
    cout<<x31.get(GRB_DoubleAttr_X);
    cout<<x32.get(GRB_DoubleAttr_X);
    cout<<y1.get(GRB_DoubleAttr_X);
    cout<<y2.get(GRB_DoubleAttr_X);
    cout<<y3.get(GRB_DoubleAttr_X);
    cout<<z1.get(GRB_DoubleAttr_X);
    cout<<z2.get(GRB_DoubleAttr_X);
    cout<<s1.get(GRB_DoubleAttr_X);
    cout<<s2.get(GRB_DoubleAttr_X);
    cout<<s3.get(GRB_DoubleAttr_X);

    model.write("model1.lp");

  } catch(GRBException e) {
    cout << "Error code = " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  } catch(...) {
    cout << "Exception during optimization" << endl;
  }

  return 0;
}
