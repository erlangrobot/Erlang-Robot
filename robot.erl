-module(robot).
-export([start/1 ,read_replies/1, move_forward/0]).
-export([sleep/1, read_ir/0, abc/1, def/1]).

start(Args) ->
   spawn(robot, read_ir, []),
   spawn(robot, move_forward, []),
   sleep(300000).

sleep(Time) -> 
	receive 
	after Time -> 
		true
	end.

def(K) -> 
	move ! K,
	read ! read_ir.

abc(Port) ->
   receive
	K -> 
   		Port ! {self(), {command, [49,K]}}
   end,
   abc(Port).


move_forward() -> 
   register(move,self()),
   Port = open_port({spawn,port},[{packet,2}]),
   abc(Port).



read_replies(Port) ->
	receive
	    read_ir -> 
   		Port ! {self(), {command, [50]}}; 
	         
	    {Port, {data, Any}} ->
		if 
		  Any == [97] -> def(9);
		  Any == [98] -> def(18);
		  Any == [99] -> def(10);
		  Any == [100] -> def(0);
		  Any == [101] -> def(17)
		end	
	end,
	read_replies(Port).

read_ir() -> 
   register(read,self()),
   Port = open_port({spawn,port},[{packet,2}]),
   Port ! {self(), {command, [50]}}, 
   read_replies(Port).
