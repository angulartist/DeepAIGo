/* MIT License

Copyright (c) 2017 ParkJunYeong(https://github.com/ParkJunYeong)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include <preprocess/Symmetrics.h>
#include <stdio.h>

void PrintTensor(const DeepAIGo::Tensor& tensor)
{
    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            printf("%d ", (int)tensor[0][i][j]);
        }

        printf("\n");
    }
}

int main()
{
    DeepAIGo::Tensor a { boost::extents[1][3][3] };
    a[0][0][0] = 1;
    a[0][0][1] = 2;
    a[0][0][2] = 3;
    a[0][1][0] = 4;
    a[0][1][1] = 5;
    a[0][1][2] = 6;
    a[0][2][0] = 7;
    a[0][2][1] = 8;
    a[0][2][2] = 9;

    printf("[origin]\n");
    PrintTensor(a);

    auto rot90 = DeepAIGo::Symmetrics::Rot90(a);
    printf("\n[rot90]\n");
    PrintTensor(rot90);

    auto rot180 = DeepAIGo::Symmetrics::Rot180(a);
    printf("\n[rot180]\n");
    PrintTensor(rot180);

    auto rot270 = DeepAIGo::Symmetrics::Rot270(a);
    printf("\n[rot270]\n");
    PrintTensor(rot270);

    auto flipud = DeepAIGo::Symmetrics::FlipUD(a);
    printf("\n[flipud]\n");
    PrintTensor(flipud);

    auto fliplr = DeepAIGo::Symmetrics::FlipLR(a);
    printf("\n[fliplr]\n");
    PrintTensor(fliplr);

    auto diag1 = DeepAIGo::Symmetrics::Diag1(a);
    printf("\n[diag1]\n");
    PrintTensor(diag1);

    auto diag2 = DeepAIGo::Symmetrics::Diag2(a);
    printf("\n[diag2]\n");
    PrintTensor(diag2);

    getchar();
}