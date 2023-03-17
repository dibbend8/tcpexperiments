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

//
// TODO auto-generated type
//

simple PacketSource
{
    gates:
    	output out;
}
simple PacketSink
{
    gates:
        input in;
}


module TokenBucket {
    parameters:
        int maxTokens = 1000;
        int tokenRate = 100;
    gates:
        input in;
        output out;
}


network ExampleNetwork {
    submodules:
        source: PacketSource;
        destination: PacketSink;
        tokenBucket: TokenBucket;
    connections:
        source.out --> tokenBucket.in;
        tokenBucket.out --> destination.in;
}
network NoTokenNet
{
    submodules:
        source:Host;
        dest: Sink;
   	connections:
   	    source.tolink <-->{datarate=10 Mbps;} <--> dest.in;
}
            
