#include <iostream>
using namespace std;

int main()
{
    int **matrix = new int *[10];
    for (size_t i = 0; i < 10; i++)
    {
        matrix[i] = new int[10];
    }

    for (size_t i = 0; i < 10; i++)
    {
        for (size_t j = 0; j < 10; j++)
        {
            if (i == 5 && j == 5)
            {
                matrix[i][j] = 1;
                cout << matrix[i][j] << " ";
            }
            else
            {
                matrix[i][j] = 0;
                cout << matrix[i][j] << " ";
            }
        }
        cout << endl;
    }
    int s = 2;
    for (size_t i = 0; i < 10; i++)
    {
        for (size_t j = 0; j < 10; j++)
        {
            if (matrix[i][j] == 1)
            {
                for (size_t k = 0; k <= s; k++)
                {
                    for (size_t l = 0; l <= s; l++)
                    {
                        matrix[i + k][j + l] = 7;
                        matrix[i - k][j - l] = 7;
                        matrix[i + k][j - l] = 7;
                        matrix[i - k][j + l] = 7;
                    }
                }
            }
        }
    }
    cout << endl;
    cout << endl;
    for (size_t i = 0; i < 10; i++)
    {
        for (size_t j = 0; j < 10; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    // cout << endl;
    // cout << endl;
    // for (size_t i = 0; i < 10; i++)
    // {
    //     for (size_t j = 0; j < 10; j++)
    //     {
    //         if (matrix[i][j] == 7)
    //         {
    //             for (size_t k = 0; k < s; k++)
    //             {
    //                 for (size_t l = 0; l < s; l++)
    //                 {
    //                     matrix[i + k][j + l] = 8;
    //                     matrix[i + k][j - l] = 8;
    //                 }
    //             }
    //         }
    //     }
    // }

    // for (size_t i = 0; i < 10; i++)
    // {
    //     for (size_t j = 0; j < 10; j++)
    //     {
    //         cout << matrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }
}