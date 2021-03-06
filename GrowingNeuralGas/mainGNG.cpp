/*
 *   This file is part of NeuralGas.
 *
 *   NeuralGas is free software: you can redistribute it and/or modify it
 *   under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *   NeuralGas is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with NeuralGas.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include <iostream>
#include "GNGAlgorithm.h"
#include "tools/math_helpers.h"

using namespace std;
using namespace neuralgas;

float func(const unsigned int& time)
{return 0.5 ;}

float funclambda(const unsigned int& time)
{return 5 ;}

float functheta(const unsigned int& time)
{return 100;}

int main(int argc, char *argv[])
{
   
     
    std::vector< Vector<float>* >* v=new std::vector<Vector<float>* >;
    for( int i=0; i < 1500; i++)
    {
        Vector<float>* x;
        float odd = 3.7 * i*i -  i*5.2+2.3;
        float even =5.2 * i*i +  i*5.2+2.3;
        if (i % 2 || i%3 == 0)
           x = new Vector<float>(1,even);
        else
           x = new Vector<float>(1,odd);
        v->push_back(x);
       
    }
    GNGAlgorithm<float,float> gng(1);
    
    
    for(int i=0; i < NUM_PARAM; i++)
            gng.setFuncArray(func,i);

    gng.setFuncArray(functheta,7);
    gng.setFuncArray(funclambda,6);

    gng.setData(v);
    gng.setRefVectors(2);
    gng.run();
    gng.showGraph();
    for( int i=0; i < 1500; i++)
         delete (*v)[i];
    
    //delete v;

    return EXIT_SUCCESS;
}
