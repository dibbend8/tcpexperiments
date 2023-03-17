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

network Network
{
    parameters:
        int numhost = default(2);
        double hostrate @unit(Mbps)= default(100Mbps);
        double routerrate @unit(Gbps) = default(1Gbps);
    submodules:
        host[numhost]:Host;
        router:Router;
        server:Server;
     connections allowunconnected:
        for i= 0.. numhost-1
        {
        	    host[i].torouter <-->{datarate=100Mbps;}<-->router.tohost++;
        }   
        router.toserver++ <-->{datarate=1Gbps;}<--> server.torouter;
}
