#include <cstdlib>
#include <iostream>
#include <DataGenerator/NoisyAutomata.h>

using namespace std;



int main(int argc, char *argv[])
{
    NoisyAutomata na;
    na.generate(1000);
    vector<Vector<float>*>* data = na.getData();
  
    for (int i=0; i < data->size(); i++)
        std::cout <<data->operator[](i)->operator[](0)<<" "<<data->operator[](i)->operator[](1)<<std::endl;
    return EXIT_SUCCESS;
}
