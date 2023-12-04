//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without EVen the implied warranty of
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
    EV<<"Number of hosts"<<numhosts<<endl;
    packetSize = getAncestorPar("packetSize").intValue();
    int numburstpacket = 1;
    burstparam = numburstpacket*packetSize*8;
    stepsize = 1e-7;
    delaybound = 4e-3;
    remove("delaybounds.csv");
    delay_bounds.open("delaybounds.csv",ios::out|ios::app);
    updateSDNprice = new cMessage();
    link1 = (cDatarateChannel *) getModuleByPath("^.router[0]")->gate("toserver$o",0)->getChannel();
    link2 = (cDatarateChannel *) getModuleByPath("^.router[1]")->gate("toserver$o",0)->getChannel();
    for(int i=0;i<numhosts;i++)
    {
        sourcerates.push_back(0);
        sourceprices.push_back(0);//
        if(i>1)
        {
            mincaps.push_back(link2->getDatarate());
        }
        else
        {
            mincaps.push_back(link1->getDatarate());
        }
        EV<<"Minimum Capacity for Host "<<i<<" = "<<mincaps[i]<<endl;
    }
}

void SDN::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(strcmp(msg->getName(),"rates")==0)
    {
        Sdm *sdn = (Sdm *)msg;
        EV<<"**************************** Received rates from  Router "<<sdn->getSenderModule()->getIndex()<<endl;
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
            EV<<"SDN.............. Source rate for host "<<i<<" = "<<sourcerates[i]<<endl;
            double sumrate=0;
            for(int j=0;j<numhosts;j++)
            {
                if(j!=i)
                {
                    sumrate+=sourcerates[j];
                }
            }

            //            double minRate=0,delayBoundComp=0,boundCost=0;
            //            minRate = mincaps[i] - sumrate;
            //            if(i!=2)
            //            {
            //                delayBoundComp = (2*(numhosts-1)*(packetSize*8))/minRate;
            //                boundCost = (2*(numhosts-1)*(packetSize*8))/delaybound;
            //            }
            //            else
            //            {
            //                delayBoundComp = (1*(numhosts-1)*(packetSize*8) + burstparam)/minRate;
            //                boundCost = (1*(numhosts-1)*(packetSize*8) + burstparam)/delaybound;
            //            }
            //            double sumrate;
            double minRate=0;
            double delayBoundComp=0,boundCost=0;
            if(i==0 && simTime()<=100)
            {

                minRate = fmax(0,fmin( link1->getDatarate() -  sourcerates[1], link2->getDatarate()- (sourcerates[1]+sourcerates[2])));

                if(sourcerates[1]==0 && sourcerates[2]==0)
                {
                    delayBoundComp = (2*packetSize*8)/1e9 + (2*(packetSize*8))/minRate ;
                    boundCost = (2*(packetSize*8))/(delaybound - (2*packetSize*8)/1e9);
                }
                else if((sourcerates[2]==0 && sourcerates[1]!=0 ) || (sourcerates[1]==0 && sourcerates[2]!=0))
                {
                    delayBoundComp = (2*packetSize*8)/1e9 + (2*(packetSize*8))/minRate + 1*burstparam/minRate ;
                    boundCost = (1*burstparam+2*(packetSize*8))/(delaybound - (2*packetSize*8)/1e9);
                }
                else
                {
                    delayBoundComp = (2*packetSize*8)/1e9 + (2*(packetSize*8))/minRate + 2*burstparam/minRate ;
                    boundCost = (2*burstparam+2*(packetSize*8))/(delaybound - (2*packetSize*8)/1e9);
                }
                delay_bounds<<i<<","<<simTime().dbl()<<","<<delayBoundComp<<",";
            }
            if(i==0 && simTime()>100)
            {
                sourceprices[i]=0;
                delay_bounds<<i<<","<<simTime().dbl()<<","<<delayBoundComp<<",";
            }
            else if (i==1 && simTime()>=10 && simTime()<=120)
            {
                minRate = fmax(0,fmin(link1->getDatarate() - sourcerates[0],link1->getDatarate() - (sourcerates[0]+sourcerates[2])));
                if(sourcerates[0]==0 && sourcerates[2]==0)
                {
                    delayBoundComp = (2*packetSize*8)/1e9 + (2*(packetSize*8))/minRate ;
                    boundCost = (2*(packetSize*8))/(delaybound - (2*packetSize*8)/1e9);
                }
                else if((sourcerates[2]==0 && sourcerates[0]!=0 ) || (sourcerates[0]==0 && sourcerates[2]!=0))
                {
                    delayBoundComp = (2*packetSize*8)/1e9 + (2*(packetSize*8))/minRate + 1*burstparam/minRate ;
                    boundCost = (1*burstparam+2*(packetSize*8))/(delaybound - (2*packetSize*8)/1e9);
                }
                else
                {
                    delayBoundComp = (2*packetSize*8)/1e9 + (2*(packetSize*8))/minRate + 2*burstparam/minRate ;
                    boundCost = (2*burstparam+2*(packetSize*8))/(delaybound - (2*packetSize*8)/1e9);
                }
                delay_bounds<<i<<","<<simTime().dbl()<<","<<delayBoundComp<<",";
            }
            if(i==1 && simTime()>120)
            {
                sourceprices[i]=0;
                delay_bounds<<i<<","<<simTime().dbl()<<","<<delayBoundComp<<",";
            }
            else if(i==2 && simTime()>=40 && simTime()<=140)
            {
                minRate = fmax(0,link2->getDatarate()-(sourcerates[0]+sourcerates[1]));
                if(sourcerates[0]==0 && sourcerates[1]==0)
                {
                    delayBoundComp = (1*packetSize*8)/1e9 + (1*(packetSize*8))/minRate ;
                    boundCost = (1*(packetSize*8))/(delaybound - (1*packetSize*8)/1e9);
                }
                else if((sourcerates[0]==0 && sourcerates[1]!=0) || (sourcerates[1]==0 && sourcerates[0]!=0))
                {
                    delayBoundComp = (1*packetSize*8)/1e9 + (1*(packetSize*8))/minRate + 1*burstparam/minRate ;
                    boundCost = (2*burstparam+1*(packetSize*8))/(delaybound - (1*packetSize*8)/1e9);
                }
                else
                {
                    delayBoundComp = (1*packetSize*8)/1e9 + (1*(packetSize*8))/minRate + 2*burstparam/minRate ;
                    boundCost = (2*burstparam+1*(packetSize*8))/(delaybound - (1*packetSize*8)/1e9);
                }
                delay_bounds<<i<<","<<simTime().dbl()<<","<<delayBoundComp<<",";
            }
            else if(i==2 && simTime()>140)
            {
                sourceprices[i]=0;
                delay_bounds<<i<<","<<simTime().dbl()<<","<<delayBoundComp<<",";
            }
            //            EV<<"SDN......................Minimum Capacity for host  "<<i<<" = "<<mincaps[i]<<endl;

            //            if(i!=2)
            //            {
            ////                delayBoundComp = (2*(numhosts-1)*(burstparam+packetSize)+burstparam)/(mincaps[i] - sumrate);
            //                boundCost = (burstparam)/(delaybound);
            //
            //            }
            //            else
            //            {
            ////                delayBoundComp = (2*(numhosts-1)*(burstparam+packetSize)+burstparam)/(mincaps[i] - sumrate);
            //                boundCost = (1*(numhosts-1)*(burstparam+packetSize)+burstparam)/(delaybound);
            ////                EV<<"SDN......................BoundCost for host" <<i<<" = "<<(1*(numhosts-1)*(burstparam+packetSize)+burstparam)/(delaybound)<<endl;
            //            }

            EV<<"SDN......................Delaybound for host" <<i<<" = "<<delayBoundComp<<endl;
            EV<<"SDN......................Minimum Rate for host  "<<i<<" = "<<minRate<<endl;
            EV<<"SDN......................BoundCost for host" <<i<<" = "<<boundCost<<endl;
            //            EV<<"SDN......................Sum of others rates = "<<sumrate<<endl;
            EV<<"SDN......................Previous Price = "<<sourceprices[i]<<endl;

            //            EV<<"SDN...................... Computed Price = "<<sourceprices[i]-stepsize*(mincaps[i] - boundCost - sumrate)<<endl;

            EV<<"SDN...................... Computed Price = "<<sourceprices[i]-stepsize*(minRate - boundCost)<<endl;

            sourceprices[i] = fmax(sourceprices[i]-stepsize*(minRate - boundCost),0);
            //            if(i!=0 && simTime()<40)
            //            {
            //                sourceprices[i]=0;
            //            }
            //            else if(i==2 && simTime()>=40 && simTime()<80)
            //            {
            //                sourceprices[i]=0;
            //            }
            //            if(sourcerates[i]==0)
            //            {
            //                sourceprices[i]=0;
            //            }
            EV<<"SDN................ Price at SDN for host "<<i<< " = "<<sourceprices[i]<<endl;
            sdn->setRates(i,sourceprices[i]);
        }
        delay_bounds<<"\n";
        send(sdn->dup(),"torouter$o",0);
        send(sdn->dup(),"torouter$o",1);
        delete sdn;

    }

}

void SDN::finish()
{
    delay_bounds.close();
}
