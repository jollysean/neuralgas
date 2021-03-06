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

/** 
* \file CDNAlgorithm.h
* \author Manuel Noll
* 
*  Copyright(c) 2010 Manuel Noll - All rights reserved
*  \version 1.0
*  \date    2010
*/

#ifndef CDNALGORITHM_H
#define CDNALGORITHM_H

#include "MGNGAlgorithm.h"

namespace neuralgas {

/** \class CDNAlgorithm
 *  \brief Cool down GNG Algorithm (still uncomplete and buggy)
 */
template<typename T,typename S> class CDNAlgorithm : public MGNGAlgorithm<T,S>
{
public:
	// cto initializing the class 
	CDNAlgorithm(const unsigned int& dim) : MGNGAlgorithm<T,S>(dim){}
	void setEnergy(const float& energy) {_energy = energy;}
protected:
	virtual void updateNeighbor(const unsigned int&,const unsigned int&);
	virtual void updateWinner(const unsigned int&,const unsigned int&);
	float _energy;

};


/** \brief Defines the update rule for the in the second index given neighbor 
*
*   The update rule depends on the actual datum. With that datum a given
*   topological neighbor is updated by an algorithmic dependent rule.
*   w_j(new) = w_j(old) + epsilon_n * ( x_t - w_j(old) )
*   c_j(new) = c_j(old) + epsilon_n * (C - c_j(old) )
*
*   \param time is the data vector that is used for updating 
*   \param node_index is the index of topological neighbor that shall be updated
*/
template<typename T,typename S> void CDNAlgorithm<T,S>::updateNeighbor(const unsigned int& time,const unsigned int& index)
{
 float distance = pow(this->_graphptr->getDistance((*this)[time],index),2);
 float rate;

 if (distance > 0)
    rate = this->_graphptr->getTemp(index) * exp(- (time - this->_graphptr->getBirthday(index)  ) ) + _energy * distance;
 else
    rate = this->_graphptr->getTemp(index) * exp(- (time - this->_graphptr->getBirthday(index)  ) ) + _energy;


 this->_graphptr->setTemp(index,rate);
 this->_graphptr->setBirthday(index,time);


 (*(this->_graphptr))[index].weight  +=  rate *( (*this)[time]-(*(this->_graphptr))[index].weight);
 this->_graphptr->context(index) += rate * ( this->globalContextV - this->_graphptr->context(index));            
}

/** \brief defines the update rule for the winner
*
*   The update rule depends on the actual datum. With that datum the given
*   winner is updated by an algorithmic dependent rule.
*    w(new) = w(old) + epsilon_b * (x_t - w(old) )
*
*   \param time is the data vector that is used for updating 
*   \param winner is the index of the winner that shall be updated
*/
template<typename T,typename S> void CDNAlgorithm<T,S>::updateWinner(const unsigned int& time,const unsigned int& winner)
{
                  
float rate = this->_graphptr->getTemp(winner) * exp(- float( time - this->_graphptr->getBirthday(winner) ) )+ _energy;

this->_graphptr->setTemp(winner,rate);
this->_graphptr->setBirthday(winner,time);

(*(this->_graphptr))[winner].weight  +=  rate *( (*this)[time]-(*(this->_graphptr))[winner].weight);
this->_graphptr->context(winner) += rate * ( this->globalContextV - this->_graphptr->context(winner));            
}

} // namespace neuralgas

#endif
