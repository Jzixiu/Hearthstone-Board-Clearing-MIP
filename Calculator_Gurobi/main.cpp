#include <iostream>
#include "gurobi_c++.h"
using namespace std;

#define var_M (10000)

int main()
{
    // freopen("data","r",stdin);

    try{

    // Create an environment
    GRBEnv env = GRBEnv();
    env.set(GRB_IntParam_OutputFlag,0);
    env.start();

    // Create an empty model
    GRBModel model=GRBModel(env);

    //num of friendly
    int m;
    //num of enemy
    int n;

    //Atk of friendly
    int *A;
    //Hp of friendly
    int *B;
    //Atk of enemy
    int *P;
    //Hp of enemy
    int *Q;
    //Attack
    GRBVar **x;
    //Whether friendly survived
    GRBVar *y;
    //Whether enemy survived
    GRBVar *z;
    //Score of friendly
    GRBVar *s;
    //Total attack that enemy took
    GRBLinExpr *C;
    //Total attack that friendly took
    GRBLinExpr *D;

    cout<<"输入友方随从数量:";
    cin>>m;
    cout<<"输入敌方随从数量:";
    cin>>n;

    A=(int*)calloc(m+1,sizeof(int));
    B=(int*)calloc(m+1,sizeof(int));
    P=(int*)calloc(n+1,sizeof(int));
    Q=(int*)calloc(n+1,sizeof(int));
    x=(GRBVar**)calloc(m+1,sizeof(GRBVar*));
    for(int i=1;i<=m;i++)
    {
        x[i]=(GRBVar*)calloc(n+1,sizeof(GRBVar));
    }
    y=(GRBVar*)calloc(m+1,sizeof(GRBVar));
    z=(GRBVar*)calloc(n+1,sizeof(GRBVar));
    s=(GRBVar*)calloc(m+1,sizeof(GRBVar));
    C=(GRBLinExpr*)calloc(n+1,sizeof(GRBLinExpr));
    D=(GRBLinExpr*)calloc(m+1,sizeof(GRBLinExpr));

    for(int i=1;i<=m;i++)
    {
        cout<<"输入友方第"<<i<<"个随从的攻击力:";
        cin>>A[i];
        cout<<"输入友方第"<<i<<"个随从的生命值:";
        cin>>B[i];
        cout<<endl;
    }
    for(int i=1;i<=n;i++)
    {
        cout<<"输入敌方第"<<i<<"个随从的攻击力:";
        cin>>P[i];
        cout<<"输入敌方第"<<i<<"个随从的生命值:";
        cin>>Q[i];
        cout<<endl;
    }

    //Variable initialization
    for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=n;j++)
        {
            x[i][j]=model.addVar(0,1,0,GRB_BINARY);
        }
    }
    for(int i=1;i<=m;i++)
    {
        y[i]=model.addVar(0,1,0,GRB_BINARY);
    }
    for(int i=1;i<=n;i++)
    {
        z[i]=model.addVar(0,1,0,GRB_BINARY);
    }

    //Linear expression initialization
    for(int j=1;j<=n;j++)
    {
        GRBLinExpr temp;
        for(int i=1;i<=m;i++)
        {
            temp=x[i][j]*A[i];
            C[j]+=temp;
        }
    }
    for(int i=1;i<=m;i++)
    {
        GRBLinExpr temp;
        for(int j=1;j<=n;j++)
        {
            temp=x[i][j]*P[j];
            D[i]+=temp;
        }        
    }    

    //Objective initialization
    GRBLinExpr target;
    for(int i=1;i<=n;i++)
    {
        GRBLinExpr temp;
        temp=P[i]*z[i];        
        target+=temp;
    }
    model.setObjective(target,GRB_MINIMIZE);

    //Constraints initialization
    for(int i=1;i<=m;i++)
    {
        GRBLinExpr temp;
        for(int j=1;j<=n;j++)
        {
            temp+=x[i][j];
        }
        model.addConstr(temp<=1);
    }
    for(int i=1;i<=m;i++)
    {
        model.addConstr(D[i]+var_M*y[i] >= B[i]);
        model.addConstr(D[i]+var_M*y[i] <= B[i]+var_M-1);        
    }
    for(int j=1;j<=n;j++)
    {
        model.addConstr(C[j]+var_M*z[j] >= Q[j]);
        model.addConstr(C[j]+var_M*z[j] <= Q[j]+var_M-1);
    }

    model.optimize();
    int O=model.get(GRB_DoubleAttr_ObjVal);

    //Start of Second optimization

    //Add variable s
    for(int i=1;i<=m;i++)
    {
        s[i]=model.addVar(0,GRB_INFINITY,0,GRB_INTEGER,"s"+to_string(i));
    }

    //Add constraints
    for(int i=1;i<=m;i++)
    {
        model.addConstr(s[i]-y[i]*var_M <= 0);
    }
    for(int i=1;i<=m;i++)
    {
        model.addConstr(s[i]+A[i]*D[i]-y[i]*var_M >= A[i]*B[i]-var_M);
    }
    for(int i=1;i<=m;i++)
    {
        model.addConstr(s[i]+A[i]*D[i]+y[i]*var_M <= A[i]*B[i]+var_M);
    }
    model.addConstr(target <= O);
    
    GRBLinExpr target2;
    for(int i=1;i<=m;i++)
    {
        target2+=s[i];
    }
    model.setObjective(target2,GRB_MAXIMIZE);

    model.update();
    model.optimize();

    cout<<"解场方式:"<<endl;
    for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=n;j++)
        {
            if((int)x[i][j].get(GRB_DoubleAttr_X)==1)
            {
                cout<<"友方["<<i<<"]("<<A[i]<<"/"<<B[i]<<")攻击敌方["<<j<<"]("<<P[j]<<"/"<<Q[j]<<")"<<endl;
            }
        }
    }

    cout<<"死亡的友方随从:"<<endl;
    for(int i=1;i<=m;i++)
    {
        if((int)y[i].get(GRB_DoubleAttr_X)==0)
        {
            cout<<"["<<i<<"]("<<A[i]<<"/"<<B[i]<<") ";
        }
    }
    cout<<endl;
    cout<<"死亡的敌方随从:"<<endl;
    for(int i=1;i<=n;i++)
    {
        if(((int)z[i].get(GRB_DoubleAttr_X))==0)
        {
            cout<<"["<<i<<"]("<<P[i]<<"/"<<Q[i]<<") ";
        }
    }
    cout<<endl;
    cout<<"解场后:"<<endl;
    cout<<"敌方:"<<endl;
    for(int i=1;i<=n;i++)
    {
        if(((int)z[i].get(GRB_DoubleAttr_X))==1)
        {
            cout<<"["<<i<<"]("<<P[i]<<"/"<<Q[i]<<") ";
        }
    }
    cout<<endl;
    cout<<"友方:"<<endl;
    for(int i=1;i<=m;i++)
    {
        if((int)y[i].get(GRB_DoubleAttr_X)==1)
        {
            cout<<"["<<i<<"]("<<A[i]<<"/"<<B[i]<<") ";
        }
    }
    cout<<endl;
    cout<<"Target="<<model.get(GRB_DoubleAttr_ObjVal)<<endl;
    for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=n;j++)
        {
            cout<<(int)x[i][j].get(GRB_DoubleAttr_X);
        }
    }
    for(int i=1;i<=m;i++)
    {
        cout<<(int)y[i].get(GRB_DoubleAttr_X);
    }
    for(int i=1;i<=n;i++)
    {
        cout<<(int)z[i].get(GRB_DoubleAttr_X);
    }

    // model.write("model.lp");

    free(A);
    free(B);
    free(P);
    free(Q);
    for(int i=1;i<=m;i++) free(x[i]);
    free(x);
    free(y);
    free(z);
    free(s);
    free(C);
    free(D);

    } catch(GRBException e) {
    cout << "Error code = " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
    } catch(...) {
    cout << "Exception during optimization" << endl;
    }

    return 0;
}