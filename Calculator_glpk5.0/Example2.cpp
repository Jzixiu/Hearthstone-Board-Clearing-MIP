#include <stdio.h>            /* C input/output                       */
#include <stdlib.h>           /* C standard library                   */
#include <glpk.h>
#include <iostream>
using namespace std;

int main(void)
{
  /* declare variables */
  glp_prob *lp;
  int ia[1+1000], ja[1+1000];
  double ar[1+1000];
  double z;
  /* create problem */
  lp = glp_create_prob();
  glp_set_prob_name(lp, "HS");
  glp_set_obj_dir(lp, GLP_MAX);
  /* fill problem */
  glp_add_rows(lp, 21);
  glp_set_row_bnds(lp, 1, GLP_UP, 0.0, 1.0);
  glp_set_row_bnds(lp, 2, GLP_UP, 0.0, 1.0);
  glp_set_row_bnds(lp, 3, GLP_UP, 0.0, 0.0);
  glp_set_row_bnds(lp, 4, GLP_DB, 1.0, 10000.0);
  glp_set_row_bnds(lp, 5, GLP_DB, 1.0, 10000.0);
  glp_set_row_bnds(lp, 6, GLP_DB, 1.0, 10000.0);
  glp_set_row_bnds(lp, 7, GLP_DB, 3.0, 10002.0);
  glp_set_row_bnds(lp, 8, GLP_DB, 4.0, 10003.0);
  glp_set_row_bnds(lp, 9, GLP_LO, 0.0, 0.0);
  glp_set_row_bnds(lp, 10, GLP_LO, 0.0, 0.0);
  glp_set_row_bnds(lp, 11, GLP_LO, 0.0, 0.0);
  glp_set_row_bnds(lp, 12, GLP_UP, 0.0, 0.0);
  glp_set_row_bnds(lp, 13, GLP_UP, 0.0, 0.0);
  glp_set_row_bnds(lp, 14, GLP_UP, 0.0, 0.0);
  glp_set_row_bnds(lp, 15, GLP_LO, -9998, 0.0);
  glp_set_row_bnds(lp, 16, GLP_LO, -9998, 0.0);
  glp_set_row_bnds(lp, 17, GLP_LO, -9998, 0.0);
  glp_set_row_bnds(lp, 18, GLP_UP, 0.0, 10002.0);
  glp_set_row_bnds(lp, 19, GLP_UP, 0.0, 10002.0);
  glp_set_row_bnds(lp, 20, GLP_UP, 0.0, 10002.0);
  glp_set_row_bnds(lp, 21, GLP_UP, 0.0, 2.0);
  // glp_set_row_bnds(lp, 22, GLP_FX, 0.0, 0.0);
  




  glp_add_cols(lp, 14);
  for(int i=1;i<=11;i++)
  {
    glp_set_col_kind(lp, i, GLP_BV);
  }
  for(int i=12;i<=14;i++)
  {
    glp_set_col_kind(lp, i, GLP_IV);    
  }

  glp_set_obj_coef(lp,12,1.0);
  glp_set_obj_coef(lp,13,1.0);
  glp_set_obj_coef(lp,14,1.0);

  int matrix[21][14]=
  {
    {1,1,0,0,0,0,0,0,0,0,0,0,0,0,},//1
    {0,0,1,1,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,1,1,0,0,0,0,0,0,0,0,},//3
    {2,4,0,0,0,0,10000,0,0,0,0,0,0,0,},
    {0,0,2,4,0,0,0,10000,0,0,0,0,0,0,},//5
    {0,0,0,0,2,4,0,0,10000,0,0,0,0,0,},
    {2,0,2,0,2,0,0,0,0,10000,0,0,0,0,},//7
    {0,2,0,2,0,2,0,0,0,0,10000,0,0,0,},//8
    {0,0,0,0,0,0,0,0,0,0,0,1,0,0,},//9
    {0,0,0,0,0,0,0,0,0,0,0,0,1,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,1},//11
    {0,0,0,0,0,0,-10000,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,-10000,0,0,0,0,1,0},//13
    {0,0,0,0,0,0,0,0,-10000,0,0,0,0,1},//14
    {4,8,0,0,0,0,-10000,0,0,0,0,1,0,0},//15
    {0,0,4,8,0,0,0,-10000,0,0,0,0,1,0},
    {0,0,0,0,4,8,0,0,-10000,0,0,0,0,1},//17
    {4,8,0,0,0,0,10000,0,0,0,0,1,0,0},//18
    {0,0,4,8,0,0,0,10000,0,0,0,0,1,0},//19
    {0,0,0,0,4,8,0,0,10000,0,0,0,0,1},//20
    {0,0,0,0,0,0,0,0,0,2,4,0,0,0},
    // {0,0,0,0,0,0,0,0,0,0,0,0,0,0}
  };


  int count=1;
  for(int i=0;i<21;i++)
  {
    for(int j=0;j<14;j++)
    {
      ia[count]=i+1;
      ja[count]=j+1;
      ar[count]=matrix[i][j];
      count++;
    }
  }

  glp_load_matrix(lp, count-1, ia, ja, ar);
  /* solve problem */
  glp_simplex(lp,NULL);
  glp_intopt(lp, NULL);
  /* recover and display results */
  z = glp_get_obj_val(lp);

  cout<<endl;
  printf("z = %g\n", z);
  for(int i=1;i<=14;i++)
  {
    cout<<glp_mip_col_val(lp,i);
  }
  /* housekeeping */
  glp_delete_prob(lp);
  glp_free_env();
  return 0;
}