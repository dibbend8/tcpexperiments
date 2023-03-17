//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without couten the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "SDN.h"
#include"sdnmsg_m.h"
Define_Module(SDN);

void SDN::initialize()
{
    // TODO - Generated method body
    numhosts = getAncestorPar("numhosts").intValue();
    cout<<"Number of hosts"<<numhosts<<endl;
    int packetSize = getAncestorPar("packetSize").intValue();
    int numburstpacket = 20;
    burstparam = numburstpacket*packetSize*8;
    stepsize = 1e-6;
    delaybound = 10e-3;
    updateSDNprice = new cMessage();
    cDatarateChannel *link1 = (cDatarateChannel *) getModuleByPath("^.router[0]")->gate("toserver$o",0)->getChannel();
    cDatarateChannel *link2 = (cDatarateChannel *) getModuleByPath("^.router[1]")->gate("toserver$o",0)->getChannel();
    for(int i=0;i<numhosts;i++)
    {
        sourcerates.push_back(0);
        sourceprices.push_back(0);
        if(i>1)
        {
            mincaps.push_back(link2->getDatarate());
        }
        else
        {
            mincaps.push_back(link1->getDatarate());
        }
        cout<<"Minimum Capacity for Host "<<i<<" = "<<mincaps[i]<<endl;
    }
}

void SDN::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(strcmp(msg->getName(),"rates")==0)
    {
        Sdm *sdn = (Sdm *)msg;
        cout<<"**************************** Received rates from  Router "<<sdn->getSenderModule()->getIndex()<<endl;
        if(sdn->getSenderModule()->getIndex()==0)
        {
            sourcerates[0] = sdn->getRates(0);
            sourcerates[1] = sdn->getRates(1);
            cancelEvent(updateSDNprice);
            scheduleAt(simTime(), updateSDNprice);
        }
        else
        {
            sourcerates[2] = sdn->getRates(2);
            cancelEvent(updateSDNprice);
            scheduleAt(simTime(), updateSDNprice);
        }

        delete msg;
    }
    else if(msg==updateSDNprice)
    {
        Sdm *sdn = new Sdm("sdnprices");
        sdn->setRatesArraySize(numhosts);
        for(int i=0;i<numhosts;i++)
        {
            cout<<"SDN.............. Source rate for host "<<i<<" = "<<sourcerates[i]<<endl;
            double sumrate=0;
            for(int j=0;j<numhosts;j++)
            {
                if(j!=i)
                {
                    sumrate+=sourcerates[j];
                }
            }
            cout<<"SDN......................Minimum Capacity for host  "<<i<<" = "<<mincaps[i]<<endl;
            cout<<"SDN......................BoundCost = "<<(numhosts*burstparam)/(delaybound)<<endl;
            cout<<"SDN......................Sum of others rates = "<<sumrate<<endl;
            cout<<"SDN......................Previous Price = "<<sourceprices[i]<<endl;

            cout<<"SDN...................... Computed Price = "<<sourceprices[i]-stepsize*(mincaps[i] - (numhosts*burstparam)/(delaybound) - sumrate)<<endl;
            sourceprices[i] = fmax(sourceprices[i]-stepsize*(mincaps[i] - (numhosts*burstparam)/(delaybound) - sumrate),0);
            cout<<"SDN................ Price at SDN for host "<<i<< " = "<<sourceprices[i]<<endl;
            sdn->setRates(i,sourceprices[i]);
        }
        send(sdn->dup(),"torouter$o",0);
        send(sdn->dup(),"torouter$o",1);
        delete sdn;

    }

}
