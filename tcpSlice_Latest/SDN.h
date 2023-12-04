//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __TCPEXPERIMENTS_SDN_H_
#define __TCPEXPERIMENTS_SDN_H_

#include <omnetpp.h>
#include<vector>
#include<fstream>
using namespace omnetpp;
using namespace std;

/**
 * TODO - Generated class
 */
class SDN : public cSimpleModule
{
private:
    int numhosts,packetSize;
    double stepsize, delaybound, burstparam;
    vector<double> sourcerates,mincaps,sourceprices;
    cMessage *updateSDNprice;
    cDatarateChannel *link1,*link2;
    fstream delay_bounds;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void finish() override;
  public:
    ~SDN()
    {
        cancelAndDelete(updateSDNprice);
    }
};

#endif
