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

Define_Module(Host);


void Host::initialize()
{
    // TODO - Generated method body

    packetlength = 1512;//in bytes
    datarate = 1; //in Mbps
    interpacketDuration = (double)(packetlength*8)/(datarate*1e6);
    gendata = new cMessage("gendata");
    scheduleAfter(interpacketDuration,gendata);
    endTxMsg = new cMessage("endTxMsg");
    as = 1e4;
    updatemsg = new cMessage("updatemsg");
}

void Host::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(msg==gendata)
    {
        scheduleAfter(interpacketDuration, gendata);
        cPacket *data = new cPacket("data");
        data->setByteLength(packetlength);
        data->setKind(intuniform(0,2));
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
            scheduleAt(txFinishTime, endTxMsg);
        }

    }
    else if(msg==endTxMsg)
    {
        send(dataQ.pop(), "torouter$o");
    }
    else if(strcmp(msg->getName(),"ack")==0)
    {

        delete msg;
    }
    else if(msg==updatemsg)
    {

    }
}
