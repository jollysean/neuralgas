#include <cstdlib>
#include <iostream>
#include "NeuralGasSuite.h"
#include "GrowingNeuralGas/Testing/ErrorTesting.h"
#include "GrowingNeuralGas/MergeGrowingNeuralGas/MGNGAlgorithm.h"
#include "GrowingNeuralGas/MergeGrowingNeuralGas/CDNAlgorithm.h"
#include "GrowingNeuralGas/GNGAlgorithm.h"
#include "GrowingNeuralGas/ErrorBasedGNGAlgorithm/EBGNGAlgorithm.h"
#include "GrowingNeuralGas/LifelongBasedGNGAlgorithm/LLBGNGAlgorithm.h"
#include "DataGenerator/MackeyGlass.h"
#include "DataGenerator/NoisyAutomata.h"
#include <math.h>

using namespace std;
using namespace neuralgas;

float func(const unsigned int& time)
{return 0.5 ;}


float constalpha(const unsigned int& time)
{return 0.5;}

float constbeta(const unsigned int& time)
{return 0.75;}

float constgamma(const unsigned int& time)
{return 88;}

float constepsilonw(const unsigned int& time)
{return 0.05;}

float constepsilonn(const unsigned int& time)
{return 0.0006;}

float constdelta(const unsigned int& time)
{return 0.5;}

float consteta(const unsigned int& time)
{return 0.99995;}

float consttheta(const unsigned int& time)
{return 100;}

float constlambda(const unsigned int& time)
{return 600 ;}

float funcalpha(const unsigned int& time)
{
      float random_alpha = float(rand()) / float(50000);
      return random_alpha;
}


float funcbeta(const unsigned int& time)
{
     return 0.75 ;
}

float funcgamma(const unsigned int& time)
{return 1+sqrt(time);}

float funclambda(const unsigned int& time)
{return 3 ;}

float functheta(const unsigned int& time)
{
      return 100;
}

enum _algorithm { _gng, _ebgng, _mgng, _cdn, _llbgng };

