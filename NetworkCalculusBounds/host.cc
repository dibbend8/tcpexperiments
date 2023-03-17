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
    gendata = new cMessage("gendata");
    endTxMsg = new cMessage("endTx");
    pktsize = par("packetSize").intValue();
    rate = par("genrate").doubleValue();
    interpacketDuration = (double)(pktsize*8)/rate;
    cout<<"Arrival Rate = "<<rate<<" interpacket duration = "<<interpacketDuration<<endl;
    scheduleAfter(interpacketDuration, gendata);

}

void Host::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(msg == gendata)
    {
        cPacket *pkt = new cPacket("data");
        pkt->setByteLength(pktsize);
        cChannel *txChannel = gate("tolink$o")->getTransmissionChannel();
        simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
        cout<<"Current time  = "<<simTime()<<"  Next finish Time = "<<txFinishTime<<endl;
        if (txFinishTime <= simTime()) {
            // channel free; send out packet immediately
            send(pkt, "tolink$o");
        }
        else {
            // store packet and schedule timer; when the timer expires,
            // the packet should be removed from the queue and sent out
            txQueue.insert(pkt);
            scheduleAt(txFinishTime, endTxMsg);
        }
        cancelEvent(gendata);
        scheduleAfter(interpacketDuration, gendata);
    }
    else if(msg==endTxMsg)
    {
        cChannel *txChannel = gate("tolink$o")->getTransmissionChannel();
        simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
        if (txFinishTime <= simTime()) {
            // channel free; send out packet immediately
            send(txQueue.pop(), "tolink$o");
        }
        else {
            // store packet and schedule timer; when the timer expires,
            // the packet should be removed from the queue and sent out
//            cancelEvent(endTxMsg);
            scheduleAt(txFinishTime, endTxMsg);
        }

    }
}
