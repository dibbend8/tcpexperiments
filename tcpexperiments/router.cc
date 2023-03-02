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

#include "router.h"

Define_Module(Router);

void Router::initialize()
{
    // TODO - Generated method body

    //    reqQ = new cQueue("reqQ");
    //    servrate = getParentModule()->par("routerrate").doubleValue()*1e9;
    //    serve = new cMessage("serve");
    endTxMsg = new cMessage("endTxmsg");
    updateprice = new cMessage("updateprice");
    scheduleAfter(50e-3, updateprice);
    estrate = 0;totalrecv=0;
}

void Router::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(strcmp(msg->getName(),"data")==0)
    {
        cChannel *txChannel = gate("toserver$o",0)->getTransmissionChannel();
        simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
        cPacket *data = (cPacket *) msg;
        totalrecv+=data->getBitLength();
        reqQ.insert(data);
        if (txFinishTime <= simTime()) {
            // channel free; send out packet immediately

            if(getIndex()==2)
            {
                send(reqQ.pop(), "toserver$o",data->getKind());
            }
            else
            {
                send(reqQ.pop(), "toserver$o",0);
            }
        }
        else {
            // stores packet and schedule timer; when the timer expires,
            // the packet should be removed from the queue and sent out
//            reqQ.insert(data);
            scheduleAt(txFinishTime, endTxMsg);
        }
    }
    else if(msg==endTxMsg)
    {
        if(getIndex()==2)
        {
            cPacket *pkt = reqQ.pop();
            send(pkt, "toserver$o",pkt->getKind());
        }
        else
        {
            send(reqQ.pop(), "toserver$o",0);
        }
    }
    else if(strcmp(msg->getName(),"ack")==0)
    {
       if(getIndex()==2)
       {
           send(msg, "torouter$o",0);
       }
       else if(getIndex()==1 && msg->getKind()==2)
       {
           send(msg,"tohost$o",0);
       }
       else if(getIndex()==0)
       {
           send(msg,"tohost$o",msg->getKind());
       }
       else
       {
           send(msg,"torouter$o",0);
       }
    }
    else if(msg==updateprice)
    {
        estrate = totalrecv/simTime().dbl();

    }
}
