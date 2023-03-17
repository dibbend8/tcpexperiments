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

#include "host.h"
#include "ack_m.h"
Define_Module(Host);


void Host::initialize()
{
    // TODO - Generated method body

    packetlength = 4000;//in bytes
    maxrate = 128; //in Mbps
    minrate = 1;//Mbps
    datarate = minrate;
    interpacketDuration = (double)(packetlength*8)/(datarate*1e6);
    gendata = new cMessage("gendata");

    as = 1e5;
    updatemsg = new cMessage("updatemsg");
    if(getIndex()==0)
    {
        scheduleAfter(interpacketDuration,gendata);
    }
    else
    {
        datarate=0;
    }
    price = 0;
}

void Host::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(msg==gendata)
    {
        scheduleAfter(interpacketDuration, gendata);
        cPacket *data = new cPacket("data");
        data->setByteLength(packetlength);
        data->setKind(getIndex());
        cChannel *txChannel = gate("torouter$o")->getTransmissionChannel();
        simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
        if (txFinishTime <= simTime()) {
            // channel free; send out packet immediately
            send(data, "torouter$o");
        }
        else {
            // store packet and schedule timer; when the timer expires,
            // the packet should be removed from the queue and sent out
            dataQ.insert(data);
            cMessage *endTxMsg = new cMessage("endTxMsg");
            scheduleAt(txFinishTime, endTxMsg);
        }
        if(simTime()>=120 && getIndex()==0)
        {
            cancelEvent(gendata);
        }
        else if(simTime()>=160 && getIndex()==1)
        {
            cancelEvent(gendata);
        }
        else if(simTime()>=200 && getIndex()==2)
        {
            cancelEvent(gendata);
        }

    }
    else if(strcmp(msg->getName(),"endTxMsg")==0)
    {
        send(dataQ.pop(), "torouter$o");
        delete msg;
    }
    else if(strcmp(msg->getName(),"ack")==0)
    {
        cout<<"******************** Received Prices from Router " <<msg->getSenderModule()->getIndex()<<" for HOST "<<getIndex()<<" AT "<<simTime()<<endl;
        Ack * ack = (Ack *)msg;
        double price1 = ack->getPrice1();
        double price2 = ack->getPrice2();
        cout<<"Price received from Router 0 = "<<price1<<endl;
        cout<<"Price received from Router 1 = "<<price2<<endl;
        cout<<"Previous Datarate  = "<<datarate<<endl;
        price = price1+price2;
        datarate = fmin(fmax((as/price)-1,minrate),maxrate);
        if(simTime()<40 && getIndex()==0)
        {

            cout<<"Price received at host "<<getIndex()<<" = "<<price<<" Datarate using price = "<<(as/price)-1<<endl;
            cout<<"Datarate after bound adjustment for "<<getIndex()<<" = "<<datarate<<endl;
            interpacketDuration = (double)(packetlength*8)/(datarate*1e6);
            cancelEvent(gendata);
            scheduleAfter(interpacketDuration,gendata);
        }
        else if(simTime()>=40 && simTime()<80 && getIndex()!=2)
        {
            cout<<"Price received at host "<<getIndex()<<" = "<<price<<" Datarate using price = "<<(as/price)-1<<endl;
            cout<<"Datarate after bound adjustment for "<<getIndex()<<" = "<<datarate<<endl;
            interpacketDuration = (double)(packetlength*8)/(datarate*1e6);
            cancelEvent(gendata);
            scheduleAfter(interpacketDuration,gendata);
        }
        else if(simTime()>=80)
        {
            cout<<"Price received at host "<<getIndex()<<" = "<<price<<" Datarate using price = "<<(as/price)-1<<endl;
            cout<<"Datarate after bound adjustment for "<<getIndex()<<" = "<<datarate<<endl;
            interpacketDuration = (double)(packetlength*8)/(datarate*1e6);
            cancelEvent(gendata);
            scheduleAfter(interpacketDuration,gendata);
        }

        delete msg;
    }
    //    else if(msg==updatemsg)
    //    {
    //
    //        scheduleAfter(50e-3,updatemsg);
    //
    //    }
}
