#include <iostream>
// #include <malloc.h>
#include <glpk.h>
using namespace std;

#define var_M (10000) //Some sufficiecntly big number


int main()
{
    //保证中文输出
    // system("chcp 65001 >nul");

    //友方随从数量
    int m;
    //友方随从攻击力
    int* Friendly_Atk;

    //敌方随从数量
    int n;
    //敌方随从攻击力
    int* Enemy_Atk;
    //敌方随从血量
    int* Enemy_Hp;

    
    //矩阵加载
    int *ia,*ja;
    double* ar;
    int col_num,row_num;

    glp_prob *lp=glp_create_prob();

    cout<<"友方随从数量:";
    cin>>m;
    cout<<"敌方随从数量:";
    cin>>n;

    
    col_num=(m+1)*n;
    row_num=m+n;

    Friendly_Atk=(int*)calloc(m,sizeof(int));
    Enemy_Atk=(int*)calloc(n,sizeof(int));
    Enemy_Hp=(int*)calloc(n,sizeof(int));
    ia=(int*)calloc(col_num*row_num,sizeof(int));
    ja=(int*)calloc(col_num*row_num,sizeof(int));
    ar=(double*)calloc(col_num*row_num,sizeof(double));

    for(int i=0;i<m;i++)
    {
        cout<<"输入友方第"<<i+1<<"个随从的攻击力:";
        cin>>Friendly_Atk[i];
        cout<<endl;
    }
    for(int i=0;i<n;i++)
    {
        cout<<"输入敌方第"<<i+1<<"个随从的攻击力:";
        cin>>Enemy_Atk[i];
        cout<<"输入敌方第"<<i+1<<"个随从的生命值:";
        cin>>Enemy_Hp[i];
        cout<<endl;
    }

    glp_set_obj_dir(lp,GLP_MAX);

    glp_add_rows(lp,row_num);
    for(int i=1;i<=m;i++)
    {
        glp_set_row_bnds(lp,i,GLP_UP,0.0,1.0);
    }
    for(int i=1;i<=n;i++)
    {
        glp_set_row_bnds(lp,m+i,GLP_DB,Enemy_Hp[i-1]-var_M,Enemy_Hp[i-1]-1);
    }
    
    glp_add_cols(lp,col_num);
    for(int i=1;i<=col_num;i++)
    {
        glp_set_col_kind(lp,i,GLP_BV);
    }

    for(int i=1;i<=n;i++)
    {
        glp_set_obj_coef(lp,m*n+i,Enemy_Atk[i-1]);
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
        for(int k=1;k<=n;k++)
        {
            ia[count]=i;
            ja[count]=m*n+k;
            ar[count]=0;
            count++;
        }
    }

    for(int x=1;x<=n;x++)
    {
        for(int i=1;i<=m;i++)
        {
            for(int j=1;j<=n;j++)
            {
                if(j==x)
                {
                    ia[count]=m+x;
                    ja[count]=(i-1)*n+j;
                    ar[count]=Friendly_Atk[i-1];
                    count++;
                }
                else
                {
                    ia[count]=m+x;
                    ja[count]=(i-1)*n+j;
                    ar[count]=0;
                    count++;
                }
            }
        }
        for(int i=1;i<=n;i++)
        {
            if(i==x)
            {
                ia[count]=m+x;
                ja[count]=m*n+i;
                ar[count]=-1*var_M;
                count++;
            }
            else
            {
                ia[count]=m+x;
                ja[count]=m*n+i;
                ar[count]=0;
                count++;
            }
        }
    }

    int matrix[row_num+1][col_num+1];

    for(int i=1;i<count;i++)
    {
        printf("Pos(%d,%d)=%.0f\n",ia[i],ja[i],ar[i]);
        matrix[ia[i]][ja[i]]=ar[i];
    }

    for(int i=1;i<=row_num;i++)
    {
        for(int j=1;j<=col_num;j++)
        {
            cout<<matrix[i][j]<<" ";
        }
        cout<<endl;
    }

    cout<<endl<<count<<endl;

    glp_load_matrix(lp,count-1,ia,ja,ar);
    glp_simplex(lp,NULL);
    glp_intopt(lp,NULL);

    int sum=0;
    for(int i=0;i<n;i++) sum+=Enemy_Atk[i];

    cout<<endl<<"敌方最低总场攻为"<<sum-(int)glp_mip_obj_val(lp)<<endl;
    cout<<"解场方式:"<<endl;
    for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=n;j++)
        {
            if(glp_mip_col_val(lp,(i-1)*n+j)==1)
            {
                cout<<"友方第"<<i<<"个随从攻击敌方第"<<j<<"个随从"<<endl;
            }
        }
    }

    cout<<endl;
    
    cout<<"死亡的敌方随从:";
    for(int i=1;i<=n;i++)
    {
        if(glp_mip_col_val(lp,m*n+i)) cout<<i<<" ";
    }

    cout<<endl<<endl;

    cout<<"LP松弛目标="<<glp_get_obj_val(lp)<<endl;
    cout<<"LP松弛解:"<<endl;
    for(int i=1;i<=m*n+n;i++) cout<<glp_get_col_prim(lp,i)<<" ";

    cout<<endl<<endl;

    cout<<"LP整数目标="<<glp_mip_obj_val(lp)<<endl;
    cout<<"LP整数解:"<<endl;
    for(int i=1;i<=m*n+n;i++) cout<<glp_mip_col_val(lp,i)<<" ";

    free(Friendly_Atk);
    free(Enemy_Atk);
    free(Enemy_Hp);
    free(ia);
    free(ja);
    free(ar);
    glp_delete_prob(lp);
    glp_free_env();
    return 0;
}