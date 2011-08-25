#include "Voronoi.h"

namespace neuralgas {

void Voronoi::addData(const std::string& line)
{
    int i=0;
    while(line[i]!=' ')
    {
        i++;
    }
    Vector<double>* point = new Vector<double>(2);
    point->at(0) = atof( (line.substr(0,i-1)).c_str());
    point->at(1) = atof( (line.substr(i+1, line.size()-i - 2 )).c_str() );
    _data->push_back(point);

}

void Voronoi::addNeuron(const std::string& line)
{
    int i=0;
    while(line[i]!=' ')
    {
        i++;
    }
    Base_Node<double, int>* neuron = new Base_Node<double, int>;
    neuron->weight.resize (2);
    neuron->weight[0] = atof( (line.substr(0,i-1)).c_str() );
    neuron->weight[1] = atof( (line.substr(i+1, line.size()-i - 2 )).c_str() );
    _neurons->push_back(neuron);
}

void Voronoi::getData(const char* filename)
{
    _data = new std::vector < Vector<double>* >;
    std::string line;
    std::ifstream myfile (filename);
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {
          getline (myfile,line);
          addData(line);
        }
        myfile.close();
        getMaxMinValue();
    }
}

void Voronoi::showData()
{
    for(unsigned int i=0; i < _data->size(); i++)
        std::cout << (*(*_data)[i])[0] << " " << (*(*_data)[i])[1] << std::endl;
}

void Voronoi::getMaxMinValue()
{
    _maxX=(*(*_data)[0])[0];
    _minX=(*(*_data)[0])[0];
    _maxY=(*(*_data)[0])[1];
    _minY=(*(*_data)[0])[1];
    
    for (unsigned int i=1; i < _data->size(); i++)
    {

        if ((*(*_data)[i])[0] > _maxX)
            _maxX = (*(*_data)[i])[0];
        if ((*(*_data)[i])[0] < _minX)
            _minX = (*(*_data)[i])[0];
        if ((*(*_data)[i])[1] > _maxY)
            _maxY = (*(*_data)[i])[1];
        if ((*(*_data)[i])[1] < _minY)
            _minY = (*(*_data)[i])[1];

	
    }
}

void Voronoi::getNeurons(const char* filename)
{
    _neurons = new std::vector < Base_Node<double, int>* >;
    std::string line;
    std::ifstream myfile (filename);
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {
          getline (myfile,line);
          addNeuron(line);
        }
        myfile.close();
    }

}


void Voronoi::discretize()
{
    double rangeX = _maxX - _minX;
    double rangeY = _maxY - _minY;
 

    double factorX = double (_width) / rangeX;
    double factorY = double (_height) / rangeY;
    double minX = factorX * _minX;
    double minY = factorY * _minY;
    std::cout << "minX "<<_minX <<" rescaled minX "<<minX<<" minY "<<_minY<<" rescaled minY "<<minY<<std::endl;

    for(unsigned int i=0; i < _data->size(); i++)
    {
        (*(*_data)[i])[0]=( (*(*_data)[i])[0] * factorX - minX );
	(*(*_data)[i])[1]=( (*(*_data)[i])[1] * factorY - minY);

    }

    for(unsigned int i=0; i < _neurons->size(); i++)
    {
        (*_neurons)[i]->weight[0] =( (*_neurons)[i]->weight[0] * factorX - minX);
        (*_neurons)[i]->weight[1] =( (*_neurons)[i]->weight[1] * factorY - minY);
    }
}

void Voronoi::setSize(const int& height, const int& width)
{
    _height = height;
    _width = width;
}

void Voronoi::setSizefromData(const int& somesideSize)
{
    double rangeX = _maxX - _minX;
    double rangeY = _maxY - _minY;

    if (rangeX > rangeY) {
	    _width = somesideSize;
	    double factorX = double (_width) / rangeX;
	    _height = ceil(factorX * rangeY);
    }
    else {
	    _height = somesideSize;
	    double factorY = double (_height) / rangeY;
	    _width = ceil(factorY * rangeX);
    }

    std::cout << "width: " << _width << std::endl;
    std::cout << "height: " << _height << std::endl;


}

void Voronoi::setNeurons()
{
    _xValues = new double[_neurons->size()];
    _yValues = new double[_neurons->size()];

    for(unsigned int i=0; i < _neurons->size(); i++)
    {
        _xValues[i] = (*_neurons)[i]->weight[0];
        _yValues[i] = (*_neurons)[i]->weight[1];
    }
}

void Voronoi::calcVoronoi()
{
    discretize();
    setNeurons();
    _vdg.generateVoronoi(_xValues,_yValues,_neurons->size(), 0,_width,0,_height,0);
    _vdg.resetIterator();
}

void Voronoi::save(const char* filename)
{
    QImage image(_width, _height, QImage::Format_RGB32);
    QPainter painter(&image);
    
    QRgb value;
    // value = qRgb(255, 255, 255);
    //image.fill(value);
    //image.fill(0);
    image.fill (Qt::transparent);

    // data
    value = qRgb(255, 255, 255); // 0xff7aa327
    //value = qRgb(0, 0, 0);
    painter.setPen (value);
    painter.setBrush (QBrush(value));
    //value = qRgb(0, 0, 255);
    for(unsigned int i=0; i < _data->size();i++)
	painter.drawEllipse ((*(*_data)[i])[0], (*(*_data)[i])[1], 2, 2);

    // neurons
    value = qRgb(255, 0, 0); // 0xffbd9527
    painter.setPen (value);
    painter.setBrush (QBrush(value));
    for(unsigned int i=0; i < _neurons->size();i++)
	painter.drawEllipse ((*_neurons)[i]->weight[0], (*_neurons)[i]->weight[1], 5, 5);

    // voronoi lines
    //value = qRgb(237, 187, 51); // 0xffedba31
    value = qRgb(0, 0, 255); // 0xffedba31
    painter.setPen (value);

    double x1,y1,x2,y2;
    while(_vdg.getNext(x1,y1,x2,y2))
	painter.drawLine (x1, y1, x2, y2);

    image.save(filename, "JPG");
}

} // namespace neuralgas
