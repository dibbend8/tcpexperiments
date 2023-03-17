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
    updateprice = new cMessage("updateprice");
    updateinterval = 500e-3; //every 500 ms
    scheduleAfter(updateinterval, updateprice);
    estrate = 0;totalrecv=0;
    endTxMsg = new cMessage("endTxMsg");
    lp=1e-5;
    price=0;
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
            //            cout<<"Scheduling End of Transmission at "<<txFinishTime<<endl;
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
            //            cout<<"Scheduling End of Transmission at "<<txFinishTime<<endl;
            scheduleAt(txFinishTime, endTxMsg);
        }

    }
    else if(msg==updateprice && getIndex()!=2)
    {
//        cout<<"!!!!!!!!!!!!!!!!!!!! UPDATING PRICES AT ROUTER "<<getIndex()<<"!!!!!!!!!! AT "<<simTime()<<endl;
        scheduleAfter(updateinterval, updateprice);
        estrate = (double)(totalrecv*4000*8)/updateinterval;
//        cout<<"Estimated link rate at router "<<getIndex()<<" = "<<estrate<<" total received = "<<totalrecv<<endl;
        cDatarateChannel * incomech = (cDatarateChannel*)gate("toserver$o",0)->getChannel();
        double c = incomech->getDatarate();
        price  =  fmax(price+lp*(estrate-c),0);
//        cout<<"Updated Price for "<<getIndex()<<" is "<<price<<endl;
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
        //        else
        //        {
        //            ack->setPrice1(price);
        //            send(ack,"tohost$o",0);
        //            send(ack->dup(),"tohost$o",1);
        //
        //        }

    }
    else if(strcmp(msg->getName(),"ack")==0)
    {
        //        if(getIndex()==1)
        //        {
        //            send(msg,"tohost$o",0);
        //
        //        }
        //        else
        //        {
        EV<<"ACK received at Router "<<getIndex()<<endl;
        Ack * ack = (Ack*)msg;
        ack->setPrice1(price);
        send(ack->dup(),"tohost$o",0);
        send(ack->dup(),"tohost$o",1);

        delete msg;
        //        }
    }
    //    else if(strcmp(msg->getName(),"ack")==0)
    //    {
    //        Ack * ack = (Ack*) msg;
    //        if(getIndex()==2)
    //        {
    //            ack->setPrice2(price);
    //            send(ack, "torouter$o",0);
    //        }
    //        else if(getIndex()==1 && msg->getKind()==2)
    //        {
    //            send(ack,"tohost$o",0);
    //        }
    //        else if(getIndex()==0)
    //        {
    //            send(ack,"tohost$o",msg->getKind());
    //        }
    //        else
    //        {
    //            ack->setPrice1(price);
    //            send(ack,"torouter$o",0);
    //        }
    //    }
}
