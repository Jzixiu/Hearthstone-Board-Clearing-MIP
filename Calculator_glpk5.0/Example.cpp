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
  glp_set_obj_dir(lp, GLP_MIN);
  /* fill problem */
  glp_add_rows(lp, 7);
  glp_set_row_bnds(lp, 1, GLP_UP, 0.0, 1.0);
  glp_set_row_bnds(lp, 2, GLP_UP, 0.0, 0.0);
  glp_set_row_bnds(lp, 3, GLP_UP, 0.0, 1.0);
  glp_set_row_bnds(lp, 4, GLP_UP, 0.0, 3.0);
  glp_set_row_bnds(lp, 5, GLP_UP, 0.0, 11.0);
  glp_set_row_bnds(lp, 6, GLP_LO, -9996.0, 0.0);
  glp_set_row_bnds(lp, 7, GLP_LO, -9988.0, 0.0);



  glp_add_cols(lp, 8);
  for(int i=1;i<=8;i++)
  {
    glp_set_col_kind(lp, i, GLP_BV);
  }

  glp_set_obj_coef(lp,7,-3.0);
  glp_set_obj_coef(lp,8,-4.0);

  int matrix[7][8]=
  {
    {1,1,0,0,0,0,0,0},
    {0,0,1,1,0,0,0,0},
    {0,0,0,0,1,1,0,0},
    {2,0,2,0,2,0,-10000,0},
    {0,2,0,2,0,2,0,-10000},
    {2,0,2,0,2,0,-10000,0},
    {0,2,0,2,0,2,0,-10000}
  };


  int count=1;
  for(int i=0;i<7;i++)
  {
    for(int j=0;j<8;j++)
    {
      ia[count]=i+1;
      ja[count]=j+1;
      ar[count]=matrix[i][j];
      count++;
    }
  }

  glp_load_matrix(lp, 56, ia, ja, ar);
  /* solve problem */
  glp_simplex(lp,NULL);
  glp_intopt(lp, NULL);
  /* recover and display results */
  z = glp_mip_obj_val(lp);

  printf("z = %g\n", z);
  for(int i=1;i<=8;i++)
  {
    cout<<glp_mip_col_val(lp,i);
  }
  /* housekeeping */
  glp_delete_prob(lp);
  glp_free_env();
  return 0;
}