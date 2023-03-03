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
#include "ack_m.h"
Define_Module(Router);

void Router::initialize()
{
    // TODO - Generated method body

    //    reqQ = new cQueue("reqQ");
    //    servrate = getParentModule()->par("routerrate").doubleValue()*1e9;
    //    serve = new cMessage("serve");
    endTxMsg = new cMessage("endTxmsg");
    updateprice = new cMessage("updateprice");
    scheduleAfter(10e-3, updateprice);
    estrate = 0;totalrecv=0;
}

void Router::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(strcmp(msg->getName(),"data")==0)
    {
        cChannel *txChannel = gate("toserver$o",0)->getTransmissionChannel();
        simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
        cPacket *data = check_and_cast<cPacket *> (msg);
        totalrecv+=data->getByteLength();
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
    else if(msg==updateprice && getIndex()!=0)
    {
        estrate = (totalrecv*8)/simTime().dbl();
        cout<<"Estimated link rate at router "<<getIndex()<<" = "<<estrate<<endl;
        cDatarateChannel * incomech = (cDatarateChannel*)gate("torouter$o",0)->getChannel();
        double c = incomech->getDatarate();
        price  -=  lp*fmax((estrate-c),0);

        cout<<"Updated Price for "<<getIndex()<<" is "<<price<<endl;
        scheduleAfter(10e-3, updateprice);


    }
    else if(strcmp(msg->getName(),"ack")==0)
    {
        Ack * ack = (Ack*) msg;
        if(getIndex()==2)
        {
            ack->setPrice2(price);
            send(ack, "torouter$o",0);
        }
        else if(getIndex()==1 && msg->getKind()==2)
        {
            send(ack,"tohost$o",0);
        }
        else if(getIndex()==0)
        {
            send(ack,"tohost$o",msg->getKind());
        }
        else
        {
            ack->setPrice1(price);
            send(ack,"torouter$o",0);
        }
    }
}
