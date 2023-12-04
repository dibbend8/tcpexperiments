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
#include "sdnmsg_m.h"
Define_Module(Host);


void Host::initialize()
{
    // TODO - Generated method body

    packetlength = getAncestorPar("packetSize").intValue();//in bytes
    maxrate = 100; //in Mbps
    minrate = 1;//Mbps
    datarate = minrate;
    interpacketDuration = (double)(packetlength*8)/(datarate*1e6);
    gendata = new cMessage("gendata");
    lp = 1e-2;
    as = 1e5;
    string filename = "datarates"+to_string(getIndex())+".csv";
    remove(filename.c_str());
    datarates.open(filename,ios::out | ios::app);
    updatemsg = new cMessage("updatemsg");
    if(getIndex()==0)
    {
        scheduleAfter(interpacketDuration,gendata);
    }
    else if(getIndex()==1)
    {
        scheduleAfter(10,gendata);
    }
    else if(getIndex()==2)
    {
        scheduleAfter(40,gendata);
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

    if(simTime()>=100 && getIndex()==0)
    {
        cancelEvent(gendata);
    }
    if(simTime()>=120 && getIndex()!=2)
    {
        cancelEvent(gendata);
    }
    if(simTime()>=140)
    {
        cancelEvent(gendata);
    }

    if(msg==gendata)
    {
        EV<<"Generating data for "<<getIndex()<<endl;

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


        scheduleAfter(interpacketDuration, gendata);
    }
    else if(strcmp(msg->getName(),"endTxMsg")==0)
    {
        send(dataQ.pop(), "torouter$o");
        delete msg;
    }
    else if(strcmp(msg->getName(),"ack")==0)
    {
        EV<<"******************** Received ECN from Router ****************** for HOST "<<getIndex()<<" AT "<<simTime()<<endl;
        Ack * ack = (Ack *)msg;
        double price1 = ack->getPrice1();
        double price2 = ack->getPrice2();
        price = price1+price2;
        delete msg;
    }
    else if(strcmp(msg->getName(),"sdnprices")==0)
    {

        Sdm * prices = (Sdm *) msg;
        EV<<"................... Updating Delay Prices from SDN ................for  HOST"<<getIndex()<<" at "<<simTime()<<endl;
        EV<<"Current Price = "<<price<<endl;
        int num_sources = prices->getRatesArraySize();
        double delayprice = 0;
        for(int i=0;i<num_sources;i++)
        {
            if(i!=getIndex())
            {
                delayprice+=prices->getRates(i);
            }
            if(getIndex()==0)
            {
                EV<<"Received prices for host "<<i <<" = "<< prices->getRates(i)<<endl;
            }
        }


        EV<<"Updated Price = "<<price+delayprice<<endl;
        EV<<"Previous Rate for Host "<<getIndex()<< " = "<<datarate<<endl;
        datarates<<getIndex()<<","<<simTime().dbl()<<","<<datarate<<"\n";
        EV<<"Computing the rate using equation = "<<(as/(price+delayprice))-1<<endl;
        datarate = fmin(fmax((as/(price+delayprice))-1,minrate),maxrate);

//        EV<<"Increment  = "<<lp*(as/(1+datarate) - price)<<endl;
//        datarate += lp*(as/(1+datarate) - price);

//        if(datarate>maxrate)
//        {
//            datarate = maxrate;
//        }
//        else if (datarate<minrate)
//        {
//            datarate = minrate;
//        }
        EV<<"Datarate after bound adjustment for "<<getIndex()<<" = "<<datarate<<endl;
        if(simTime()<10 && getIndex()==0)
        {

            interpacketDuration = (double)(packetlength*8)/(datarate*1e6);
            cancelEvent(gendata);
            scheduleAfter(interpacketDuration,gendata);
        }
        else if(simTime()>=10 && simTime()<40 && getIndex()!=2)
        {
            interpacketDuration = (double)(packetlength*8)/(datarate*1e6);
            cancelEvent(gendata);
            scheduleAfter(interpacketDuration,gendata);
        }
        else if(simTime()>=40)
        {
            interpacketDuration = (double)(packetlength*8)/(datarate*1e6);
            cancelEvent(gendata);
            scheduleAfter(interpacketDuration,gendata);
        }
        if(simTime()>=100 && getIndex()==0)
        {
            EV<<"IS scheduled generate data  for "<<getIndex()<<" = "<<gendata->isScheduled()<<endl;
            cancelEvent(gendata);
            EV<<"IS scheduled generate data  for "<<getIndex()<<" = "<<gendata->isScheduled()<<endl;

        }
        if(simTime()>=120 && getIndex()!=2)
        {
            cancelEvent(gendata);
        }
        if(simTime()>=140)
        {
            cancelEvent(gendata);
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
void Host::finish()
{
    datarates.close();
}