int main(int argc, char *argv[])
{

    if (argc < 2) {
      cerr << "Usage: " << argv[0] << " gng/ebgng/mgng/cdn/llbgng" << endl;
      exit(1);
    }

    GNGModul<float, int> *gng;
    unsigned int algorithm;
    //MGNGAlgorithm<float,int>* mgng    = new MGNGAlgorithm<float,int>(1);
    //GNGAlgorithm<float,int>* gng      = new GNGAlgorithm<float,int>(1);
    //EBGNGAlgorithm<float,int>* ebgng  = new EBGNGAlgorithm<float,int>(2);
    //CDNAlgorithm<float,int>* cdn      = new CDNAlgorithm<float,int>(1);
    if (string(argv[1]) == "gng")
    {
	    gng = new GNGAlgorithm<float,int>(1);
        algorithm = _gng;
    }
    else if (string(argv[1]) == "ebgng")
    {
	    gng = new EBGNGAlgorithm<float,int>(1);
        algorithm = _ebgng;
    }
    else if (string(argv[1]) == "mgng")
    {
        gng = new MGNGAlgorithm<float,int>(1);
        algorithm = _mgng;
    }
    else if (string(argv[1]) == "cdn")
    {
        gng = new CDNAlgorithm<float,int>(1);
        algorithm = _cdn;
    }
    else if (string(argv[1]) == "llbgng")
    {
        gng = new LLBGNGAlgorithm<float,int>(1);
        algorithm = _llbgng;
    }
    else
    {
        cerr << "Usage: " << argv[0] << " gng/ebgng/mgng/cdn/llbgng" << endl;
        exit(1);
    }

    int sizeofdata=10000;
    
    NeuralGasSuite<float,int> ng;

    
    MackeyGlass* mg = new MackeyGlass;
    mg->setPastTimeSteps(17);
    mg->setBoundary(0.4);
    mg->setPower(10);
    /*NoisyAutomata* na = new NoisyAutomata;
    na->setSigma (0.1);
    na->setTransProb (0.1);*/
    
    
    ng.setDataGenerator(mg);
    //ng.setDataGenerator(na);
    
    
    mg->generate(sizeofdata);
    //na->generate(sizeofdata);
    //vector<Vector<float>*>* data = na->getData();
    vector<Vector<float>*>* data = mg->getData();
    
    for (unsigned int i=0; i < data->size(); i++)
	    std::cout <<data->operator[](i)->operator[](0)<<" "<<data->operator[](i)->operator[](1)<<std::endl;

    ng.add(gng);
    ng.setRefVectors(2);
   
    for(int i=0; i < NUM_PARAM; i++)
    {
        gng->setFuncArray(func,i);
        //ebgng->setFuncArray(func,i);
        //cdn->setFuncArray(func,i);
    }
    
    // mgng->setFuncArray(constalpha,0);
    // mgng->setFuncArray(constbeta,1);
    // mgng->setFuncArray(constgamma,2);
    // mgng->setFuncArray(constdelta,3);
    // mgng->setFuncArray(constepsilonw,4);
    // mgng->setFuncArray(constepsilonn,5);
    // mgng->setFuncArray(consttheta,6);
    // mgng->setFuncArray(consteta,7);
    // mgng->setFuncArray(constlambda,8);
    if (algorithm == _mgng)
    {
        gng->setFuncArray(constalpha,0);
        gng->setFuncArray(constbeta,1);
        gng->setFuncArray(constgamma,2);
        gng->setFuncArray(constdelta,3);
        gng->setFuncArray(constepsilonw,4);
        gng->setFuncArray(constepsilonn,5);
        gng->setFuncArray(consttheta,6);
        gng->setFuncArray(consteta,7);
        gng->setFuncArray(constlambda,8);
    }
    else if (algorithm == _cdn)
    {
        
    // cdn->setFuncArray(constalpha,0);
    // cdn->setFuncArray(constbeta,1);
    // cdn->setFuncArray(constgamma,2);
    // cdn->setFuncArray(constdelta,3);
    // cdn->setFuncArray(constepsilonw,4);
    // cdn->setFuncArray(constepsilonn,5);
    // cdn->setFuncArray(consttheta,6);
    // cdn->setFuncArray(consteta,7);
    // cdn->setFuncArray(constlambda,8);

        gng->setFuncArray(constalpha,0);
        gng->setFuncArray(constbeta,1);
        gng->setFuncArray(constgamma,2);
        gng->setFuncArray(constdelta,3);
        gng->setFuncArray(constepsilonw,4);
        gng->setFuncArray(constepsilonn,5);
        gng->setFuncArray(consttheta,6);
        gng->setFuncArray(consteta,7);
        gng->setFuncArray(constlambda,8);
    }
    else if (algorithm == _gng)
    {
        gng->setFuncArray(constgamma,3);
        gng->setFuncArray(functheta,7);
        gng->setFuncArray(funclambda,6);
        //gng->setStoppingCriterion (global_error);
        gng->setStoppingCriterion (epochs);
        gng->setMaxEpochs (100);
        gng->setSamplingMode(randomly);
        //(dynamic_cast<GNGAlgorithm<float,int>*>(gng))->setMinGlobalError (0.01);
    }
    else if (algorithm == _ebgng)
    {
    // ebgng->setFuncArray(constgamma,3);
    // ebgng->setFuncArray(functheta,7);
    // ebgng->setFuncArray(funclambda,8);
        gng->setFuncArray(constgamma,3);
        gng->setFuncArray(functheta,7);
        gng->setFuncArray(funclambda,8);
        //gng->setSamplingMode(randomly);
        gng->setStoppingCriterion (epochs);
        // gng->setMaxEpochs (1000000000);
    }
    else if (algorithm == _llbgng)
    {
	    (dynamic_cast<LLBGNGAlgorithm<float,int>*>(gng))->setTimeWindows (20, 100, 100);
	    (dynamic_cast<LLBGNGAlgorithm<float,int>*>(gng))->setLearningRates (0.1, 0.01, 0.1);
	    (dynamic_cast<LLBGNGAlgorithm<float,int>*>(gng))->setInsertionRate (10);
	    (dynamic_cast<LLBGNGAlgorithm<float,int>*>(gng))->setAdaptationThreshold (0.05);
	    (dynamic_cast<LLBGNGAlgorithm<float,int>*>(gng))->setInsertionTolerance (0.001);
	    (dynamic_cast<LLBGNGAlgorithm<float,int>*>(gng))->setDeletionThreshold (0.5);
	    (dynamic_cast<LLBGNGAlgorithm<float,int>*>(gng))->setMinimalNodeAge (0.001);
	    (dynamic_cast<LLBGNGAlgorithm<float,int>*>(gng))->setMaximalEdgeAge (50);
	    (dynamic_cast<LLBGNGAlgorithm<float,int>*>(gng))->setStabilization (1.001);

	    (dynamic_cast<LLBGNGAlgorithm<float,int>*>(gng))->setSamplingMode (randomly);
	    //gng->setStoppingCriterion (epochs);
	    //gng->setMaxEpochs (100);
	    //(dynamic_cast<LLBGNGAlgorithm<float,int>*>(gng))->setMaxNodes(5);
	    gng->setStoppingCriterion (stability);
    }
   
    //(dynamic_cast< CDNAlgorithm<float,int>* > (ng[1]))->setEnergy(0.1);
    //na->save("data.txt");
    mg->save("data.txt");
    ng.run();
    std::vector<float> errors;
    for (int i=0; i < ng.size(); i++)
    {
        std::cout << "Algorithm " << i << std::endl;
        //errors = ng.getErrors(i,500);
        errors = ng.getErrors(i,sizeofdata-1);
        float total_error=0.0;
        ng[i]->showGraph();
	ng[i]->save ("nodes.txt");
        for(unsigned int j=0; j < errors.size(); j++)
        {
         //       std::cout << errors [j] << " ";
                total_error+=errors[j];
        }
        std::cout << "Gesamtfehler "<< total_error / sizeofdata <<std::endl;
    
        std::cout << std::endl;
    }

    std::cout << std::endl;
    delete mg;
    //delete na;
    
    return EXIT_SUCCESS;
}
