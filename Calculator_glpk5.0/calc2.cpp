#include <iostream>
#include <vector>
// #include <malloc.h>
#include <glpk.h>
using namespace std;

#define var_M 10000

void readin(vector<int>& vec)
{
    int temp;
    cin>>temp;
    vec.push_back(temp);
}

int main()
{
    freopen("/Users/zixiu.li/desktop/code/build/data","r",stdin);
    //保证中文输出
    // system("chcp 65001 >nul");

    //Atk of friendly
    vector<int> A;
    A.push_back(0);//1-indexed
    //Hp of friendly
    vector<int> B;
    B.push_back(0);

    //Atk of enemy
    vector<int> P;
    P.push_back(0);
    //Hp of enemy
    vector<int> Q;
    Q.push_back(0);

    //num of friendly minions
    int m;
    //num of enemy minions
    int n;

    //Matrix loading
    int *ia,*ja;
    double* ar;
    int col_num,row_num;

    glp_prob *lp=glp_create_prob();

    cout<<"友方随从数量:";
    cin>>m;
    cout<<"敌方随从数量:";
    cin>>n;

    col_num=m*n+m+n;
    row_num=2*m+n;

    int total=(m*n+2*m+n)*(6*m+n+1);

    ia=(int*)calloc(total+1,sizeof(int));
    ja=(int*)calloc(total+1,sizeof(int));
    ar=(double*)calloc(total+1,sizeof(int));


    for(int i=0;i<m;i++)
    {
        cout<<"输入友方第"<<i+1<<"个随从的攻击力:";
        readin(A);
        cout<<"输入友方第"<<i+1<<"个随从的生命值:";
        readin(B);
        cout<<endl;
    }
    for(int i=0;i<n;i++)
    {
        cout<<"输入敌方第"<<i+1<<"个随从的攻击力:";
        readin(P);
        cout<<"输入敌方第"<<i+1<<"个随从的生命值:";
        readin(Q);
        cout<<endl;
    }

    glp_add_cols(lp,col_num);
    for(int k=1;k<=col_num;k++)
    {
        glp_set_col_kind(lp,k,GLP_BV);
    }

    glp_add_rows(lp,row_num);
    for(int i=1;i<=m;i++)
    {
        glp_set_row_bnds(lp,i,GLP_UP,0.0,1.0);
    }
    for(int i=1;i<=m;i++)
    {
        glp_set_row_bnds(lp,m+i,GLP_DB,B[i],B[i]+var_M-1);
    }
    for(int j=1;j<=n;j++)
    {
        glp_set_row_bnds(lp,2*m+j,GLP_DB,Q[j],Q[j]+var_M-1);
    }

    int count=1;
    for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=m;j++)
        {
            if(i==j)
            {
               for(int k=1;k<=n;k++)
                {
                    ia[count]=i;
                    ja[count]=(j-1)*n+k;
                    ar[count]=1;
                    count++;
                }
            }
            else
            {
                for(int k=1;k<=n;k++)
                {
                    ia[count]=i;
                    ja[count]=(j-1)*n+k;
                    ar[count]=0;
                    count++;
                }
            }
        }
        for(int k=1;k<=m+n;k++)
        {
            ia[count]=i;
            ja[count]=m*n+k;
            ar[count]=0;
            count++;
        }
    }

    for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=m;j++)
        {
            if(i==j)
            {
               for(int k=1;k<=n;k++)
                {
                    ia[count]=m+i;
                    ja[count]=(j-1)*n+k;
                    ar[count]=P[k];
                    count++;
                }
            }
            else
            {
                for(int k=1;k<=n;k++)
                {
                    ia[count]=m+i;
                    ja[count]=(j-1)*n+k;
                    ar[count]=0;
                    count++;
                }
            }
        }
        for(int k=1;k<=m;k++)
        {
            ia[count]=m+i;
            ja[count]=m*n+k;
            if(k==i) ar[count]=var_M;
            else ar[count]=0;
            count++;
        }
        for(int k=1;k<=n;k++)
        {
            ia[count]=m+i;
            ja[count]=m*n+m+k;
            ar[count]=0;
            count++;
        }
    }

    for(int j=1;j<=n;j++)
    {
        for(int i=1;i<=m;i++)
        {
            for(int k=1;k<=n;k++)
            {
                ia[count]=2*m+j;
                ja[count]=(i-1)*n+k;
                if(j==k) ar[count]=A[i];
                else ar[count]=0;
                count++;
            }
        }
        for(int k=1;k<=m;k++)
        {
            ia[count]=2*m+j;
            ja[count]=m*n+k;
            ar[count]=0;
            count++;
        }
        for(int k=1;k<=n;k++)
        {
            ia[count]=2*m+j;
            ja[count]=m*n+m+k;
            if(k==j) ar[count]=var_M;
            else ar[count]=0;
            count++;
        }
    }

    glp_load_matrix(lp,count-1,ia,ja,ar);
    glp_set_obj_dir(lp,GLP_MIN);
    for(int j=1;j<=n;j++)
    {
        glp_set_obj_coef(lp,m*n+m+j,P[j]);
    }
    glp_simplex(lp,NULL);
    glp_intopt(lp,NULL);

    int O_=glp_mip_obj_val(lp);
    cout<<O_<<endl;

    // cout<<"LP松弛目标="<<glp_get_obj_val(lp)<<endl;
    // cout<<"LP松弛解:"<<endl;
    // for(int i=1;i<=col_num;i++) cout<<glp_get_col_prim(lp,i)<<" ";

    // cout<<endl<<endl;

    // cout<<"LP整数目标="<<glp_mip_obj_val(lp)<<endl;
    // cout<<"LP整数解:"<<endl;
    // for(int i=1;i<=col_num;i++) cout<<glp_mip_col_val(lp,i)<<" ";

    glp_delete_prob(lp);
    glp_free_env();

    glp_prob *lp2=glp_create_prob();

    glp_add_cols(lp2,col_num+m);
    for(int k=1;k<=col_num;k++)
    {
        glp_set_col_kind(lp2,k,GLP_BV);
    }
    for(int k=1;k<=m;k++)
    {
        glp_set_col_kind(lp2,col_num+k,GLP_IV);
    }
    for(int k=1;k<=col_num+m;k++)
    {
        glp_set_obj_coef(lp2,k,0.0);
    }

    glp_add_rows(lp2,row_num+4*m+1);
    for(int i=1;i<=m;i++)
    {
        glp_set_row_bnds(lp2,i,GLP_UP,0.0,1.0);
    }
    for(int i=1;i<=m;i++)
    {
        glp_set_row_bnds(lp2,m+i,GLP_DB,B[i],B[i]+var_M-1);
    }
    for(int j=1;j<=n;j++)
    {
        glp_set_row_bnds(lp2,2*m+j,GLP_DB,Q[j],Q[j]+var_M-1);
    }
    for(int i=1;i<=m;i++)
    {
        glp_set_row_bnds(lp2,row_num+i,GLP_LO,0.0,0.0);
    }
    for(int i=1;i<=m;i++)
    {
        glp_set_row_bnds(lp2,row_num+m+i,GLP_UP,0.0,0.0);
    }
    for(int i=1;i<=m;i++)
    {
        glp_set_row_bnds(lp2,row_num+2*m+i,GLP_LO,A[i]*B[i]-var_M,0.0);
    }
    for(int i=1;i<=m;i++)
    {
        glp_set_row_bnds(lp2,row_num+3*m+i,GLP_UP,0.0,A[i]*B[i]+var_M);
    }
    glp_set_row_bnds(lp2,row_num+4*m+1,GLP_UP,0.0,O_);

    for(int i=1;i<=row_num;i++)
    {
        for(int j=1;j<=m;j++)
        {
            ia[count]=i;
            ja[count]=col_num+j;
            ar[count]=0;
            count++;
        }
    }

    for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=col_num;j++)
        {
            ia[count]=row_num+i;
            ja[count]=j;
            ar[count]=0;
            count++;
        }
        for(int j=1;j<=m;j++)
        {
            ia[count]=row_num+i;
            ja[count]=col_num+j;
            if(i==j) ar[count]=1;
            else ar[count]=0;
            count++;
        }
    }

    for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=m*n;j++)
        {
            ia[count]=row_num+m+i;
            ja[count]=j;
            ar[count]=0;
            count++;
        }
        for(int j=1;j<=m;j++)
        {
            ia[count]=row_num+m+i;
            ja[count]=m*n+j;
            if(i==j) ar[count]=-1*var_M;
            else ar[count]=0;
            count++;
        }
        for(int j=1;j<=n;j++)
        {
            ia[count]=row_num+m+i;
            ja[count]=m*n+m+j;
            ar[count]=0;
            count++;
        }
        for(int j=1;j<=m;j++)
        {
            ia[count]=row_num+m+i;
            ja[count]=col_num+j;
            if(i==j) ar[count]=1;
            else ar[count]=0;
            count++;
        }
    }

    for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=m;j++)
        {
            for(int k=1;k<=n;k++)
            {
                ia[count]=row_num+2*m+i;
                ja[count]=(j-1)*n+k;
                if(i==j) ar[count]=A[i]*P[k];
                else ar[count]=0;
                count++;
            }
        }
        for(int j=1;j<=m;j++)
        {
            ia[count]=row_num+2*m+i;
            ja[count]=m*n+j;
            if(i==j) ar[count]=-1*var_M;
            else ar[count]=0;
            count++;
        }
        for(int j=1;j<=n;j++)
        {
            ia[count]=row_num+2*m+i;
            ja[count]=m*n+m+j;
            ar[count]=0;
            count++;
        }
        for(int j=1;j<=m;j++)
        {
            ia[count]=row_num+2*m+i;
            ja[count]=col_num+j;
            if(i==j) ar[count]=1;
            else ar[count]=0;
            count++;
        }
    }

    for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=m;j++)
        {
            for(int k=1;k<=n;k++)
            {
                ia[count]=row_num+3*m+i;
                ja[count]=(j-1)*n+k;
                if(i==j) ar[count]=A[i]*P[k];
                else ar[count]=0;
                count++;
            }
        }
        for(int j=1;j<=m;j++)
        {
            ia[count]=row_num+3*m+i;
            ja[count]=m*n+j;
            if(i==j) ar[count]=var_M;
            else ar[count]=0;
            count++;
        }
        for(int j=1;j<=n;j++)
        {
            ia[count]=row_num+3*m+i;
            ja[count]=m*n+m+j;
            ar[count]=0;
            count++;
        }
        for(int j=1;j<=m;j++)
        {
            ia[count]=row_num+3*m+i;
            ja[count]=col_num+j;
            if(i==j) ar[count]=1;
            else ar[count]=0;
            count++;
        }
    }

    for(int j=1;j<=m*n+m;j++)
    {
        ia[count]=row_num+4*m+1;
        ja[count]=j;
        ar[count]=0;
        count++;
    }
    for(int k=1;k<=n;k++)
    {
        ia[count]=row_num+4*m+1;
        ja[count]=m*n+m+k;
        ar[count]=P[k];
        count++;
    }
    for(int j=1;j<=m;j++)
    {
        ia[count]=row_num+4*m+1;
        ja[count]=col_num+j;
        ar[count]=0;
        count++;
    }

    int matrix[row_num+4*m+1+1][col_num+m+1];

    for(int i=1;i<count;i++)
    {
        printf("Pos(%d,%d)=%.0f\n",ia[i],ja[i],ar[i]);
        matrix[ia[i]][ja[i]]=ar[i];
    }

    for(int i=1;i<=row_num+4*m+1;i++)
    {
        cout<<"{";
        for(int j=1;j<=col_num+m;j++)
        {
            cout<<matrix[i][j]<<",";
        }
        cout<<"},"<<endl;
    }

    cout<<endl<<count<<endl;

    glp_load_matrix(lp2,count-1,ia,ja,ar);
    glp_set_obj_dir(lp2,GLP_MAX);
    for(int i=1;i<=m;i++)
    {
        glp_set_obj_coef(lp2,col_num+i,1);
    }
    glp_simplex(lp2,NULL);
    glp_intopt(lp2,NULL);

    cout<<endl<<endl;

    cout<<"LP松弛目标="<<glp_get_obj_val(lp2)<<endl;
    cout<<"LP松弛解:"<<endl;
    for(int i=1;i<=col_num+m;i++) cout<<glp_get_col_prim(lp2,i)<<" ";

    cout<<endl<<endl;

    cout<<"LP整数目标="<<glp_mip_obj_val(lp2)<<endl;
    cout<<"LP整数解:"<<endl;
    for(int i=1;i<=col_num+m;i++) cout<<glp_mip_col_val(lp2,i)<<" ";

    // free(ia);
    // free(ja);
    // free(ar);
    

    return 0;
}