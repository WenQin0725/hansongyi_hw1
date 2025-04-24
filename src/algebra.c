#include "algebra.h"
#include <stdio.h>
#include <math.h>



Matrix create_matrix(int row, int col)
{
    Matrix m;
    m.rows = row;
    m.cols = col;
    return m;
}




Matrix K_Matrix(Matrix a,int I,int J)
{
    Matrix m;
    m.rows = a.rows-1;
    m.cols = a.cols-1;
    int ai = 0,aj = 0;
    for(int _i=0;_i<m.rows;_i++)
    {
        if(_i==I)
        {
            ai++;
        }
        for(int _j=0;_j<m.cols;_j++)
        {
            if(_j==J)
            {
                aj++;
            }
            m.data[_i][_j] = a.data[ai][aj];
            aj++;
        }
        aj = 0;
        ai++;
    }
    return m;
}

int signal(int i,int j)
{
    if((i+j)%2==0)
        return 1;
    else
    return -1; 
}





Matrix swapRows(Matrix a,int i_1,int i_2)
{
    for(int j=0;j<a.cols;j++)
    {
        double t = a.data[i_1][j];
        a.data[i_1][j] = a.data[i_2][j];
        a.data[i_2][j] = t;
    }
    return a;
}





int checkZeroRowBlow(Matrix a,int I)
{
    int flag = 1;
    for(int i = I+1;i<a.rows;i++)
    {
        for(int j=0;j<a.cols;j++)
        {
            if(a.data[i][j]!=0)
            {
                flag--;
                break;
            }
            if(flag==0)
            break;
        }
    }
    if(flag==1)
    return 1;
    else
    return 0;
}




Matrix Elimination(Matrix a,int I,int J)
{
    for(int i=I+1;i<a.rows;i++)
    {
        if(a.data[i][J]!=0)
        {
            double factor = a.data[i][J]/a.data[I][J];
            for(int j=J;j<a.cols;j++)
            {
                a.data[i][j] -= factor*a.data[I][j];
            }
        }
        else
        {
            continue;
        }
    }
    return a;
}




Matrix add_matrix(Matrix a, Matrix b)
{
    if (a.rows!=b.rows || a.cols != b.cols)
    {
        printf("Error: Matrix a and b must have the same rows and cols.\n");
        return create_matrix(0, 0);
    }
    else
    {
        for(int i=0;i<a.rows;i++)
        {
            for (int j=0;j<a.cols;j++)
            {
                a.data[i][j]+=b.data[i][j];
            }
        }
    }
    return a;
}




Matrix sub_matrix(Matrix a, Matrix b)
{
    if (a.rows!=b.rows || a.cols != b.cols)
    {
        printf("Error: Matrix a and b must have the same rows and cols.\n");
        return create_matrix(0, 0);
    }
    else
    {
        for(int i=0;i<a.rows;i++)
        {
            for (int j=0;j<a.cols;j++)
            {
                a.data[i][j]-=b.data[i][j];
            }
        }
    }
    return a;
}





Matrix mul_matrix(Matrix a, Matrix b)
{
    if (a.cols!=b.rows)
    {
        printf("Error: The number of cols of matrix a must be equal to the number of rows of matrix b.\n");
        return create_matrix(0, 0);
    }
    else
    {
        Matrix c;
        double sum = 0;
        c.rows = a.rows;
        c.cols = b.cols;
        for(int i=0;i<c.rows;i++)
        {
            for(int j=0;j<c.cols;j++)
            {
                for(int k=0;k<a.cols;k++)
                {
                    sum+=a.data[i][k]*b.data[k][j];
                }
                c.data[i][j] = sum;
                sum = 0;
            }
           
        }
        return c;
    }
}






Matrix scale_matrix(Matrix a, double k)
{
    for(int i=0;i<a.rows;i++)
    {
        for(int j=0;j<a.cols;j++)
        {
            a.data[i][j]*=k;
        }
    }
    return a;
}







Matrix transpose_matrix(Matrix a)
{
    Matrix c;
    c.rows = a.cols;
    c.cols = a.rows;
    for(int i=0;i<c.rows;i++)
    {
        for(int j=0;j<c.cols;j++)
        {
            c.data[i][j] = a.data[j][i];
        }
    }
    return c;
}







double det_matrix(Matrix a)
{
    if(a.rows!=a.cols)
    {
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }
    else
    {
        double sum = 0;
        if(a.cols==1)
        {
            return a.data[0][0];
        }
        else
        {
            for(int i=0;i<a.rows;i++)
            {
                    Matrix m = K_Matrix(a,i,0);
                    sum += signal(i,0)*det_matrix(m)*a.data[i][0];   
            }
        }
         return sum;
    }
}







Matrix inv_matrix(Matrix a)
{
    if(a.rows!=a.cols)
    {
        printf("Error: The matrix must be a square matrix.\n");
        return create_matrix(0, 0);
    }
    else if(det_matrix(a)==0)
    {
        printf("Error: The matrix is singular.\n");
        return create_matrix(0, 0);
    }
    else
    {
        Matrix c;
        c.rows = a.rows;
        c.cols = a.cols;
        for(int i=0;i<a.rows;i++)
        {
            for(int j=0;j<a.cols;j++)
            {
                c.data[i][j] = signal(i,j)*det_matrix(K_Matrix(a,i,j));
            }
        }
        c = scale_matrix(transpose_matrix(c),1/det_matrix(a));
        return c;
    }
}





int rank_matrix(Matrix a)
{
    int rank = 0;
    int i = 0;
    int finish = 0;
    for(int j=0;j<a.cols;j++)
    {
        if(i<a.rows-1)
        {
            if(a.data[i][j]==0)
            {
                int found = 0;
                    for(int _i=i+1;_i<a.rows;_i++)
                    {
                        if(a.data[_i][j]!=0)
                        {
                            a = swapRows(a,i,_i);
                            found++;
                            break;
                        }
                    }
                    if(found==0)
                    {
                        continue;
                    }
            }
	        a = Elimination(a,i,j);
	        rank++;
	        finish = checkZeroRowBlow(a,i);
	        i++;
	        if(finish==1)
	        {
	            break;
	        }
        }
        else
        {
            int found = 0;
            for(int _j=j;_j<a.cols;_j++)
            {
                if(a.data[i][_j]!=0)
                {
                    found++;
                    rank++;
                    break;
                }
            }
            break;
        }
        
    }
    return rank;
}





double trace_matrix(Matrix a)
{
    if(a.rows!=a.cols)
    {
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }
    else
    {
        double trace = 0;
        for(int i=0;i<a.rows;i++)
        {
            trace+=a.data[i][i];
        }
        return trace;
    }
}




void print_matrix(Matrix a)
{
    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            // 按行打印，每个元素占8个字符的宽度，小数点后保留2位，左对齐
            printf("%-8.2f", a.data[i][j]);
        }
        printf("\n");
    }
}