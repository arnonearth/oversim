/*
 * DenaCastNoChurn.h
 *
 *  Created on: Dec 31, 2010
 *      Author: root
 */

#ifndef DENACASTNOCHURN_H_
#define DENACASTNOCHURN_H_

#include <ChurnGenerator.h>

class GlobalStatistics;
class TransportAddress;

/**
 * No churn generating class (only bootstraps a networks)
 */

class DenaCastNoChurn: public ChurnGenerator
{
public:
    void handleMessage(cMessage* msg);
    void initializeChurn();
    ~DenaCastNoChurn();

protected:
    void updateDisplayString();
    void fillArrivalVector();

private:
    double initialMean; //!< mean of update interval during initalization phase
    double initialDeviation; //!< deviation of update interval during initalization phase
    bool initAddMoreTerminals; //!< true, if we're still adding more terminals in the init phase
    cMessage* mobilityTimer; /**< message to schedule events */
    std::vector <double> ArrivalVector;
    double simtimelimit;

};
#endif /* DENACASTNOCHURN_H_ */
