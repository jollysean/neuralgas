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
#include <DataGenerator/BinaryAutomata.h>

using namespace std;
using namespace neuralgas;

int main(int argc, char *argv[])
{
    BinaryAutomata ba;
    ba.generate(1000);
    std::vector<Vector<double>* > * data = ba.getData();
    for(unsigned int i=0; i < data->size(); i++)
            std::cout << (*((*data)[i]))[0] << " ";
    return EXIT_SUCCESS;
}
