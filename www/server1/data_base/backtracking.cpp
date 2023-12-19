#include<iostream>
#include<vector>

using namespace std;


int queens(int row,
            std::vector<int>& col, 
            std::vector<int>& diag1, 
            std::vector<int>& diag2) // 8, 8x8 => n, nxn
{ // O(n!) -> 14 
    // NP = Non-Polinomial
    const int n = col.size();
    if (row == n)
    {
        return 1;
    }
    int count = 0;
    for (int column = 0; column < n; ++column)
    {
        if (col[column] == 0 && diag1[column + row] == 0 && diag2[row - column + (n - 1)] == 0)
        {
            col[column] = diag1[column + row] = diag2[row - column + (n - 1)] = 1;
            count += queens(row + 1, col, diag1, diag2);
            col[column] = diag1[column + row] = diag2[row - column + (n - 1)] = 0;
        }
    }
    return count;
}
/* row - column = {n - 1, n - 2, n - 3, ..., 0, -1, -2, -3, ..., -(n - 1)}
* // {2 * n - 2, 2 * n - 3, 2 * n - 4, ..., n - 1, n - 2, ..., 0}
01001001 -(n - 1)
00101010
00011100
11111111
00011100
00101010
01001001
10001000
(n - 1)

*/

/*
col, diag1, diag2
*/



int main()
{
    for (int n = 1; n <= 40; ++n)
    {
        std::vector<int> col(n);
        std::vector<int> diag1(2 * n - 1);
        std::vector<int> diag2(2 * n - 1);
        std::cout << n << " = " << queens(0, col, diag1, diag2) << std::endl;
    }
}