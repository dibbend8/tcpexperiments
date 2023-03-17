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
#include "sdnmsg_m.h"
Define_Module(Router);

void Router::initialize()
{
    // TODO - Generated method body

    //    reqQ = new cQueue("reqQ");
    //    servrate = getParentModule()->par("routerrate").doubleValue()*1e9;
    //    serve = new cMessage("serve");
    updateprice = new cMessage("updateprice");
    updateinterval = 500e-3; //every 500 ms
    scheduleAfter(updateinterval, updateprice);
    estrate = 0;totalrecv=0;
    endTxMsg = new cMessage("endTxMsg");
    lp=1e-5;
    price=0;
    numhosts = getAncestorPar("numhosts").intValue();
    for(int i=0;i<numhosts;i++)
    {
        recv.push_back(0);
    }
}

void Router::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(strcmp(msg->getName(),"data")==0)
    {
        cChannel *txChannel = gate("toserver$o",0)->getTransmissionChannel();
        simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
        cPacket *data = check_and_cast<cPacket *> (msg);
        totalrecv++;
        recv[data->getKind()]++;
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
            cancelEvent(endTxMsg);
            //            EV<<"Scheduling End of Transmission at "<<txFinishTime<<endl;
            scheduleAt(txFinishTime, endTxMsg);
        }
    }
    else if(msg==endTxMsg)
    {
        cChannel *txChannel = gate("toserver$o",0)->getTransmissionChannel();
        simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
        if (txFinishTime <= simTime()) {
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
        else
        {
            cancelEvent(endTxMsg);
            //            EV<<"Scheduling End of Transmission at "<<txFinishTime<<endl;
            scheduleAt(txFinishTime, endTxMsg);
        }

    }
    else if(msg==updateprice && getIndex()!=2)
    {
        EV<<"!!!!!!!!!!!!!!!!!!!! UPDATING PRICES AT ROUTER "<<getIndex()<<"!!!!!!!!!! AT "<<simTime()<<endl;
        scheduleAfter(updateinterval, updateprice);
        estrate = (double)(totalrecv*4000*8)/updateinterval;
        Sdm *sdnmsg = new Sdm("rates");
        sdnmsg->setRatesArraySize(numhosts);

        if(getIndex()==0)
        {
            sdnmsg->setRates(0, (double) (recv[0]*4000*8)/updateinterval);
            sdnmsg->setRates(1, (double) (recv[1]*4000*8)/updateinterval);
        }
        else
        {
            sdnmsg->setRates(0, (double) (recv[2]*4000*8)/updateinterval);
        }
        send(sdnmsg,"tosdn$o",0);
        for(int i=0;i<numhosts;i++)
        {
            recv[i] = 0;
        }
        EV<<"Estimated link rate at router "<<getIndex()<<" = "<<estrate<<" total received = "<<totalrecv<<endl;
        cDatarateChannel * incomech = (cDatarateChannel*)gate("toserver$o",0)->getChannel();
        double c = incomech->getDatarate();
        EV<<"Previous Price for Router "<<getIndex()<<" = "<<price<<endl;
        EV<<"Computing the difference in rates  = "<<(estrate-c)<<endl;
        EV<<"Increment Step  = "<<lp*(estrate-c)<<endl;
        price  =  fmax(price+lp*(estrate-c),0);
        EV<<"Updated Price for "<<getIndex()<<" is "<<price<<endl;
        totalrecv=0;
        Ack *ack =  new Ack("ack");

        if(getIndex()==1)
        {
            ack->setPrice2(price);
            Ack* ackcopy = ack->dup();
            send(ack->dup(), "torouter$o",0);
            ackcopy->setPrice1(0);
            send(ackcopy,"tohost$o",0);
        }
        delete ack;
    }
    else if(strcmp(msg->getName(),"ack")==0)
    {

        EV<<"ACK received at Router "<<getIndex()<<endl;
        Ack * ack = (Ack*)msg;
        ack->setPrice1(price);
        send(ack->dup(),"tohost$o",0);
        send(ack->dup(),"tohost$o",1);
        delete msg;
    }
    else if(strcmp(msg->getName(),"sdnprices")==0)
        {
            EV<<"Router "<<getIndex()<<"................. Received prices from SDN "<<endl;
            Sdm *sdn = (Sdm*) msg;
            if(getIndex()==0)
            {
                EV<<"Price for host 0 = "<<sdn->getRates(0)<<endl;
                EV<<"  Price for host 1 = "<<sdn->getRates(1)<<endl;
                send(sdn->dup(),"tohost$o",0);
                send(sdn->dup(),"tohost$o",1);
            }
            else
            {
                EV<<"Price for host 2 = "<<sdn->getRates(2)<<endl;
                send(sdn->dup(),"tohost$o",0);
            }
            delete msg;
        }
}
