/** 
* \class LLBGNGGraph
* \author Sergio Roa
* \author Manuel Noll
* 
*  Copyright(c) 2010 Manuel Noll - All rights reserved
*  \version 1.0
*  \date    2011
*/

#ifndef LLBGNGGRAPH_H
#define LLBGNGGRAPH_H

#include <GrowingNeuralGas/GNGModulGraph.h>
#include <queue>
#include <tools/metrics.h>

namespace neuralgas {


/** \brief The derived node for a LLBGNGGraph
 *
 *  The node permits having a quality measure for learning when inserting and deleting nodes,
 *  calculated from a last mean error and a previous mean error in a similar way as in the paper
 *  "Life-long learning cell structures -- continuosly learning without catastrophic interference"
 *  by Fred H. Hamker,
 *  but using the average error variables proposed in
 * "Intrinsic Motivation Systems for Autonomous Mental Development"
 * by Oudeyer, Kaplan, Hafner.
 *
 */
template<typename T,typename S>
struct LLBGNGNode : Base_Node<T,S>
{
	// default \p LLBGNGNode cto
	LLBGNGNode ();
	// default \p LLBGNGNode dto
	~LLBGNGNode ();
	/// errors vector for calculating different parameters. Its size is
	/// set by the maximum allowable long term error window
	// std::vector<T> longterm_errors;
	std::vector<T> errors;
	/// errors vector for calculating different parameters. Its size is
	/// set by the maximum allowable short term error window
	// std::vector<T> shortterm_errors;
	// calculate \p learning_quality measure
	void calculateLearningQuality ();
	/// quality measure for learning
	T learning_quality;
	// calculate \p insertion_quality measure
	void calculateInsertionQuality (T&);
	/// quality measure for insertion
	T insertion_quality;
	// calculate \p insertion_criterion
	void calculateInsertionCriterion ();
	/// insertion criterion
	T insertion_criterion;
	// calculate \p shortterm_avgerror and \p longterm_avgerror
	// void updateAvgError (T, const T&, const T&);
	void updateAvgError (T, const unsigned int&, const unsigned int&, const unsigned int&);
	/// short-term error counter
	// T shortterm_avgerror;
	T prev_avgerror;
	/// long-term error counter
	// T longterm_avgerror;
	T last_avgerror;
	/// inherited error calculating during node insertion
	T inherited_error;
	/// An insertion is only allowed if the long-term error exceeds this threshold
	T insertion_threshold;
	// decrease node age
	void decreaseAge (unsigned int);
	/// age of the node
	T age;
	// update \p learning_rate
	void updateLearningRate (T&, T&);
	/// learning rate of the node
	T learning_rate;
	/// local similarity of weights
	T local_similarity;
	// update activations counter
	void increaseActivationsCounter ();
	/// counter for the nr. of activations in a training epoch
	unsigned int activations_counter;
};

/// \brief default \p LLBGNGNode cto
template<typename T, typename S>
LLBGNGNode<T,S>::LLBGNGNode () :
	insertion_threshold (0),
	age (1),
	learning_rate (0),
	local_similarity (0),
	activations_counter (0)
{
}

/// \brief default \p LLBGNGNode dto
template<typename T, typename S>
LLBGNGNode<T,S>::~LLBGNGNode ()
{
	// longterm_errors.clear ();
	// shortterm_errors.clear ();
	errors.clear ();
}


/// \brief calculate \p learning_quality measure
template<typename T, typename S>
void LLBGNGNode<T,S>::calculateLearningQuality ()
{
	// learning_quality = (shortterm_avgerror + 1) / (longterm_avgerror + 1);
	learning_quality = (last_avgerror + 1) / (prev_avgerror + 1);

}

/// \brief calculate \p insertion_quality measure
template<typename T, typename S>
void LLBGNGNode<T,S>::calculateInsertionQuality (T& insertion_tolerance)
{
	// insertion_quality = longterm_avgerror - insertion_threshold * (1 + insertion_tolerance);
	insertion_quality = last_avgerror - insertion_threshold * (1 + insertion_tolerance);


}

/// \brief calculate \p insertion_criterion
template<typename T, typename S>
void LLBGNGNode<T,S>::calculateInsertionCriterion ()
{
	insertion_criterion = insertion_quality /*- age*/;
}



/** \brief update \p learning_quality measure
 *  \param adaptation_threshold cut-off value of learning
 */
template<typename T, typename S>
void LLBGNGNode<T,S>::updateLearningRate (T& adaptation_threshold, T& default_rate)
{

	T rate_decision_boundary;

	rate_decision_boundary = learning_quality / (1 + adaptation_threshold) /*+ age -1*/;

	if (rate_decision_boundary < 0)
		learning_rate = 0;
	else if (rate_decision_boundary <= 1)
		learning_rate = rate_decision_boundary * default_rate;
	else
		learning_rate = default_rate;
}


/** \brief calculate \p shortterm_avgerror and \p longterm_avgerror
 *  \param last_error last calculated distance to some data item
 *  \param shortterm_window short term window constant
 *  \param longterm_window long term window constant
 */
/*template<typename T, typename S>
void LLBGNGNode<T,S>::updateAvgError (T last_error, const T& shortterm_window, const T& longterm_window)
{
	shortterm_errors.push_back (last_error);
	// T shortterm_front_error = shortterm_errors.front();
	longterm_errors.push_back (last_error);
	// T longterm_front_error = longterm_errors.front();
	if (longterm_errors.size() > longterm_window)
		// longterm_errors.pop();
		longterm_errors.erase (longterm_errors.begin());
	if (shortterm_errors.size() > shortterm_window)
		// shortterm_errors.pop();
		shortterm_errors.erase (shortterm_errors.begin());
	T shortterm_window_scaled = shortterm_window;
	T longterm_window_scaled = longterm_window;
	
	if (longterm_errors.size() < longterm_window)
	{
		T errorwindows_ratio = longterm_window / shortterm_window;
		// std::cout << "errowindows ratio: " << errorwindows_ratio << std::endl;
		shortterm_window_scaled = longterm_errors.size() / errorwindows_ratio;
		longterm_window_scaled = longterm_errors.size();
 		// std::cout << "shorterm window: " << shortterm_window_scaled << std::endl;
		// std::cout << "longterm window: " << longterm_window_scaled << std::endl;
	}
	assert (longterm_errors.size() == longterm_window_scaled);

	shortterm_avgerror = 0;
	for (unsigned int i=0; i<shortterm_errors.size(); i++)
		shortterm_avgerror += shortterm_errors[i];
	shortterm_avgerror /= shortterm_window_scaled;
	longterm_avgerror = 0;
	for (unsigned int i=0; i<longterm_errors.size(); i++)
		longterm_avgerror += longterm_errors[i];
	longterm_avgerror /= longterm_window_scaled;
	
	// std::cout << "errors size: " << errors.size() << std::endl;
	// std::cout << "shortterm update: " << shortterm_avgerror << std::endl;

	
}*/


/** \brief calculate \p last_avgerror and \p prev_avgerror
 *  \param last_error last calculated distance to some data item
 *  \param smoothing smoothing window constant
 *  \param timewindow error time window constant
 */
template<typename T, typename S>
void LLBGNGNode<T,S>::updateAvgError (T last_error, const unsigned int& smoothing, const unsigned int& timewindow, const unsigned int& max_errors_size)
{
	errors.push_back (last_error);
	if (errors.size() > max_errors_size)
		errors.erase (errors.begin());

	unsigned int errors_size = errors.size();
	
	T timewindow_ratio = timewindow / T(smoothing + timewindow);
	T smoothing_ratio = smoothing / T(smoothing + timewindow);

	unsigned int smoothing_last = smoothing + 1;
	unsigned int smoothing_prev = smoothing + 1;

	unsigned int windowbegin_prev_avgerror;
	unsigned int windowlast_prev_avgerror;
	unsigned int windowbegin_last_avgerror;
	if (errors.size () <= smoothing + timewindow) {
		windowbegin_prev_avgerror = 0;
	}
	else
		windowbegin_prev_avgerror = errors_size -1 - (timewindow + smoothing);

	if (errors.size () <= timewindow)
		windowlast_prev_avgerror = (unsigned int)ceil((errors_size - 1) * smoothing_ratio);
	else
		windowlast_prev_avgerror = errors_size - 1 - timewindow;

	
	if (errors.size () <= smoothing) {
		windowbegin_last_avgerror = (unsigned int)ceil((errors_size - 1) * timewindow_ratio);
		smoothing_prev = errors_size - windowbegin_last_avgerror;
		smoothing_last = errors_size - windowbegin_last_avgerror;
	}
	else
		windowbegin_last_avgerror = errors_size - 1 - smoothing;

	assert (windowbegin_prev_avgerror >= 0 && windowlast_prev_avgerror >= 0 && windowbegin_last_avgerror >= 0);
	prev_avgerror = 0.0;
	last_avgerror = 0.0;

	
	for (unsigned int i=windowbegin_last_avgerror; i < errors_size; i++) 
		last_avgerror += errors[i];
	for (unsigned int i=windowbegin_prev_avgerror; i<windowlast_prev_avgerror; i++)
		prev_avgerror += errors[i];	
	
	prev_avgerror /= smoothing_prev;
	last_avgerror /= smoothing_last;

	// learningProgressHistory.push_back (-(last_avgerror - prev_avgerror));

	
	// cout << "\tLearning progress: " << endl;
	// cout << "\t" << learningProgressHistory.back() << endl;

}
	
/** \brief decrease node age
 *  \param age_time_window time window parameter
 */
template<typename T, typename S>
void LLBGNGNode<T,S>::decreaseAge (unsigned int age_time_window)
{
	age = exp (-1/(T) age_time_window) * age;
}

/** \brief update \p activations_counter
 */
template<typename T, typename S>
void LLBGNGNode<T,S>::increaseActivationsCounter ()
{
	activations_counter++;
}

/** \brief LLBGNGGraph provides some additional learning strategies proposed in
 *   the algorithm Life-long learning cell structures -- continuosly learning
 *   without catastrophic interference by Fred H. Hamker.
 *   The algorithm tries to avoid bias/variance issues in the original GNG.
*/
template<typename T, typename S>
class LLBGNGGraph : public GNGModulGraph<T,S>
{
public:
	/// cto Graph creation (node and edges weights share dimensionality)
	LLBGNGGraph (const unsigned int&, const unsigned int&);
	/// std dto
	~LLBGNGGraph(){}
	// set time window constants
	void setTimeWindows (unsigned int, unsigned int, unsigned int);
	// calculate inherited variables for a node to be inserted between two nodes
	void calculateInheritedParams (const unsigned int, const unsigned int, const unsigned int);
	// calculate long term and short term error for some node
	void updateAvgError (const unsigned int, T last_error);
	// calculate learning quality for some node
	void calculateLearningQuality (const unsigned int);
	// calculate insertion quality for some node
	void calculateInsertionQuality (const unsigned int);
	// calculate insertion criterion for some node
	void calculateInsertionCriterion (const unsigned int);
	// update learning rate for some winner node
	void updateWinnerLearningRate (const unsigned int);
	// update learning rate for some winner-neighboring node
	void updateNeighborLearningRate (const unsigned int);
	// set input adaptation threshold
	void setAdaptationThreshold (T&);
	// set insertion tolerance
	void setInsertionTolerance (T&);
	// set initial learning rate constants
	void setLearningRates (T&, T&, T&);
	// set deletion threshold
	void setDeletionThreshold (T&);
	// get deletion_threshold
	T getDeletionThreshold () const;
	// set minimal node age constant
	void setMinimalNodeAge (T&);
	// set minimal node age constant
	T getMinimalNodeAge () const;
	// set maximal edge age constant
	void setMaximalEdgeAge (unsigned int&);
	// get maximal edge age constant
	unsigned int getMaximalEdgeAge () const;
	// set stabilization constant
	void setStabilization (T&);
	// get stabilization constant
	T getStabilization () const;
	// decrease age of some node
	void decreaseNodeAge (const unsigned int);
	// update activations counter for some node
	void increaseActivationsCounter (const unsigned int);
private:
	// returns a pointer to a edge of a type that is currently used by the graph
	virtual LLBGNGNode<T,S>* newNode(void);	
	/// adaptation threshold constant
	T adaptation_threshold;
	/// insertion tolerance constant
	T insertion_tolerance;
	/// initial winner learning rate constant
	T winner_learning_rate;
	/// initial winner-neighbors learning rate constant
	T neighbors_learning_rate;
	/// initial learning rate constant of the insertion threshold
	T insertion_learning_rate;
	/// minimal node age constant
	T minimal_node_age;
	/// maximal edge age constant
	unsigned int maximal_edge_age;
	/// stabilization constant
	T stabilization;
	/// deletion threshold
	T deletion_threshold;
	/// short term window constant
	// unsigned int shortterm_window;
	unsigned int smoothing_window;
	/// long term window constant
	// unsigned int longterm_window;
	unsigned int error_time_window;
	/// age time window constant
	unsigned int age_time_window;
	/// maximal size of error vector
	const unsigned int max_errors_size;
};

/** \brief cto Graph creation (node and edges weights share dimensionality)
    \param dim dimensionality
 */
template<typename T, typename S>
LLBGNGGraph<T,S>::LLBGNGGraph (const unsigned int &dim, const unsigned int& max_error_window) :
	Base_Graph<T,S>(dim),
	UGraph<T,S>(dim),
	TGraph<T,S>(dim),
	GNGModulGraph<T,S>(dim),
	adaptation_threshold (0),
	insertion_tolerance (0),
	winner_learning_rate (0),
	neighbors_learning_rate (0),
	insertion_learning_rate (0),
	minimal_node_age (0),
	maximal_edge_age (0),
	stabilization (0),
	deletion_threshold (0),
	// shortterm_window (1),
	// longterm_window (1),
	smoothing_window (1),
	error_time_window (1),
	age_time_window (1),
	max_errors_size (max_error_window)
{
}


/** \brief overriden function from \p Base_Graph
    to create a node of type \p LLBGNGNode */
template<typename T,typename S>
LLBGNGNode<T,S>* LLBGNGGraph<T,S>::newNode(void)
{
	LLBGNGNode<T,S>* n = new LLBGNGNode<T,S>;
	if (this->high_limits.size() != 0 && this->low_limits.size() != 0)
		n->inherited_error = euclidean<T,S> (this->getHighLimits(), this->getLowLimits());
	else
		n->inherited_error = this->high_limit - this->low_limit;		
	//default inherited errors (used at the beginning of the algorithm)
	// n->shortterm_avgerror = n->inherited_error;
	// n->longterm_avgerror = n->inherited_error;
	// n->longterm_errors.push_back (n->inherited_error);
	// n->shortterm_errors.push_back (n->inherited_error);
	n->prev_avgerror = n->inherited_error;
	n->last_avgerror = n->inherited_error;
	n->errors.push_back (n->inherited_error);
	return n; 
}

/** \brief set window constants. This function should only
    be called once when creating the graph
    \param shortterm short term time window constant
    \param longterm long term time window constant
    \param age age time window constant */
/*template<typename T, typename S>
void LLBGNGGraph<T,S>::setTimeWindows (unsigned int shortterm, unsigned int longterm, unsigned int age)
{
	assert (shortterm <= longterm);
	shortterm_window = shortterm;
	longterm_window = longterm;
	age_time_window = age;
}*/

/** \brief set window constants. This function should only
    be called once when creating the graph
    \param smoothing smoothing time window constant
    \param longterm error time window constant
    \param age age time window constant */
template<typename T, typename S>
void LLBGNGGraph<T,S>::setTimeWindows (unsigned int smoothing, unsigned int error, unsigned int age)
{
	assert (max_errors_size > error + smoothing);

	smoothing_window = smoothing;
	error_time_window = error;
	age_time_window = age;
}

/** \brief calculate inherited variables for a node to be inserted between two nodes
 *  \param index node index
 *  \param first_index 1st node index
 *  \param snd_index 2nd node index
 */
/*template<typename T, typename S>
void LLBGNGGraph<T,S>::calculateInheritedParams (const unsigned int index, const unsigned int first_index, const unsigned int snd_index)
{
	LLBGNGNode<T,S>* node = static_cast<LLBGNGNode<T,S>* >(this->_nodes[index]);
	LLBGNGNode<T,S>* first_node = static_cast<LLBGNGNode<T,S>* >(this->_nodes[first_index]);
	LLBGNGNode<T,S>* snd_node = static_cast<LLBGNGNode<T,S>* >(this->_nodes[snd_index]);
	
	node->weight = (first_node->weight + snd_node->weight) / (T)2;
	node->longterm_avgerror = (first_node->longterm_avgerror + snd_node->longterm_avgerror) / (T)2;
	node->shortterm_avgerror = (first_node->shortterm_avgerror + snd_node->shortterm_avgerror) / (T)2;
	node->inherited_error = (first_node->inherited_error + snd_node->inherited_error) / (T)2;
	node->insertion_threshold = (first_node->insertion_threshold + snd_node->insertion_threshold) / (T)2;

	//check if insertion is successful
	if (first_node->longterm_avgerror >= first_node->inherited_error * (1 - insertion_tolerance)) {
		first_node->insertion_threshold += insertion_learning_rate * (first_node->longterm_avgerror - first_node->insertion_threshold * (1 - insertion_tolerance));
   		// std::cout << "1st i.t.: " << first_node->insertion_threshold << std::endl;
	}
	
	if (snd_node->longterm_avgerror >= snd_node->inherited_error * (1 - insertion_tolerance)) {
		snd_node->insertion_threshold += insertion_learning_rate * (snd_node->longterm_avgerror - snd_node->insertion_threshold * (1 - insertion_tolerance));
		// std::cout << "2nd i.t.: " << snd_node->insertion_threshold << std::endl;
	}
	if(node->longterm_avgerror >= node->inherited_error * (1 - insertion_tolerance)) {
		node->insertion_threshold += insertion_learning_rate * (node->longterm_avgerror - node->insertion_threshold * (1 - insertion_tolerance));
		// std::cout << "node i.t.: " << node->insertion_threshold << std::endl;		
	}
	first_node->inherited_error = first_node->longterm_avgerror;
	snd_node->inherited_error = snd_node->longterm_avgerror;
	node->inherited_error = node->longterm_avgerror;
	// std::cout << "1st i.e.: " << first_node->inherited_error << std::endl;
	// std::cout << "2nd i.e.: " << snd_node->inherited_error << std::endl;
	// std::cout << "node i.e.: " << node->inherited_error << std::endl;
	
	assert (node->longterm_errors.size() == 1);
	assert (node->shortterm_errors.size() == 1);
	node->longterm_errors.front() = node->longterm_avgerror;
	node->shortterm_errors.front() = node->shortterm_avgerror;

	
}*/


/** \brief calculate inherited variables for a node to be inserted between two nodes
 *  \param index node index
 *  \param first_index 1st node index
 *  \param snd_index 2nd node index
 */
template<typename T, typename S>
void LLBGNGGraph<T,S>::calculateInheritedParams (const unsigned int index, const unsigned int first_index, const unsigned int snd_index)
{
	LLBGNGNode<T,S>* node = static_cast<LLBGNGNode<T,S>* >(this->_nodes[index]);
	LLBGNGNode<T,S>* first_node = static_cast<LLBGNGNode<T,S>* >(this->_nodes[first_index]);
	LLBGNGNode<T,S>* snd_node = static_cast<LLBGNGNode<T,S>* >(this->_nodes[snd_index]);
	
	node->weight = (first_node->weight + snd_node->weight) / (T)2;
	node->prev_avgerror = (first_node->prev_avgerror + snd_node->prev_avgerror) / (T)2;
	node->last_avgerror = (first_node->last_avgerror + snd_node->last_avgerror) / (T)2;
	node->inherited_error = (first_node->inherited_error + snd_node->inherited_error) / (T)2;
	node->insertion_threshold = (first_node->insertion_threshold + snd_node->insertion_threshold) / (T)2;

	//check if insertion is successful
	if (first_node->last_avgerror >= first_node->inherited_error * (1 - insertion_tolerance)) {
		first_node->insertion_threshold += insertion_learning_rate * (first_node->last_avgerror - first_node->insertion_threshold * (1 - insertion_tolerance));
   		// std::cout << "1st i.t.: " << first_node->insertion_threshold << std::endl;
	}
	
	if (snd_node->last_avgerror >= snd_node->inherited_error * (1 - insertion_tolerance)) {
		snd_node->insertion_threshold += insertion_learning_rate * (snd_node->last_avgerror - snd_node->insertion_threshold * (1 - insertion_tolerance));
		// std::cout << "2nd i.t.: " << snd_node->insertion_threshold << std::endl;
	}
	if(node->last_avgerror >= node->inherited_error * (1 - insertion_tolerance)) {
		node->insertion_threshold += insertion_learning_rate * (node->last_avgerror - node->insertion_threshold * (1 - insertion_tolerance));
		// std::cout << "node i.t.: " << node->insertion_threshold << std::endl;		
	}
	first_node->inherited_error = first_node->last_avgerror;
	snd_node->inherited_error = snd_node->last_avgerror;
	node->inherited_error = node->last_avgerror;
	// std::cout << "1st i.e.: " << first_node->inherited_error << std::endl;
	// std::cout << "2nd i.e.: " << snd_node->inherited_error << std::endl;
	// std::cout << "node i.e.: " << node->inherited_error << std::endl;
	
	assert (node->errors.size() == 1);
	node->errors.front() = node->last_avgerror;

	
}



/** \brief calculate long term and short term error for a given node
    \param index node index */
/*template<typename T, typename S>
void LLBGNGGraph<T,S>::updateAvgError (const unsigned int index, T last_error)
{
	(static_cast<LLBGNGNode<T,S>* > (this->_nodes[index]))->updateAvgError(last_error, shortterm_window, longterm_window);

}*/
/** \brief calculate last and previous mean error for a given node
    \param index node index */
template<typename T, typename S>
void LLBGNGGraph<T,S>::updateAvgError (const unsigned int index, T last_error)
{
	static_cast<LLBGNGNode<T,S>* > (this->_nodes[index])->updateAvgError(last_error, smoothing_window, error_time_window, max_errors_size);

}

/** \brief calculate learning quality for a node
    \param index node index */
template<typename T, typename S>
void LLBGNGGraph<T,S>::calculateLearningQuality (const unsigned int index)
{
	static_cast<LLBGNGNode<T,S>* > (this->_nodes[index])->calculateLearningQuality();
	
}

/** \brief calculate insertion quality for a node
    \param index node index */
template<typename T, typename S>
void LLBGNGGraph<T,S>::calculateInsertionQuality (const unsigned int index)
{
	static_cast<LLBGNGNode<T,S>* > (this->_nodes[index])->calculateInsertionQuality(insertion_tolerance);
	
}

/** \brief calculate insertion criterion for a node
    \param index node index */
template<typename T, typename S>
void LLBGNGGraph<T,S>::calculateInsertionCriterion (const unsigned int index)
{
	static_cast<LLBGNGNode<T,S>* > (this->_nodes[index])->calculateInsertionCriterion();
	
}

/** \brief set initial learning rate constants
 *  \param winner initial winner learning rate constant
 *  \param neighbors initial winner-neighbors learning rate constant
 *  \param insertion_threshold learning rate constant
 */
template<typename T, typename S>
void LLBGNGGraph<T,S>::setLearningRates (T& winner, T& neighbors, T& insertion_threshold)
{
	winner_learning_rate = winner;
	neighbors_learning_rate = neighbors;
	insertion_learning_rate = insertion_threshold;
}


/** \brief update learning rate for a winner node
 *  \param index node index
 */
template<typename T, typename S>
void LLBGNGGraph<T,S>::updateWinnerLearningRate (const unsigned int index)
{
	static_cast<LLBGNGNode<T,S>* > (this->_nodes[index])->updateLearningRate(adaptation_threshold, winner_learning_rate);	
}

/** \brief update learning rate for a winner-neighboring node
 *  \param index node index
 */
template<typename T, typename S>
void LLBGNGGraph<T,S>::updateNeighborLearningRate (const unsigned int index)
{
	static_cast<LLBGNGNode<T,S>* > (this->_nodes[index])->updateLearningRate(adaptation_threshold, neighbors_learning_rate);	
}


/** \brief set adaptation threshold
    \param threshold given adaptation threshold */
template<typename T, typename S>
void LLBGNGGraph<T,S>::setAdaptationThreshold (T& threshold)
{
	adaptation_threshold = threshold;	
}

/** \brief set insertion tolerance
    \param tolerance insertion tolerances */
template<typename T, typename S>
void LLBGNGGraph<T,S>::setInsertionTolerance (T& tolerance)
{
	insertion_tolerance = tolerance;
}

/** \brief set \p deletion_threshold
    \param threshold given deletion threshold */
template<typename T, typename S>
void LLBGNGGraph<T,S>::setDeletionThreshold (T& threshold)
{
	deletion_threshold = threshold;
}

/** \brief get \p deletion_threshold
 */
template<typename T, typename S>
T LLBGNGGraph<T,S>::getDeletionThreshold () const
{
	return deletion_threshold;
}


/* \brief set \p minimal_node_age constant
 * \param age minimal node age constant
 */
template<typename T, typename S>
void LLBGNGGraph<T,S>::setMinimalNodeAge (T& age)
{
	minimal_node_age = age;
}

/* \brief get \p minimal_node_age constant
 */
template<typename T, typename S>
T LLBGNGGraph<T,S>::getMinimalNodeAge () const
{
	return minimal_node_age;
}

/* \brief set maximal edge age constant
 * \param age maximal edge age constant
 */
template<typename T, typename S>
void LLBGNGGraph<T,S>::setMaximalEdgeAge (unsigned int& age)
{
	maximal_edge_age = age;
}

/* \brief get \p maximal_edge_age constant
 */
template<typename T, typename S>
unsigned int LLBGNGGraph<T,S>::getMaximalEdgeAge () const
{
	return maximal_edge_age;
}


/* set \p stabilization constant
 * \param s stabilization constant
 */
template<typename T, typename S>
void LLBGNGGraph<T,S>::setStabilization (T& s)
{
	stabilization = s;
}

/* get \p stabilization constant
 */
template<typename T, typename S>
T LLBGNGGraph<T,S>::getStabilization () const
{
	return stabilization;
}

/* \brief decrease age of a node
 */
template<typename T, typename S>
void LLBGNGGraph<T,S>::decreaseNodeAge (const unsigned int index)
{
	static_cast<LLBGNGNode<T,S>* > (this->_nodes[index])->decreaseAge (age_time_window);
}

/** \brief update activations counter for some node
 */
template<typename T, typename S>
void LLBGNGGraph<T,S>::increaseActivationsCounter (const unsigned int index)
{
	static_cast<LLBGNGNode<T,S>* > (this->_nodes[index])->increaseActivationsCounter ();
}


} // namespace neuralgas

#endif
