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

#include "server.h"
#include "ack_m.h"
Define_Module(Server);

void Server::initialize()
{
    // TODO - Generated method body
}

void Server::handleMessage(cMessage *msg)
{
    // TODO - Generated method body

    if(strcmp(msg->getName(),"data")==0)
    {
        Ack *ack  = new Ack("ack");
        ack->setKind(msg->getKind());
        ack->setId(msg->getKind());
        ack->setPrice1(0);
        ack->setPrice2(0);
        send(ack, "torouter$o");
    }


    delete msg;
}
