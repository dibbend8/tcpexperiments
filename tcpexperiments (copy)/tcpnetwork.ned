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
network TCPNet
{
    parameters:
        int numhost = default(3);
        double hostrate @unit(Mbps)= default(100Mbps);
        double routerrate @unit(Gbps) = default(1Gbps);
        int numrouter = default(3);
        int numserver = default(3);
    submodules:
        host[numhost]:Host;
        router[numrouter]:Router;
        server[numserver]:Server;
        sdn:SDN;
     connections allowunconnected:
  			host[0].torouter <--> {datarate=100Mbps;} <--> router[0].tohost++;
  			host[1].torouter <--> {datarate=100Mbps;} <--> router[0].tohost++;
  			host[2].torouter <--> {datarate=100Mbps;} <--> router[1].tohost++;
//  			server[0].torouter <--> {datarate=100Mbps;} <--> router[2].tohost++;
//  			server[1].torouter <--> {datarate=100Mbps;} <--> router[2].tohost++;
//  			server[2].torouter <--> {datarate=100Mbps;} <--> router[2].tohost++;
  			router[0].toserver++ <--> {datarate=100Mbps;} <--> router[1].torouter++;
  			router[1].toserver++ <--> {datarate=100Mbps;} <--> router[2].torouter++;
  			 for i= 0.. numrouter-1
        	{
        	    server[i].torouter <--> {datarate=100Mbps;} <--> router[2].toserver++;
        	    sdn.torouter++ <-->{datarate=100Mbps;}<-->router[i].tosdn++;
        	}   
  			
  			
}