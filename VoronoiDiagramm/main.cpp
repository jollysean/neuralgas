#include <QtCore/QCoreApplication>
#include "Voronoi.h"

using namespace neuralgas;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::string filename_data="data.txt";
    std::string filename_neurons="nodes.txt";
    std::string filename_voronoi="voronoi";
    std::string output="voronoi.png";

    Voronoi v;
    if (argc > 1)
	    if (std::string(argv[1]) == "-w")
		    v.setWhiteBackground ();
    if (!v.getData(filename_data.c_str()))
    {
	    std::cerr << "unable to read file data.txt" << std::endl;
	    return 1;
    }
    v.showData();
    //v.setSize(500,1000);
    if (!v.getNeurons(filename_neurons.c_str()))
    {
	    std::cerr << "unable to read file nodes.txt" << std::endl;
	    return 1;
    }
    v.getMaxMinValue ();
    v.calcVoronoiGnuplot();
    v.saveVoronoiGnuplot(filename_voronoi, filename_data, filename_neurons);
    v.setSizefromData(500);
    v.discretizeData ();
    v.discretizeNeurons ();
    v.calcVoronoiImage();
    v.saveVoronoiImage(output.c_str());
    std::cout <<"run done"<<std::endl;
    return 0;
}
